/*
 * Copyright (C) 2020-2021 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "android.hardware.light-service.hh"
#include <android-base/logging.h>

#include "Lights.h"

namespace aidl {
namespace android {
namespace hardware {
namespace light {

static const HwLight kAttentionHwLight = {
    .id = (int32_t)LightType::ATTENTION,
    .type = LightType::ATTENTION,
    .ordinal = 0
};

static const HwLight kBacklightHwLight = {
    .id = (int32_t)LightType::BACKLIGHT,
    .type = LightType::BACKLIGHT,
    .ordinal = 0
};

static const HwLight kBatteryHwLight = {
    .id = (int32_t)LightType::BATTERY,
    .type = LightType::BATTERY,
    .ordinal = 0
};

static const HwLight kNotificationHwLight = {
    .id = (int32_t)LightType::NOTIFICATIONS,
    .type = LightType::NOTIFICATIONS,
    .ordinal = 0
};

Lights::Lights(std::ofstream&& backlight,
               std::ofstream&& rLedBrightness,
               std::ofstream&& gLedBrightness,
               std::ofstream&& bLedBrightness,
               std::ofstream&& rLedTimeout,
               std::ofstream&& gLedTimeout,
               std::ofstream&& bLedTimeout,
               std::ofstream&& rLedLocked,
               std::ofstream&& gLedLocked,
               std::ofstream&& bLedLocked) :
               mBacklight(std::move(backlight)),
               mRLedBrightness(std::move(rLedBrightness)),
               mGLedBrightness(std::move(gLedBrightness)),
               mBLedBrightness(std::move(bLedBrightness)),
               mRLedTimeout(std::move(rLedTimeout)),
               mGLedTimeout(std::move(gLedTimeout)),
               mBLedTimeout(std::move(bLedTimeout)),
               mRLedLocked(std::move(rLedLocked)),
               mGLedLocked(std::move(gLedLocked)),
               mBLedLocked(std::move(bLedLocked)) {
}

ndk::ScopedAStatus Lights::setLightState(int32_t id, const HwLightState& state) {
    ndk::ScopedAStatus status = ndk::ScopedAStatus::ok();
    switch ((LightType)id) {
        case LightType::ATTENTION:
            setAttentionLight(state);
            break;
        case LightType::BACKLIGHT:
            setBacklightLight(state);
            break;
        case LightType::BATTERY:
            setBatteryLight(state);
            break;
        case LightType::NOTIFICATIONS:
            setNotificationLight(state);
            break;
        default:
            status = ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
            break;
    }
    return status;
}

ndk::ScopedAStatus Lights::getLights(std::vector<HwLight> *_aidl_return) {
    _aidl_return->push_back(kAttentionHwLight);
    _aidl_return->push_back(kBacklightHwLight);
    _aidl_return->push_back(kBatteryHwLight);
    _aidl_return->push_back(kNotificationHwLight);
    return ndk::ScopedAStatus::ok();
}

void Lights::setNotificationLight(const HwLightState& state) {
    std::lock_guard<std::mutex> lock(mLock);
    mNotificationState = state;
    setPrioritizedLightLocked();
}

void Lights::setAttentionLight(const HwLightState& state) {
    std::lock_guard<std::mutex> lock(mLock);
    mAttentionState = state;
    setPrioritizedLightLocked();
}

void Lights::setBacklightLight(const HwLightState& state) {
    std::lock_guard<std::mutex> lock(mLock);
    uint32_t brightness = rgbToBrightness(state);
    mBacklight << brightness << std::endl;
}

void Lights::setBatteryLight(const HwLightState& state) {
    std::lock_guard<std::mutex> lock(mLock);
    mBatteryState = state;
    setPrioritizedLightLocked();
}

void Lights::setPrioritizedLightLocked() {
    if (isLit(mNotificationState)) {
        setLightStateLocked(mNotificationState);
    } else if (isLit(mAttentionState)) {
        setLightStateLocked(mAttentionState);
    } else if (isLit(mBatteryState)) {
        setLightStateLocked(mBatteryState);
    } else {
        setLightLocked(0, 0, 0);
    }
}

void Lights::setLightStateLocked(const HwLightState& state) {
    int onMS, offMS;
    uint32_t color = state.color & 0x00ffffff;

    switch (state.flashMode) {
        case FlashMode::TIMED:
        case FlashMode::HARDWARE:
            onMS = state.flashOnMs;
            offMS = state.flashOffMs;
            break;
        case FlashMode::NONE:
        default:
            onMS = 0;
            offMS = 0;
            break;
    }

    setLightLocked(color, onMS, offMS);
}

void Lights::setLightLocked(uint32_t color, int onMS, int offMS) {
    uint32_t r = ((color >> 16) & 0xff);
    uint32_t g = ((color >> 8) & 0xff);
    uint32_t b = ((color) & 0xff);

    mRLedLocked << 0 << std::endl;
    mGLedLocked << 0 << std::endl;
    mBLedLocked << 0 << std::endl;

    mRLedBrightness << r << std::endl;
    mGLedBrightness << g << std::endl;
    mBLedBrightness << b << std::endl;

    mRLedTimeout << onMS << " " << offMS << std::endl;
    mGLedTimeout << onMS << " " << offMS << std::endl;
    mBLedTimeout << onMS << " " << offMS << std::endl;

    mRLedLocked << 1 << std::endl;
    mGLedLocked << 1 << std::endl;
    mBLedLocked << 1 << std::endl;
}

bool Lights::isLit(const HwLightState& state) {
    return (state.color & 0x00ffffff);
}

uint32_t Lights::rgbToBrightness(const HwLightState& state) {
    uint32_t color = state.color & 0x00ffffff;
    return ((77 * ((color >> 16) & 0xff)) + (150 * ((color >> 8) & 0xff)) + (29 * (color & 0xff))) >> 8;
}

} // namespace light
} // namespace hardware
} // namespace android
} // namespace aidl
