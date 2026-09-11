#pragma once

#include <Arduino.h>
#include <WiFiManager.h>

namespace WifiSetup {

constexpr char AP_NAME[] = "BVG-Beacon-Setup";
constexpr uint16_t PORTAL_TIMEOUT_SECONDS = 300;
using ApStartedCallback = void (*)(const char *name);

extern ApStartedCallback apStartedCallback;

void handlePortalStarted(WiFiManager *manager);
void begin(ApStartedCallback onApStarted = nullptr);

} // namespace WifiSetup
