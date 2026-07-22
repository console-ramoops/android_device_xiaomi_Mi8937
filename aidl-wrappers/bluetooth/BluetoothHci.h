/*
 * Copyright (C) 2024-2026 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/android/hardware/bluetooth/BnBluetoothHci.h>
#include <aidl/android/hardware/bluetooth/IBluetoothHciCallbacks.h>
#include <android/hardware/bluetooth/1.0/IBluetoothHci.h>
#include <android/hardware/bluetooth/1.0/IBluetoothHciCallbacks.h>
#include <hidl/HidlSupport.h>

namespace aidl {
namespace android {
namespace hardware {
namespace bluetooth {
namespace impl {

using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;

class BluetoothHciHidlCallback : public ::android::hardware::bluetooth::V1_0::IBluetoothHciCallbacks {
  public:
    BluetoothHciHidlCallback(const std::shared_ptr<IBluetoothHciCallbacks>& cb)
        : mCb(cb) {}

    Return<void> initializationComplete(::android::hardware::bluetooth::V1_0::Status status) override {
        if (mCb) {
            mCb->initializationComplete(
                status == ::android::hardware::bluetooth::V1_0::Status::SUCCESS
                    ? Status::SUCCESS
                    : Status::INITIALIZATION_ERROR);
        }
        return Void();
    }

    Return<void> hciEventReceived(const hidl_vec<uint8_t>& event) override {
        if (mCb) {
            std::vector<uint8_t> data(event.begin(), event.end());
            mCb->hciEventReceived(data);
        }
        return Void();
    }

    Return<void> aclDataReceived(const hidl_vec<uint8_t>& data) override {
        if (mCb) {
            std::vector<uint8_t> vec(data.begin(), data.end());
            mCb->aclDataReceived(vec);
        }
        return Void();
    }

    Return<void> scoDataReceived(const hidl_vec<uint8_t>& data) override {
        if (mCb) {
            std::vector<uint8_t> vec(data.begin(), data.end());
            mCb->scoDataReceived(vec);
        }
        return Void();
    }

  private:
    std::shared_ptr<IBluetoothHciCallbacks> mCb;
};

class BluetoothHci : public BnBluetoothHci {
  public:
    BluetoothHci() = default;
    ~BluetoothHci() override = default;

    ::ndk::ScopedAStatus initialize(
        const std::shared_ptr<IBluetoothHciCallbacks>& cb) override;
    ::ndk::ScopedAStatus sendHciCommand(
        const std::vector<uint8_t>& command) override;
    ::ndk::ScopedAStatus sendAclData(
        const std::vector<uint8_t>& data) override;
    ::ndk::ScopedAStatus sendScoData(
        const std::vector<uint8_t>& data) override;
    ::ndk::ScopedAStatus sendIsoData(
        const std::vector<uint8_t>& data) override;
    ::ndk::ScopedAStatus close() override;

  private:
    ::android::sp<::android::hardware::bluetooth::V1_0::IBluetoothHci> mHidlHci;
    ::android::sp<BluetoothHciHidlCallback> mHidlCallback;
};

}  // namespace impl
}  // namespace bluetooth
}  // namespace hardware
}  // namespace android
}  // namespace aidl
