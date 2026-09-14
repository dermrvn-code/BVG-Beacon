#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <time.h>

#include "helpers.h"

namespace BVG
{

    constexpr int MAX_LINES = 10;

    constexpr char BVG_API_BASE[] = "https://v6.bvg.transport.rest";

    enum LineType
    {
        SUBURBAN,
        SUBWAY,
        TRAM,
        BUS,
        FERRY,
        UNKNOWN
    };

    struct Line
    {
        char stationId[32];
        char stationName[64];
        char directionId[32];
        char directionName[64];
        char lineName[32];
        char lineType[16];
        LineType type;
    };

    struct Departure
    {
        Line *line;

        int next;
        Departure(Line *line, int minutes) : line(line), next(minutes) {}
        Departure() : line(nullptr), next(-1) {}
    };

    struct State
    {
        int lineCount = 0;

        int activeLineIndex = -1;
        Departure currentDeparture;

        bool apiWorking = true;
    };

    extern State state;

    // Configuration

    bool add_line(
        const char *stationId,
        const char *stationName,
        const char *directionId,
        const char *directionName,
        const char *lineName,
        const char *lineType);

    bool remove_line(
        const char *stationId,
        const char *directionId,
        const char *lineName);

    void clear_lines();

    // Runtime
    void begin();
    void loop();

} // namespace BVG
