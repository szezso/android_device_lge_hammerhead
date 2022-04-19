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

#pragma once

#include <aidl/android/hardware/light/BnLights.h>
#include <fstream>
#include <mutex>

namespace aidl {
namespace android {
namespace hardware {
namespace light {

class Lights : public BnLights {
public:
    Lights(std::ofstream&& backlight,
           std::ofstream&& rLedBrightness,
           std::ofstream&& gLedBrightness,
           std::ofstream&& bLedBrightness,
           std::ofstream&& rLedTimeout,
           std::ofstream&& gLedTimeout,
           std::ofstream&& bLedTimeout,
           std::ofstream&& rLedLocked,
           std::ofstream&& gLedLocked,
           std::ofstream&& bLedLocked);

    ndk::ScopedAStatus setLightState(int32_t id, const HwLightState& state) override;
    ndk::ScopedAStatus getLights(std::vector<HwLight> *_aidl_return) override;

private:
    void setNotificationLight(const HwLightState& state);
    void setAttentionLight(const HwLightState& state);
    void setBacklightLight(const HwLightState& state);
    void setBatteryLight(const HwLightState& state);

    void setPrioritizedLightLocked();
    void setLightStateLocked(const HwLightState& state);
    void setLightLocked(uint32_t color, int onMS, int offMS);

    bool isLit(const HwLightState& state);
    uint32_t rgbToBrightness(const HwLightState& state);

    std::ofstream mBacklight;
    std::ofstream mRLedBrightness;
    std::ofstream mGLedBrightness;
    std::ofstream mBLedBrightness;
    std::ofstream mRLedTimeout;
    std::ofstream mGLedTimeout;
    std::ofstream mBLedTimeout;
    std::ofstream mRLedLocked;
    std::ofstream mGLedLocked;
    std::ofstream mBLedLocked;

    HwLightState mNotificationState;
    HwLightState mAttentionState;
    HwLightState mBatteryState;

    std::mutex mLock;
};

} // namespace light
} // namespace hardware
} // namespace android
} // namespace aidl
