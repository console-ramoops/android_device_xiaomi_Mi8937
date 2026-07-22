/*
 * Copyright (C) 2024-2026 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "BiometricsFingerprintAidl.h"

#include <android/binder_manager.h>
#include <android/binder_process.h>
#include <android-base/logging.h>

using ::aidl::android::hardware::biometrics::fingerprint::BiometricsFingerprintAidl;

int main() {
    LOG(INFO) << "Xiaomi Fingerprint AIDL-to-HIDL service is starting...";

    ABinderProcess_setThreadPoolMaxThreadCount(0);

    std::shared_ptr<BiometricsFingerprintAidl> fingerprint =
        ::ndk::SharedRefBase::make<BiometricsFingerprintAidl>();
    const std::string instance = std::string() + BiometricsFingerprintAidl::descriptor + "/default";

    binder_status_t status = AServiceManager_addService(
        fingerprint->asBinder().get(), instance.c_str());

    CHECK(status == STATUS_OK) << "Failed to register " << instance << " service";

    LOG(INFO) << "Successfully registered Fingerprint AIDL service: " << instance;

    ABinderProcess_joinThreadPool();
    return EXIT_FAILURE;
}
