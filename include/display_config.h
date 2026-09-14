#pragma once

#include <Arduino.h>
#include <TFT_eSPI.h>

namespace DisplayConfig
{

    constexpr char SOFTWARE_VERSION[] = "v1.0";
    constexpr uint8_t ROTATION = 0;

    constexpr int16_t RING_MARGIN = 10;
    constexpr int16_t RING_WIDTH = 10;

    constexpr uint16_t BACKGROUND = TFT_BLACK;
    constexpr uint16_t TRACK_COLOR = TFT_DARKGREY;

    constexpr uint16_t PROGRESS_SUBURBAN_COLOR = 0x0469; // #008d4f
    constexpr uint16_t PROGRESS_SUBWAY_COLOR = 0x0231;   // #004f8d
    constexpr uint16_t PROGRESS_TRAM_COLOR = 0xD8E4;     // #d82020
    constexpr uint16_t PROGRESS_BUS_COLOR = 0x9193;      // #993399
    constexpr uint16_t PROGRESS_FERRY_COLOR = 0x0417;    // #0080ba
}
