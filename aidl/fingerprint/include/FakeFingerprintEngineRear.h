/*
 * Copyright (C) 2022 The Android Open Source Project
 *               2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include "FakeFingerprintEngine.h"

using namespace ::aidl::android::hardware::biometrics::common;

namespace aidl::android::hardware::biometrics::fingerprint {

// A fake engine that is backed by system properties instead of hardware.
class FakeFingerprintEngineRear : public FakeFingerprintEngine {
  public:
    FakeFingerprintEngineRear() : FakeFingerprintEngine() {}
    ~FakeFingerprintEngineRear() {}
};

}  // namespace aidl::android::hardware::biometrics::fingerprint
