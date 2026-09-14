#include "bvg.h"

namespace BVG
{

    State state;

    static Line lines[MAX_LINES];

    static unsigned long lastUpdate = 0;
    static const unsigned long UPDATE_INTERVAL_SECONDS = 10;

    JsonDocument json;

    // --------------------------------------------------
    // Configuration
    // --------------------------------------------------

    bool add_line(
        const char *stationId,
        const char *stationName,
        const char *directionId,
        const char *directionName,
        const char *lineName,
        const char *lineType)
    {
        if (state.lineCount >= MAX_LINES)
            return false;

        Line &line = lines[state.lineCount];

        snprintf(line.stationId, sizeof(line.stationId), "%s", stationId);
        snprintf(line.stationName, sizeof(line.stationName), "%s", stationName);
        snprintf(line.directionId, sizeof(line.directionId), "%s", directionId);
        snprintf(line.directionName, sizeof(line.directionName), "%s", directionName);
        snprintf(line.lineName, sizeof(line.lineName), "%s", lineName);

        if (strcmp(lineType, "suburban") == 0)
        {
            line.type = SUBURBAN;
        }
        else if (strcmp(lineType, "subway") == 0)
        {
            line.type = SUBWAY;
        }
        else if (strcmp(lineType, "tram") == 0)
        {
            line.type = TRAM;
        }
        else if (strcmp(lineType, "bus") == 0)
        {
            line.type = BUS;
        }
        else if (strcmp(lineType, "ferry") == 0)
        {
            line.type = FERRY;
        }
        else
        {
            line.type = UNKNOWN;
        }

        state.lineCount++;

        return true;
    }

    bool remove_line(
        const char *stationId,
        const char *directionId,
        const char *lineName)
    {
        for (int i = 0; i < state.lineCount; i++)
        {
            if (
                strcmp(lines[i].stationId, stationId) == 0 &&
                strcmp(lines[i].directionId, directionId) == 0 &&
                strcmp(lines[i].lineName, lineName) == 0)
            {
                for (int j = i; j < state.lineCount - 1; j++)
                {
                    lines[j] = lines[j + 1];
                }

                state.lineCount--;
                return true;
            }
        }

        return false;
    }

    void clear_lines()
    {
        state.lineCount = 0;
    }

    bool fetch_next_depature(Line *line, Departure &depature)
    {
        int maxResults = 5;

        char stationIdEncoded[128];
        char directionIdEncoded[64];

        urlEncode(
            line->stationId,
            stationIdEncoded,
            sizeof(stationIdEncoded));

        urlEncode(
            line->directionId,
            directionIdEncoded,
            sizeof(directionIdEncoded));

        char url[512];
        snprintf(
            url,
            sizeof(url),
            "%s/stops/%s/departures"
            "?direction=%s"
            "&results=5"
            "&linesOfStops=false"
            "&remarks=false"
            "&language=en"
            "&suburban=%s"
            "&subway=%s"
            "&tram=%s"
            "&bus=%s"
            "&ferry=%s"
            "&express=false"
            "&regional=false",
            BVG_API_BASE,
            stationIdEncoded,
            directionIdEncoded,
            line->type == LineType::SUBURBAN ? "true" : "false",
            line->type == LineType::SUBWAY ? "true" : "false",
            line->type == LineType::TRAM ? "true" : "false",
            line->type == LineType::BUS ? "true" : "false",
            line->type == LineType::FERRY ? "true" : "false");

        Serial.printf("Fetching BVG data from: %s\n", url);

        if (!get(url, json))
        {
            Serial.printf("BVG request failed: %s\n", url);
            return false;
        }

        JsonArray departures = json["departures"];

        if (departures.isNull())
        {
            depature = Departure(line, -1);
            return true;
        }

        for (JsonObject departure : departures)
        {

            const char *lineName =
                departure["line"]["name"] | "";

            if (!lineName ||
                !equalsIgnoreCase(lineName, line->lineName))
            {
                continue;
            }

            int minutes =
                minutesUntil(departure["when"] | "");

            if (minutes < 1)
            {
                continue;
            }

            depature = Departure(line, minutes);
            return true;
        }

        depature = Departure(line, -1);
        return true;
    }

    // --------------------------------------------------
    // Update
    // --------------------------------------------------

    unsigned int failedFetchCount = 0;

    unsigned int pendingNext = 0;
    bool hasPendingNext = false;

    const unsigned int MAX_DEPARTURE_JUMP = 30;

    void update()
    {
        if (!state.lineCount ||
            state.activeLineIndex < 0 ||
            state.activeLineIndex >= state.lineCount)
            return;

        Departure next;

        Serial.printf("Fetching departure for line: %s with index %d\n", lines[state.activeLineIndex].lineName, state.activeLineIndex);
        if (!fetch_next_depature(&lines[state.activeLineIndex], next))
        {
            if (++failedFetchCount >= 3)
            {
                state.apiWorking = false;
            }
            return;
        }

        failedFetchCount = 0;
        state.apiWorking = true;

        int current = state.currentDeparture.next;

        // First value -> accept
        if (current == -1 && !hasPendingNext)
        {
            state.currentDeparture = next;
            return;
        }

        // Check jump, including transitions to/from -1
        unsigned int diff = abs(next.next - current);

        if (diff <= MAX_DEPARTURE_JUMP)
        {
            state.currentDeparture = next;
            hasPendingNext = false;
            return;
        }

        // Suspicious value -> require confirmation
        if (hasPendingNext && pendingNext == next.next)
        {
            state.currentDeparture = next;
            hasPendingNext = false;
            return;
        }

        pendingNext = next.next;
        hasPendingNext = true;
    }

    // --------------------------------------------------
    // Lifecycle
    // --------------------------------------------------

    void begin()
    {
        state = State();
        lastUpdate = 0;
    }

    int currentLineIndex = 0;
    bool updateInstantly = false;
    void loop()
    {
        if (state.lineCount == 0 || state.activeLineIndex < 0 || state.activeLineIndex >= state.lineCount)
            return;

        if (state.activeLineIndex != currentLineIndex)
        {
            currentLineIndex = state.activeLineIndex;
            updateInstantly = true;
        }

        unsigned long now = millis();

        if (lastUpdate == 0 ||
            now - lastUpdate >= UPDATE_INTERVAL_SECONDS * 1000 ||
            updateInstantly)
        {
            lastUpdate = now;
            updateInstantly = false;
            update();
        }
    }

} // namespace BVG
