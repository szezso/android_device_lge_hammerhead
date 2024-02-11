#
# Copyright (C) 2013 The Android Open-Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

BUILD_BROKEN_ELF_PREBUILT_PRODUCT_COPY_FILES := true
BUILD_BROKEN_USES_BUILD_COPY_HEADERS := true

TARGET_CPU_ABI := armeabi-v7a
TARGET_CPU_ABI2 := armeabi
TARGET_ARCH := arm
TARGET_ARCH_VARIANT := armv7-a-neon
TARGET_CPU_VARIANT := generic
TARGET_CPU_VARIANT_RUNTIME := krait

# Binder API version
TARGET_USES_64_BIT_BINDER := true

TARGET_NO_BOOTLOADER := true

BOARD_KERNEL_BASE := 0x00000000
BOARD_KERNEL_PAGESIZE := 2048

BOARD_KERNEL_CMDLINE := console=none androidboot.hardware=hammerhead user_debug=31 msm_watchdog_v2.enable=1 androidboot.bootdevice=msm_sdcc.1
BOARD_KERNEL_CMDLINE += loop.max_part=7
BOARD_MKBOOTIMG_ARGS := --ramdisk_offset 0x02900000 --tags_offset 0x02700000
BOARD_KERNEL_IMAGE_NAME := zImage-dtb

BOARD_USES_ALSA_AUDIO := true

# Apex
OVERRIDE_TARGET_FLATTEN_APEX := true

BOARD_HAVE_BLUETOOTH := true
BOARD_HAVE_BLUETOOTH_BCM := true
BOARD_CUSTOM_BT_CONFIG := device/lge/hammerhead/bluetooth/configs/vnd_hammerhead.txt
BOARD_BLUETOOTH_BDROID_BUILDCFG_INCLUDE_DIR := device/lge/hammerhead/bluetooth/configs

# Filesystem
TARGET_FS_CONFIG_GEN := device/lge/hammerhead/config.fs

# Wifi related defines
WPA_SUPPLICANT_VERSION      := VER_0_8_X
BOARD_WLAN_DEVICE           := bcmdhd
BOARD_WPA_SUPPLICANT_DRIVER := NL80211
BOARD_WPA_SUPPLICANT_PRIVATE_LIB := lib_driver_cmd_$(BOARD_WLAN_DEVICE)
BOARD_HOSTAPD_DRIVER        := NL80211
BOARD_HOSTAPD_PRIVATE_LIB   := lib_driver_cmd_$(BOARD_WLAN_DEVICE)
WIFI_DRIVER_FW_PATH_PARAM   := "/sys/module/bcmdhd/parameters/firmware_path"
WIFI_DRIVER_FW_PATH_AP      := "/vendor/firmware/fw_bcmdhd_apsta.bin"
WIFI_DRIVER_FW_PATH_STA     := "/vendor/firmware/fw_bcmdhd.bin"
WIFI_HIDL_UNIFIED_SUPPLICANT_SERVICE_RC_ENTRY := true

BOARD_USES_SECURE_SERVICES := true

TARGET_NO_RADIOIMAGE := true
TARGET_BOARD_PLATFORM := msm8974
TARGET_BOOTLOADER_BOARD_NAME := hammerhead
TARGET_BOARD_INFO_FILE := device/lge/hammerhead/board-info.txt
TARGET_NO_RPC := true

MAX_VIRTUAL_DISPLAY_DIMENSION := 2048
NUM_FRAMEBUFFER_SURFACE_BUFFERS := 3
VSYNC_EVENT_PHASE_OFFSET_NS := 7500000
SF_VSYNC_EVENT_PHASE_OFFSET_NS := 5000000
TARGET_USES_ION := true
TARGET_ADDITIONAL_GRALLOC_10_USAGE_BITS := 0x2000U | 0x02000000U
TARGET_SCREEN_DENSITY := 480
TARGET_DISABLE_POSTRENDER_CLEANUP := true

# Dex-preopt
ifeq ($(HOST_OS),linux)
  ifneq ($(TARGET_BUILD_VARIANT),eng)
    WITH_DEXPREOPT ?= true
  endif
endif
USE_DEX2OAT_DEBUG := false
DONT_DEXPREOPT_PREBUILTS := true
WITH_DEXPREOPT_DEBUG_INFO := false
WITH_DEXPREOPT_BOOT_IMG_AND_SYSTEM_SERVER_ONLY := true

TARGET_USERIMAGES_USE_EXT4 := true
TARGET_USERIMAGES_USE_F2FS := true
BOARD_BOOTIMAGE_PARTITION_SIZE := 23068672
BOARD_RECOVERYIMAGE_PARTITION_SIZE := 23068672
BOARD_SYSTEMIMAGE_PARTITION_SIZE := 1536000512
BOARD_USERDATAIMAGE_PARTITION_SIZE := 13725837312
BOARD_CACHEIMAGE_PARTITION_SIZE := 271744512
BOARD_CACHEIMAGE_FILE_SYSTEM_TYPE := ext4
BOARD_FLASH_BLOCK_SIZE := 131072

BOARD_ROOT_EXTRA_FOLDERS := firmware persist
BOARD_ROOT_EXTRA_SYMLINKS += /vendor/etc:/vtc
BOARD_ROOT_EXTRA_SYMLINKS += /data/tombstones:/tombstones

# Define kernel config for inline building
TARGET_KERNEL_ADDITIONAL_FLAGS := HOSTCFLAGS="-fuse-ld=lld -Wno-unused-command-line-argument"
TARGET_KERNEL_CLANG_COMPILE := false
TARGET_KERNEL_LLVM_BINUTILS := false
TARGET_KERNEL_NO_GCC := false
TARGET_KERNEL_CONFIG := lineageos_hammerhead_defconfig
TARGET_KERNEL_SOURCE := kernel/lge/hammerhead

TARGET_RECOVERY_FSTAB = device/lge/hammerhead/fstab.hammerhead

TARGET_RELEASETOOLS_EXTENSIONS := device/lge/hammerhead

# QCOM selinux policies
include device/qcom/sepolicy-legacy/sepolicy.mk

BOARD_VENDOR_SEPOLICY_DIRS += device/lge/hammerhead/sepolicy/vendor
SYSTEM_EXT_PRIVATE_SEPOLICY_DIRS += device/lge/hammerhead/sepolicy/private

DEVICE_MANIFEST_FILE := device/lge/hammerhead/manifest.xml
DEVICE_MATRIX_FILE := device/lge/hammerhead/compatibility_matrix.xml

TARGET_FORCE_HWC_FOR_VIRTUAL_DISPLAYS := true
TARGET_HAS_HH_VSYNC_ISSUE := true

TARGET_PROCESS_SDK_VERSION_OVERRIDE := \
    /system/bin/cameraserver=22 \
    /system/bin/mediaserver=22 \
    /system/vendor/bin/mm-qcamera-daemon=22

# Lights
TARGET_PROVIDES_LIBLIGHT := true

ifneq ($(TARGET_BUILD_VARIANT),user)
SELINUX_IGNORE_NEVERALLOWS := true
endif

# Shims
TARGET_LD_SHIM_LIBS := \
    /vendor/lib/libril-qc-qmi-1.so|libshim_ril.so

# SVELTE
MALLOC_SVELTE := true

# VNDK
TARGET_VNDK_USE_CORE_VARIANT := true

# Power
TARGET_USES_INTERACTION_BOOST := true
TARGET_USES_NON_LEGACY_POWERHAL := true

# Legacy memfd
TARGET_HAS_MEMFD_BACKPORT := true

-include vendor/lge/hammerhead/BoardConfigVendor.mk
