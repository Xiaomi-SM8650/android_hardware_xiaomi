/*
 * Copyright (C) 2022 The Android Open Source Project
 *               2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include "FingerprintEngine.h"

using namespace ::aidl::android::hardware::biometrics::common;

namespace aidl::android::hardware::biometrics::fingerprint {

// An engine that is backed by system properties instead of hardware.
class FingerprintEngineSide : public FingerprintEngine {
  public:
    static constexpr int32_t defaultSensorLocationX = 0;
    static constexpr int32_t defaultSensorLocationY = 600;
    static constexpr int32_t defaultSensorRadius = 150;

    FingerprintEngineSide();
    ~FingerprintEngineSide() {}

    virtual SensorLocation defaultSensorLocation() override;
};

}  // namespace aidl::android::hardware::biometrics::fingerprint
