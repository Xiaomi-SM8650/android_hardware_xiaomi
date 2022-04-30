/*
 * Copyright (C) 2022 The Android Open Source Project
 *               2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "FakeFingerprintEngineUdfps.h"

#include <android-base/logging.h>

#include <fingerprint.sysprop.h>

#include "Fingerprint.h"
#include "util/CancellationSignal.h"
#include "util/Util.h"

#undef LOG_TAG
#define LOG_TAG "FingerprintVirtualHalUdfps"

using namespace ::android::fingerprint::virt;

namespace aidl::android::hardware::biometrics::fingerprint {

FakeFingerprintEngineUdfps::FakeFingerprintEngineUdfps()
    : FakeFingerprintEngine(), mPointerDownTime(0), mUiReadyTime(0) {}

SensorLocation FakeFingerprintEngineUdfps::defaultSensorLocation() {
    return SensorLocation{.sensorLocationX = defaultSensorLocationX,
                          .sensorLocationY = defaultSensorLocationY,
                          .sensorRadius = defaultSensorRadius};
}

ndk::ScopedAStatus FakeFingerprintEngineUdfps::onPointerDownImpl(int32_t /*pointerId*/,
                                                                 int32_t /*x*/, int32_t /*y*/,
                                                                 float /*minor*/, float /*major*/) {
    BEGIN_OP(0);
    // verify whetehr touch coordinates/area matching sensor location ?
    mPointerDownTime = Util::getSystemNanoTime();
    if (Fingerprint::cfg().get<bool>("control_illumination")) {
        fingerDownAction();
    }
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus FakeFingerprintEngineUdfps::onPointerUpImpl(int32_t /*pointerId*/) {
    BEGIN_OP(0);
    mUiReadyTime = 0;
    mPointerDownTime = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus FakeFingerprintEngineUdfps::onUiReadyImpl() {
    BEGIN_OP(0);

    if (Util::hasElapsed(mPointerDownTime, uiReadyTimeoutInMs * 100)) {
        LOG(ERROR) << "onUiReady() arrives too late after onPointerDown()";
    } else {
        fingerDownAction();
    }
    return ndk::ScopedAStatus::ok();
}

void FakeFingerprintEngineUdfps::fingerDownAction() {
    FakeFingerprintEngine::fingerDownAction();
    mUiReadyTime = 0;
    mPointerDownTime = 0;
}

void FakeFingerprintEngineUdfps::updateContext(WorkMode mode, ISessionCallback* cb,
                                               std::future<void>& cancel, int64_t operationId,
                                               const keymaster::HardwareAuthToken& hat) {
    FakeFingerprintEngine::updateContext(mode, cb, cancel, operationId, hat);
    mPointerDownTime = 0;
    mUiReadyTime = 0;
}

}  // namespace aidl::android::hardware::biometrics::fingerprint
