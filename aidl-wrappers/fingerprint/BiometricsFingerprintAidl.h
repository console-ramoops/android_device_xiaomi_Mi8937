/*
 * Copyright (C) 2024-2026 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/android/hardware/biometrics/fingerprint/BnFingerprint.h>
#include <aidl/android/hardware/biometrics/fingerprint/BnSession.h>
#include <aidl/android/hardware/biometrics/fingerprint/ISessionCallback.h>
#include <android/hardware/biometrics/fingerprint/2.1/IBiometricsFingerprint.h>
#include <android/hardware/biometrics/fingerprint/2.1/IBiometricsFingerprintClientCallback.h>
#include <hidl/HidlSupport.h>

namespace aidl {
namespace android {
namespace hardware {
namespace biometrics {
namespace fingerprint {

using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::hardware::hidl_vec;
using ::android::sp;
using ::android::hardware::biometrics::fingerprint::V2_1::IBiometricsFingerprint;
using ::android::hardware::biometrics::fingerprint::V2_1::IBiometricsFingerprintClientCallback;
using ::android::hardware::biometrics::fingerprint::V2_1::RequestStatus;

class FingerprintHidlCallback : public IBiometricsFingerprintClientCallback {
  public:
    FingerprintHidlCallback(const std::shared_ptr<ISessionCallback>& cb)
        : mCb(cb) {}

    Return<void> onEnrollResult(uint64_t /*deviceId*/, uint32_t fingerId,
                                uint32_t groupId, uint32_t remaining) override {
        if (mCb) {
            mCb->onEnrollmentProgress(fingerId, remaining);
        }
        return Void();
    }

    Return<void> onAcquired(uint64_t /*deviceId*/,
                            ::android::hardware::biometrics::fingerprint::V2_1::FingerprintAcquiredInfo acquiredInfo,
                            int32_t vendorCode) override {
        if (mCb) {
            mCb->onAcquired(static_cast<AcquiredInfo>(acquiredInfo), vendorCode);
        }
        return Void();
    }

    Return<void> onAuthenticated(uint64_t /*deviceId*/, uint32_t fingerId,
                                  uint32_t groupId, const hidl_vec<uint8_t>& token) override {
        if (mCb) {
            if (fingerId != 0) {
                HardwareAuthToken hat;
                mCb->onAuthenticationSucceeded(fingerId, hat);
            } else {
                mCb->onAuthenticationFailed();
            }
        }
        return Void();
    }

    Return<void> onError(uint64_t /*deviceId*/,
                         ::android::hardware::biometrics::fingerprint::V2_1::FingerprintError error,
                         int32_t vendorCode) override {
        if (mCb) {
            mCb->onError(static_cast<Error>(error), vendorCode);
        }
        return Void();
    }

    Return<void> onRemoved(uint64_t /*deviceId*/, uint32_t fingerId,
                           uint32_t groupId, uint32_t remaining) override {
        if (mCb) {
            std::vector<int32_t> enrollments = {static_cast<int32_t>(fingerId)};
            mCb->onEnrollmentsRemoved(enrollments);
        }
        return Void();
    }

    Return<void> onEnumerate(uint64_t /*deviceId*/, uint32_t fingerId,
                             uint32_t groupId, uint32_t remaining) override {
        if (mCb) {
            std::vector<int32_t> enrollments = {static_cast<int32_t>(fingerId)};
            mCb->onEnrollmentsEnumerated(enrollments);
        }
        return Void();
    }

  private:
    std::shared_ptr<ISessionCallback> mCb;
};

class BiometricsFingerprintAidl : public BnFingerprint {
  public:
    BiometricsFingerprintAidl();
    ~BiometricsFingerprintAidl() override = default;

    ::ndk::ScopedAStatus getSensorProps(
        std::vector<SensorProps>* out_props) override;
    ::ndk::ScopedAStatus createSession(
        int32_t sensorId, int32_t userId,
        const std::shared_ptr<ISessionCallback>& cb,
        std::shared_ptr<ISession>* out_session) override;

  private:
    sp<IBiometricsFingerprint> mHidlHal;
};

class SessionAidl : public BnSession {
  public:
    SessionAidl(sp<IBiometricsFingerprint> hidlHal,
                const std::shared_ptr<ISessionCallback>& cb);
    ~SessionAidl() override = default;

    ::ndk::ScopedAStatus generateChallenge() override;
    ::ndk::ScopedAStatus revokeChallenge(int64_t challenge) override;
    ::ndk::ScopedAStatus enroll(
        const HardwareAuthToken& hat,
        std::shared_ptr<ICancellationSignal>* out_cancel) override;
    ::ndk::ScopedAStatus authenticate(
        int64_t operationId,
        std::shared_ptr<ICancellationSignal>* out_cancel) override;
    ::ndk::ScopedAStatus detectInteraction(
        std::shared_ptr<ICancellationSignal>* out_cancel) override;
    ::ndk::ScopedAStatus enumerateEnrollments() override;
    ::ndk::ScopedAStatus removeEnrollments(
        const std::vector<int32_t>& enrollments) override;
    ::ndk::ScopedAStatus getAuthenticatorId() override;
    ::ndk::ScopedAStatus invalidateAuthenticatorId() override;
    ::ndk::ScopedAStatus resetLockout(
        const HardwareAuthToken& hat) override;
    ::ndk::ScopedAStatus close() override;

  private:
    sp<IBiometricsFingerprint> mHidlHal;
    sp<FingerprintHidlCallback> mHidlCallback;
    std::shared_ptr<ISessionCallback> mCb;
};

}  // namespace fingerprint
}  // namespace biometrics
}  // namespace hardware
}  // namespace android
}  // namespace aidl
