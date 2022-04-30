/*
 * Copyright (C) 2022 The Android Open Source Project
 *               2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <android/binder_to_string.h>
#include <stdint.h>
#include <string>

namespace aidl::android::hardware::biometrics::fingerprint {

class FakeLockoutTracker {
  public:
    FakeLockoutTracker() : mFailedCount(0) {}
    ~FakeLockoutTracker() {}

    enum class LockoutMode : int8_t { kNone = 0, kTimed, kPermanent };

    void reset();
    LockoutMode getMode();
    void addFailedAttempt();
    int64_t getLockoutTimeLeft();
    inline std::string toString() const {
        std::ostringstream os;
        os << "----- FakeLockoutTracker:: -----" << std::endl;
        os << "FakeLockoutTracker::mFailedCount:" << mFailedCount;
        os << ", FakeLockoutTracker::mCurrentMode:" << (int)mCurrentMode;
        os << std::endl;
        return os.str();
    }

  private:
    int32_t mFailedCount;
    int64_t mLockoutTimedStart;
    LockoutMode mCurrentMode;
};

}  // namespace aidl::android::hardware::biometrics::fingerprint
