/*
 * Copyright (C) 2020 The Android Open Source Project
 *               2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/android/hardware/biometrics/fingerprint/BnFingerprint.h>

#include "FakeFingerprintEngine.h"
#include "FakeFingerprintEngineRear.h"
#include "FakeFingerprintEngineSide.h"
#include "FakeFingerprintEngineUdfps.h"

#include "FingerprintConfig.h"
#include "Session.h"
#include "thread/WorkerThread.h"

namespace aidl::android::hardware::biometrics::fingerprint {

class Fingerprint : public BnFingerprint {
  public:
    Fingerprint();

    ndk::ScopedAStatus getSensorProps(std::vector<SensorProps>* out) override;

    ndk::ScopedAStatus createSession(int32_t sensorId, int32_t userId,
                                     const std::shared_ptr<ISessionCallback>& cb,
                                     std::shared_ptr<ISession>* out) override;
    binder_status_t dump(int fd, const char** args, uint32_t numArgs);
    binder_status_t handleShellCommand(int in, int out, int err, const char** argv, uint32_t argc);

    static FingerprintConfig& cfg() {
        static FingerprintConfig* cfg = nullptr;
        if (cfg == nullptr) {
            cfg = new FingerprintConfig();
            cfg->init();
        }
        return *cfg;
    }

  private:
    void resetConfigToDefault();
    void onHelp(int);
    void onSimFingerDown();
    void clearConfigSysprop();

    std::unique_ptr<FakeFingerprintEngine> mEngine;
    WorkerThread mWorker;
    std::shared_ptr<Session> mSession;
    FingerprintSensorType mSensorType;
};

}  // namespace aidl::android::hardware::biometrics::fingerprint
