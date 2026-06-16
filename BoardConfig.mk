#
# Copyright (C) 2021 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#

# Kernel
TARGET_USES_MITHORIUM_KERNEL := true

# Partitions
BOARD_VENDORIMAGE_FILE_SYSTEM_TYPE := ext4
BOARD_VENDORIMAGE_PARTITION_SIZE := 536870912

# Inherit from common mithorium-common
include device/xiaomi/mithorium-common/BoardConfigCommon.mk

DEVICE_PATH := device/xiaomi/Mi8937
USES_DEVICE_XIAOMI_MI8937 := true

# Asserts
TARGET_BOARD_INFO_FILE := $(DEVICE_PATH)/board-info.txt
TARGET_OTA_ASSERT_DEVICE := mi8937,land,santoni,prada,ulysse,ugglite,ugg,rolex,riva,Mi8937,Mi8937_4_19

# Camera
#MI8937_CAM_USE_LATEST_CAMERA_STACK := true
ifeq ($(TARGET_KERNEL_VERSION),4.19)
TARGET_SUPPORT_HAL1 := false
endif

# Display
TARGET_SCREEN_DENSITY := 280

# Filesystem
TARGET_FS_CONFIG_GEN += $(DEVICE_PATH)/config.fs

# HIDL
DEVICE_MANIFEST_FILE += $(DEVICE_PATH)/manifest.xml

# Init
TARGET_INIT_VENDOR_LIB := //$(DEVICE_PATH):init_xiaomi_mi8937

# Kernel
ifeq ($(TARGET_KERNEL_VERSION),4.19)
TARGET_KERNEL_CONFIG += \
    vendor/msm8937-legacy.config
endif
TARGET_KERNEL_CONFIG += \
    vendor/xiaomi/msm8937/common.config \
    vendor/xiaomi/msm8937/mi8937.config

ifeq ($(MI8937_CAM_USE_LATEST_CAMERA_STACK),true)
TARGET_KERNEL_CONFIG += vendor/xiaomi/msm8937/optional/latest-camera-stack.config
endif

ifeq ($(TARGET_KERNEL_VERSION),4.19)
TARGET_KERNEL_RECOVERY_CONFIG += \
    vendor/msm8937-legacy.config
endif
TARGET_KERNEL_RECOVERY_CONFIG += \
    vendor/xiaomi/msm8937/common.config \
    vendor/xiaomi/msm8937/mi8937.config

# Partitions
BOARD_USES_METADATA_PARTITION := true

BOARD_BOOTIMAGE_PARTITION_SIZE := 67108864
BOARD_RECOVERYIMAGE_PARTITION_SIZE := 67108864
BOARD_CACHEIMAGE_FILE_SYSTEM_TYPE := ext4
BOARD_CACHEIMAGE_PARTITION_SIZE := 268435456
BOARD_SYSTEMIMAGE_PARTITION_SIZE := 3221225472
BOARD_USERDATAIMAGE_PARTITION_SIZE := 10332634112 # 10332650496 - 16384


# Power
TARGET_TAP_TO_WAKE_NODE := "/proc/sys/dev/xiaomi_msm8937_touchscreen/enable_dt2w"

# Properties
TARGET_VENDOR_PROP += $(DEVICE_PATH)/vendor.prop

# Recovery
ifeq ($(TARGET_KERNEL_VERSION),4.19)
TARGET_RECOVERY_FSTAB := $(DEVICE_PATH)/rootdir/etc/fstab_4_19.qcom
else
TARGET_RECOVERY_FSTAB := $(DEVICE_PATH)/rootdir/etc/fstab_4_9.qcom
endif

# Rootdir
SOONG_CONFIG_NAMESPACES += XIAOMI_MI8937_ROOTDIR
SOONG_CONFIG_XIAOMI_MI8937_ROOTDIR := KERNEL_VERSION
ifeq ($(TARGET_KERNEL_VERSION),4.19)
SOONG_CONFIG_XIAOMI_MI8937_ROOTDIR_KERNEL_VERSION := k4_19
else
SOONG_CONFIG_XIAOMI_MI8937_ROOTDIR_KERNEL_VERSION := k4_9
endif

# Security patch level
VENDOR_SECURITY_PATCH := 2017-04-01

# SELinux
BOARD_ODM_SEPOLICY_DIRS += $(DEVICE_PATH)/sepolicy/odm
BOARD_VENDOR_SEPOLICY_DIRS += $(DEVICE_PATH)/sepolicy/vendor
BOARD_ODM_SEPOLICY_DIRS += $(DEVICE_PATH)/biometrics/sepolicy-odm
BOARD_VENDOR_SEPOLICY_DIRS += $(DEVICE_PATH)/biometrics/sepolicy

# Inherit from the proprietary version
include vendor/xiaomi/Mi8937/BoardConfigVendor.mk
