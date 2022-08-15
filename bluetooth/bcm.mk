#
# Copyright (C) 2020-2021 The LineageOS Project
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

# AptX
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/codecs/aptx/lib/libaptX_encoder.so:$(TARGET_COPY_OUT_VENDOR)/lib/libaptX_encoder.so \
    $(LOCAL_PATH)/codecs/aptx/lib/libaptXHD_encoder.so:$(TARGET_COPY_OUT_VENDOR)/lib/libaptXHD_encoder.so

# Firmware
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/firmware/BCM43xx.hcd:$(TARGET_COPY_OUT_VENDOR)/firmware/bcm4335c0.hcd

# MAC loader
PRODUCT_PACKAGES += \
    bluetooth_loader

# Properties
PRODUCT_PROPERTY_OVERRIDES += \
    bluetooth.chip.vendor=brcm \
    persist.service.bdroid.a2dp_con=0 \
    persist.service.bdroid.scms_t=0

PRODUCT_PROPERTY_OVERRIDES += \
    ro.bluetooth.a2dp_offload.supported=false

PRODUCT_PROPERTY_OVERRIDES += \
    persist.bt.enableAptXHD=true \
    persist.service.btui.use_aptx=1 \
    persist.vendor.bt.a2dp_offload_cap=sbc-aptx-aptxtws-aptxhd-aac-ldac \
    persist.vendor.btstack.a2dp_offload_cap=sbc-aptx-aptxtws-aptxhd-aac-ldac
