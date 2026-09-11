#pragma once

#include <Arduino.h>
#include <TFT_eSPI.h>

namespace DisplayConfig {

constexpr char SOFTWARE_VERSION[] = "v1.0";
constexpr uint8_t ROTATION = 0;
constexpr uint32_t STARTUP_SCREEN_MS = 1500;

constexpr int16_t RING_MARGIN = 10;
constexpr int16_t RING_WIDTH = 10;
constexpr int16_t ICON_CENTER_Y_OFFSET = -72;


constexpr uint16_t BACKGROUND = TFT_BLACK;
constexpr uint16_t TRACK_COLOR = TFT_DARKGREY;
constexpr uint16_t PROGRESS_COLOR = TFT_CYAN;
constexpr uint16_t TIME_COLOR = TFT_WHITE;
constexpr uint16_t LINE_COLOR = TFT_LIGHTGREY;

}
