/*
 * Copyright (C) 2024-2026 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "BiometricsFingerprintAidl.h"

#include <android-base/logging.h>

namespace aidl {
namespace android {
namespace hardware {
namespace biometrics {
namespace fingerprint {

BiometricsFingerprintAidl::BiometricsFingerprintAidl() {
    mHidlHal = IBiometricsFingerprint::getService();
    if (mHidlHal == nullptr) {
        LOG(ERROR) << "Failed to get legacy IBiometricsFingerprint HIDL HAL";
    }
}

::ndk::ScopedAStatus BiometricsFingerprintAidl::getSensorProps(
    std::vector<SensorProps>* out_props) {
    SensorProps props;
    props.commonProps.sensorId = 0;
    props.commonProps.sensorStrength = SensorStrength::STRONG;
    props.commonProps.maxEnrollmentsPerUser = 5;
    props.sensorType = SensorType::REAR;
    out_props->push_back(props);
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus BiometricsFingerprintAidl::createSession(
    int32_t /*sensorId*/, int32_t /*userId*/,
    const std::shared_ptr<ISessionCallback>& cb,
    std::shared_ptr<ISession>* out_session) {
    if (mHidlHal == nullptr) {
        mHidlHal = IBiometricsFingerprint::getService();
    }
    *out_session = ::ndk::SharedRefBase::make<SessionAidl>(mHidlHal, cb);
    return ::ndk::ScopedAStatus::ok();
}

SessionAidl::SessionAidl(sp<IBiometricsFingerprint> hidlHal,
                         const std::shared_ptr<ISessionCallback>& cb)
    : mHidlHal(hidlHal), mCb(cb) {
    if (mHidlHal != nullptr) {
        mHidlCallback = new FingerprintHidlCallback(cb);
        mHidlHal->setNotify(mHidlCallback);
    }
}

::ndk::ScopedAStatus SessionAidl::generateChallenge() {
    if (mHidlHal != nullptr) {
        uint64_t challenge = mHidlHal->preEnroll();
        if (mCb) {
            mCb->onChallengeGenerated(challenge);
        }
    }
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus SessionAidl::revokeChallenge(int64_t challenge) {
    if (mHidlHal != nullptr) {
        mHidlHal->postEnroll();
        if (mCb) {
            mCb->onChallengeRevoked(challenge);
        }
    }
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus SessionAidl::enroll(
    const HardwareAuthToken& /*hat*/,
    std::shared_ptr<ICancellationSignal>* /*out_cancel*/) {
    if (mHidlHal != nullptr) {
        hidl_array<uint8_t, 69> hatVec;
        mHidlHal->enroll(hatVec, 0, 60);
    }
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus SessionAidl::authenticate(
    int64_t operationId,
    std::shared_ptr<ICancellationSignal>* /*out_cancel*/) {
    if (mHidlHal != nullptr) {
        mHidlHal->authenticate(operationId, 0);
    }
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus SessionAidl::detectInteraction(
    std::shared_ptr<ICancellationSignal>* /*out_cancel*/) {
    return ::ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
}

::ndk::ScopedAStatus SessionAidl::enumerateEnrollments() {
    if (mHidlHal != nullptr) {
        mHidlHal->enumerate();
    }
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus SessionAidl::removeEnrollments(
    const std::vector<int32_t>& enrollments) {
    if (mHidlHal != nullptr && !enrollments.empty()) {
        mHidlHal->remove(0, enrollments[0]);
    }
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus SessionAidl::getAuthenticatorId() {
    if (mHidlHal != nullptr) {
        uint64_t id = mHidlHal->getAuthenticatorId();
        if (mCb) {
            mCb->onAuthenticatorIdRetrieved(id);
        }
    }
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus SessionAidl::invalidateAuthenticatorId() {
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus SessionAidl::resetLockout(
    const HardwareAuthToken& /*hat*/) {
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus SessionAidl::close() {
    if (mHidlHal != nullptr) {
        mHidlHal->cancel();
    }
    return ::ndk::ScopedAStatus::ok();
}

}  // namespace fingerprint
}  // namespace biometrics
}  // namespace hardware
}  // namespace android
}  // namespace aidl
