/*
 * Copyright (C) 2024-2026 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "BluetoothHci.h"

#include <android-base/logging.h>

namespace aidl {
namespace android {
namespace hardware {
namespace bluetooth {
namespace impl {

::ndk::ScopedAStatus BluetoothHci::initialize(
    const std::shared_ptr<IBluetoothHciCallbacks>& cb) {
    if (cb == nullptr) {
        LOG(ERROR) << "Callback passed to initialize is null";
        return ::ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_ARGUMENT);
    }

    LOG(INFO) << "Initializing Bluetooth AIDL-to-HIDL wrapper";

    mHidlHci = ::android::hardware::bluetooth::V1_0::IBluetoothHci::getService();
    if (mHidlHci == nullptr) {
        LOG(ERROR) << "Failed to get legacy HIDL Bluetooth service";
        cb->initializationComplete(Status::INITIALIZATION_ERROR);
        return ::ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }

    mHidlCallback = new BluetoothHciHidlCallback(cb);
    auto status = mHidlHci->initialize(mHidlCallback);
    if (!status.isOk()) {
        LOG(ERROR) << "Failed to initialize HIDL Bluetooth HAL";
        cb->initializationComplete(Status::INITIALIZATION_ERROR);
        return ::ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }

    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus BluetoothHci::sendHciCommand(
    const std::vector<uint8_t>& command) {
    if (mHidlHci == nullptr) {
        return ::ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }

    hidl_vec<uint8_t> data(command.begin(), command.end());
    mHidlHci->sendHciCommand(data);
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus BluetoothHci::sendAclData(
    const std::vector<uint8_t>& data) {
    if (mHidlHci == nullptr) {
        return ::ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }

    hidl_vec<uint8_t> vec(data.begin(), data.end());
    mHidlHci->sendAclData(vec);
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus BluetoothHci::sendScoData(
    const std::vector<uint8_t>& data) {
    if (mHidlHci == nullptr) {
        return ::ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }

    hidl_vec<uint8_t> vec(data.begin(), data.end());
    mHidlHci->sendScoData(vec);
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus BluetoothHci::sendIsoData(
    const std::vector<uint8_t>& /*data*/) {
    return ::ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
}

::ndk::ScopedAStatus BluetoothHci::close() {
    LOG(INFO) << "Closing Bluetooth AIDL HAL wrapper";
    if (mHidlHci != nullptr) {
        mHidlHci->close();
        mHidlHci = nullptr;
    }
    return ::ndk::ScopedAStatus::ok();
}

}  // namespace impl
}  // namespace bluetooth
}  // namespace hardware
}  // namespace android
}  // namespace aidl
