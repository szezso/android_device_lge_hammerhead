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

#include <android/binder_manager.h>
#include <android/binder_process.h>

#include "Lights.h"

using ::aidl::android::hardware::light::Lights;

const static std::string kBacklightPath = "/sys/class/leds/lcd-backlight/brightness";

const static std::string kRLedBrightnessPath = "/sys/class/leds/red/brightness";
const static std::string kGLedBrightnessPath = "/sys/class/leds/green/brightness";
const static std::string kBLedBrightnessPath = "/sys/class/leds/blue/brightness";

const static std::string kRLedTimeoutPath = "/sys/class/leds/red/on_off_ms";
const static std::string kGLedTimeoutPath = "/sys/class/leds/green/on_off_ms";
const static std::string kBLedTimeoutPath = "/sys/class/leds/blue/on_off_ms";

const static std::string kRLedLockedPath = "/sys/class/leds/red/rgb_start";
const static std::string kGLedLockedPath = "/sys/class/leds/green/rgb_start";
const static std::string kBLedLockedPath = "/sys/class/leds/blue/rgb_start";

std::shared_ptr<Lights> createLightsService() {
    std::ofstream backlight(kBacklightPath);
    if (!backlight) {
        LOG(ERROR) << "Failed to open " << kBacklightPath << ". Error: " << errno << " - " << strerror(errno);
        return NULL;
    }

    std::ofstream rLedBrightness(kRLedBrightnessPath);
    if (!rLedBrightness) {
        LOG(ERROR) << "Failed to open " << kRLedBrightnessPath << ". Error: " << errno << " - " << strerror(errno);
        return NULL;
    }

    std::ofstream gLedBrightness(kGLedBrightnessPath);
    if (!gLedBrightness) {
        LOG(ERROR) << "Failed to open " << kGLedBrightnessPath << ". Error: " << errno << " - " << strerror(errno);
        return NULL;
    }

    std::ofstream bLedBrightness(kBLedBrightnessPath);
    if (!bLedBrightness) {
        LOG(ERROR) << "Failed to open " << kBLedBrightnessPath << ". Error: " << errno << " - " << strerror(errno);
        return NULL;
    }

    std::ofstream rLedTimeout(kRLedTimeoutPath);
    if (!rLedTimeout) {
        LOG(ERROR) << "Failed to open " << kRLedTimeoutPath << ". Error: " << errno << " - " << strerror(errno);
        return NULL;
    }

    std::ofstream gLedTimeout(kGLedTimeoutPath);
    if (!gLedTimeout) {
        LOG(ERROR) << "Failed to open " << kGLedTimeoutPath << ". Error: " << errno << " - " << strerror(errno);
        return NULL;
    }

    std::ofstream bLedTimeout(kBLedTimeoutPath);
    if (!bLedTimeout) {
        LOG(ERROR) << "Failed to open " << kBLedTimeoutPath << ". Error: " << errno << " - " << strerror(errno);
        return NULL;
    }

    std::ofstream rLedLocked(kRLedLockedPath);
    if (!rLedLocked) {
        LOG(ERROR) << "Failed to open " << kRLedLockedPath << ". Error: " << errno << " - " << strerror(errno);
        return NULL;
    }

    std::ofstream gLedLocked(kGLedLockedPath);
    if (!gLedLocked) {
        LOG(ERROR) << "Failed to open " << kGLedLockedPath << ". Error: " << errno << " - " << strerror(errno);
        return NULL;
    }

    std::ofstream bLedLocked(kBLedLockedPath);
    if (!bLedLocked) {
        LOG(ERROR) << "Failed to open " << kBLedLockedPath << ". Error: " << errno << " - " << strerror(errno);
        return NULL;
    }

    return ndk::SharedRefBase::make<Lights>(std::move(backlight),
                                            std::move(rLedBrightness),
                                            std::move(gLedBrightness),
                                            std::move(bLedBrightness),
                                            std::move(rLedTimeout),
                                            std::move(gLedTimeout),
                                            std::move(bLedTimeout),
                                            std::move(rLedLocked),
                                            std::move(gLedLocked),
                                            std::move(bLedLocked));
}

int main() {
    ABinderProcess_setThreadPoolMaxThreadCount(0);
    std::shared_ptr<Lights> lights = createLightsService();
    CHECK(lights != NULL);

    const std::string instance = std::string() + Lights::descriptor + "/default";
    binder_status_t status = AServiceManager_addService(lights->asBinder().get(), instance.c_str());
    CHECK(status == STATUS_OK);

    ABinderProcess_joinThreadPool();
    return EXIT_FAILURE; // should not reach
}
