/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package org.ifaa.aidl.manager;

import android.app.Service;
import android.content.Intent;
import android.os.Build;
import android.os.IBinder;
import android.os.RemoteException;
import android.os.ServiceManager;
import android.os.SystemProperties;
import android.util.Log;
import java.lang.ref.WeakReference;
import org.json.JSONObject;
import vendor.xiaomi.hardware.mlipay.IMlipayService;

public class IfaaService extends Service {
    private static final String LOG_TAG = IfaaService.class.getSimpleName();

    private static final int AUTH_TYPE_NOT_SUPPORT = 0;
    private static final int AUTH_TYPE_FINGERPRINT = 1;
    private static final int AUTH_TYPE_IRIS = 1<<1;
    private static final int AUTH_TYPE_OPTICAL_FINGERPRINT = 1<<4;

    private static final int ACTIVITY_START_SUCCESS = 0;
    private static final int ACTIVITY_START_FAILED = -1;

    private static final int IFAA_VERSION = 4;
    private static final String IFAASERVICE_AIDL_INTERFACE = "vendor.xiaomi.hardware.mlipay.IMlipayService/default";

    private static boolean sIsFod = SystemProperties.getBoolean("ro.hardware.fp.fod", false);

    private static final String mFingerActName = "com.android.settings.NewFingerprintActivity";
    private static final String mFingerPackName = "com.android.settings";

    private IMlipayService mMlipayService = null;

    private final IBinder mIFAABinder = new IfaaServiceStub(this);

    private final class IfaaServiceStub extends IfaaManagerService.Stub {
        IfaaServiceStub(IfaaService ifaaService) {
            new WeakReference(ifaaService);
        }

        @Override
        public int getSupportBIOTypes() {
            int ifaaProp = SystemProperties.getInt("persist.vendor.sys.pay.ifaa", 0);
            String fpVendor = SystemProperties.get("persist.vendor.sys.fp.vendor", "");

            int res = "none".equalsIgnoreCase(fpVendor) ?
                    ifaaProp & AUTH_TYPE_IRIS :
                    ifaaProp & (AUTH_TYPE_FINGERPRINT | AUTH_TYPE_IRIS);

            if ((res & AUTH_TYPE_FINGERPRINT) == AUTH_TYPE_FINGERPRINT && sIsFod) {
                res |= AUTH_TYPE_OPTICAL_FINGERPRINT;
            }

            return res;
        }

        @Override
        public int startBIOManager(int authType) {
            int res = ACTIVITY_START_FAILED;

            if (authType == AUTH_TYPE_FINGERPRINT) {
                Intent intent = new Intent();
                intent.setClassName(mFingerPackName, mFingerActName);
                intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                getApplicationContext().startActivity(intent);
                res = ACTIVITY_START_SUCCESS;
            }

            return res;
        }

        @Override
        public String getDeviceModel() {
            return Build.MANUFACTURER + "-" + Build.DEVICE;
        }

        @Override
        public byte[] processCmd(byte[] param) {
            try {
                if (getMlipayService() != null) {
                    byte[] receiveBufferByteArray = mMlipayService.invoke_command(param, param.length);
                    int length = receiveBufferByteArray.length;
                    byte[] receiveBuffer = new byte[length];
                    for (int i = 0; i < length; i++) {
                        receiveBuffer[i] = receiveBufferByteArray[i];
                    }
                    return receiveBuffer;
                }
                Log.e(LOG_TAG, "[processCmd] IMlipayService not found");
                return null;
            } catch (RemoteException e) {
                Log.e(LOG_TAG, "[processCmd] transact fail: " + e);
                return null;
            }
        }

        @Override
        public int getVersion() {
            return IFAA_VERSION;
        }

        @Override
        public String getExtInfo(int authType, String keyExtInfo) {
            return initExtString();
        }

        @Override
        public void setExtInfo(int authType, String keyExtInfo, String valExtInfo) {
            // nothing
        }

        @Override
        public int getEnabled(int bioType) {
            return 1 == bioType ? 1000 : 1003;
        }

        @Override
        public int[] getIDList(int bioType) {
            try {
                if (getMlipayService() != null) {
                    int[] ifaa_get_idlist = mMlipayService.ifaa_get_idlist(bioType);
                    int length = ifaa_get_idlist.length;
                    int[] idList = new int[length];
                    for (int i = 0; i < length; i++) {
                        idList[i] = ifaa_get_idlist[i];
                    }
                    return idList;
                }
                Log.e(LOG_TAG, "[getIDList] IMlipayService not found");
                return null;
            } catch (RemoteException e) {
                Log.e(LOG_TAG, "[getIDList] transact fail " + e);
                return null;
            }
        }
    };

    @Override // android.app.Service
    public IBinder onBind(Intent intent) {
        return this.mIFAABinder;
    }

    // Utils
    private IMlipayService getMlipayService() throws RemoteException {
        if (mMlipayService == null) {
            IBinder service = ServiceManager.getService(IFAASERVICE_AIDL_INTERFACE);
            IMlipayService asInterface = IMlipayService.Stub.asInterface(service);
            mMlipayService = asInterface;
            if (asInterface == null) {
                Log.e(LOG_TAG, "Getting IMlipayService AIDL daemon interface failed!");
            } else {
                Log.d(LOG_TAG, "IMlipayService AIDL daemon interface is binded!");
            }
        }
        return mMlipayService;
    }

    private String initExtString() {
        JSONObject obj = new JSONObject();
        JSONObject keyInfo = new JSONObject();
        String xy = SystemProperties.get("persist.vendor.sys.fp.fod.location.X_Y", "");
        String wh = SystemProperties.get("persist.vendor.sys.fp.fod.size.width_height", "");
        try {
            if (!validateVal(xy) || !validateVal(wh)) {
                Log.e(LOG_TAG, "initExtString: invalidate, xy: " + xy + ", wh: " + wh);
                return "";
            }
            String[] split = xy.split(",");
            String[] split2 = wh.split(",");
            keyInfo.put("startX", Integer.parseInt(split[0]));
            keyInfo.put("startY", Integer.parseInt(split[1]));
            keyInfo.put("width", Integer.parseInt(split2[0]));
            keyInfo.put("height", Integer.parseInt(split2[1]));
            keyInfo.put("navConflict", true);
            obj.put("type", 0);
            obj.put("fullView", keyInfo);
            return obj.toString();
        } catch (Exception e) {
            Log.e(LOG_TAG, "initExtString: Exception, xy: " + xy + ", wh: " + wh, e);
            return "";
        }
    }

    private boolean validateVal(String str) {
        return !"".equalsIgnoreCase(str) && str.contains(",");
    }
}
