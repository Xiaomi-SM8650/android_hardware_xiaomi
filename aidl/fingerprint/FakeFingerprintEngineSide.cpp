/*
 * Copyright (C) 2022 The Android Open Source Project
 *               2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "FakeFingerprintEngineSide.h"

#include <android-base/logging.h>

#include <fingerprint.sysprop.h>

#include "util/CancellationSignal.h"
#include "util/Util.h"

using namespace ::android::fingerprint::virt;

namespace aidl::android::hardware::biometrics::fingerprint {

FakeFingerprintEngineSide::FakeFingerprintEngineSide() : FakeFingerprintEngine() {}

SensorLocation FakeFingerprintEngineSide::defaultSensorLocation() {
    return SensorLocation{.sensorLocationX = defaultSensorLocationX,
                          .sensorLocationY = defaultSensorLocationY,
                          .sensorRadius = defaultSensorRadius};
}
}  // namespace aidl::android::hardware::biometrics::fingerprint
