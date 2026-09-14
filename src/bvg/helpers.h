#pragma once
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

bool get(const char *path, JsonDocument &doc);

bool urlEncode(const char *input, char *output, size_t outputSize);

int minutesUntil(const char *timestamp);

bool equalsIgnoreCase(const char *a, const char *b);
