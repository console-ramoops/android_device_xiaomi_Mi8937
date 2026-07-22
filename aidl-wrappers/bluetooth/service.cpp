/*
 * Copyright (C) 2024-2026 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "BluetoothHci.h"

#include <android/binder_manager.h>
#include <android/binder_process.h>
#include <android-base/logging.h>

using ::aidl::android::hardware::bluetooth::impl::BluetoothHci;

int main() {
    LOG(INFO) << "Xiaomi MSM8937 Bluetooth AIDL-to-HIDL service is starting...";

    ABinderProcess_setThreadPoolMaxThreadCount(0);

    std::shared_ptr<BluetoothHci> bluetoothHci =
        ::ndk::SharedRefBase::make<BluetoothHci>();
    const std::string instance = std::string() + BluetoothHci::descriptor + "/default";

    binder_status_t status = AServiceManager_addService(
        bluetoothHci->asBinder().get(), instance.c_str());

    CHECK(status == STATUS_OK) << "Failed to register " << instance << " service";

    LOG(INFO) << "Successfully registered Bluetooth AIDL service: " << instance;

    ABinderProcess_joinThreadPool();
    return EXIT_FAILURE;
}
