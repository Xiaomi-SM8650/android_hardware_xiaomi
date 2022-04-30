/*
 * Copyright (C) 2020 The Android Open Source Project
 *               2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Fingerprint.h"

#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>

using aidl::android::hardware::biometrics::fingerprint::Fingerprint;

int main() {
    LOG(INFO) << "Fingerprint HAL started";
    ABinderProcess_setThreadPoolMaxThreadCount(0);
    std::shared_ptr<Fingerprint> hal = ndk::SharedRefBase::make<Fingerprint>();

    const std::string instance = std::string(Fingerprint::descriptor) + "/virtual";
    binder_status_t status =
            AServiceManager_registerLazyService(hal->asBinder().get(), instance.c_str());
    CHECK_EQ(status, STATUS_OK);
    AServiceManager_forceLazyServicesPersist(true);

    ABinderProcess_joinThreadPool();
    return EXIT_FAILURE;  // should not reach
}
