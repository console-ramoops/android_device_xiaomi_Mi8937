#
# Copyright (C) 2021 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#

# Inherit from those products. Most specific first.
$(call inherit-product, $(SRC_TARGET_DIR)/product/core_64_bit.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base_telephony.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/product_launched_with_n_mr1.mk)

# Inherit some common LineageOS stuff.
TARGET_DISABLE_EPPE := true
$(call inherit-product, vendor/lineage/config/common_full_phone.mk)

# Maintainer
AXION_MAINTAINER := alpha269

# Blur effect
TARGET_ENABLE_BLUR := false

# AxionFX
TARGET_INCLUDE_AXFX := false

# Prebuilt LineageOS Apps
TARGET_INCLUDES_LOS_PREBUILTS := true
# Kernel
TARGET_KERNEL_VERSION := 4.19

# safely disable libperfmgr without breaking boot (set to true)
TARGET_DISABLES_LIBPERF := false

# Charging
BYPASS_CHARGE_SUPPORTED := false

# CPU governor support
PERF_GOV_SUPPORTED := true
PERF_DEFAULT_GOV := schedutil
PERF_ANIM_OVERRIDE := false

# High Brightness Mode (HBM)
HBM_SUPPORTED := false

# doze flags
# for devices with doze/sensor related issues
TARGET_NEEDS_DOZE_FIX := false
# doze gestures
TARGET_DOZE_TAP_PULSE_SUPPORTED := false
TARGET_DOZE_DOUBLE_TAP_PULSE_SUPPORTED := true
TARGET_DOZE_PICKUP_PULSE_SUPPORTED := true
TARGET_DOZE_SIDE_FPS_PULSE_SUPPORTED := false

# do not enable TARGET_IS_LOW_RAM if your device ram is greater than 4gb
# else OOM will most likely occur on operations where applications and camera can fill heap limit
# e.g uploading video/media on apps with camera preview
# this is a mitigation targets legacy devices 4gb below
# Using quicken is a trade-off: here we trade clean pages for dirty pages,
# extra cpu and battery. That's because the quicken files will be jit-ed in all
# the processes that load of shared apk and the code cache is not shared.
# Some notable apps that will be affected by this are gms and chrome.
TARGET_IS_LOW_RAM := true

# refresh rate list (optional) - defining this flags makes the controller skip the parsing of Display.Mode refresh rates
# for supported refresh rates. The refresh rate controller will also assume that all the defined refresh rates are "supported"
TARGET_SUPPORTED_REFRESH_RATES := 60

# Inherit from Mi8937 device
$(call inherit-product, device/xiaomi/Mi8937/device.mk)

# Overlays
DEVICE_PACKAGE_OVERLAYS += \
    $(LOCAL_PATH)/overlay-lineage

PRODUCT_PACKAGES += \
    xiaomi_prada_overlay_lineage \
    xiaomi_riva_overlay_lineage \
    xiaomi_rolex_overlay_lineage \
    xiaomi_ulysse_overlay_lineage \
    xiaomi_wt8937_overlay_lineage

# Device identifier. This must come after all inclusions
PRODUCT_DEVICE := Mi8937_4_19
PRODUCT_NAME := lineage_Mi8937_4_19
BOARD_VENDOR := Xiaomi
PRODUCT_BRAND := Xiaomi
PRODUCT_MODEL := MSM8937
PRODUCT_MANUFACTURER := Xiaomi
TARGET_VENDOR := Xiaomi

PRODUCT_GMS_CLIENTID_BASE := android-xiaomi

PRODUCT_BUILD_PROP_OVERRIDES += \
    BuildDesc="land-user 6.0.1 MMB29M V10.2.2.0.MALMIXM release-keys"
    BuildFingerprint=Xiaomi/land/land:6.0.1/MMB29M/V10.2.2.0.MALMIXM:user/release-keys
