#pragma once

#include <Arduino.h>
#include <FS.h>
#include <LittleFS.h>
#include <WebServer.h>
#include <WiFi.h>
#include <WiFiManager.h>

namespace WebManager {

extern WebServer server;

void handleRoot();

void handleResetWifi();

String getContentType(const String& path);

void handleNotFound();

void begin();

void handle();

} // namespace WebManager
