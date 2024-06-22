/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.xiaomi.hw.touchfeature;

interface ITouchFeature {
    int setModeValue(int touchId, int ControlMode, int ModeValue);
    int getModeCurValue(int touchId, int ControlMode);
    String getModeCurValueString(int touchId, int ControlMode);
    int getModeMaxValue(int touchId, int ControlMode);
    int getModeMinValue(int touchId, int ControlMode);
    int getModeDefaultValue(int touchId, int ControlMode);
    int modeReset(int touchId, int ControlMode);
    int[] getModeValue(int touchId, int mode);
    int setModeLongValue(int touchId, int ControlMode, int ValueLen, in int[] ValueBuf);
    String getModeWhiteList(int ValueLen, in int[] ValueBuf);
    String getTouchEvent();
}
