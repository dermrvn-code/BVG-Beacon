#include "helpers.h"

WiFiClientSecure client;

HTTPClient http;

bool get(const char* path, JsonDocument& doc)
{
    doc.clear();
    doc.shrinkToFit();

    client.setInsecure();

    if (!http.begin(client, path)) {
        Serial.println("http.begin() FAILED");
        return false;
    }

    http.setConnectTimeout(4000);
    http.setTimeout(6000);

    int code = http.GET();

    if (code != HTTP_CODE_OK) {
        Serial.printf(
            "HTTP GET failed, code: %d, error: %s\n",
            code,
            http.errorToString(code).c_str()
        );

        http.end();
        client.stop();
        return false;
    }

    int contentLength = http.getSize();

    Serial.printf(
        "HTTP content length: %d\n",
        contentLength
    );

    WiFiClient* stream = http.getStreamPtr();

    if (stream == nullptr) {
        Serial.println("HTTP stream is null");
        http.end();
        client.stop();
        return false;
    }

    DeserializationError error =
        deserializeJson(doc, *stream);

    http.end();
    client.stop();

    if (error) {
        Serial.printf(
            "Failed to parse JSON: %s\n",
            error.c_str()
        );
        return false;
    }

    return true;
}


bool urlEncode(
    const char* input,
    char* output,
    size_t outputSize
)
{
    static const char hex[] = "0123456789ABCDEF";

    if (!input || !output || outputSize == 0)
        return false;

    size_t out = 0;

    while (*input) {
        const uint8_t c = static_cast<uint8_t>(*input++);

        if (isalnum(c) ||
            c == '-' ||
            c == '_' ||
            c == '.' ||
            c == '~') {

            if (out + 1 >= outputSize)
                return false;

            output[out++] = static_cast<char>(c);

        } else {

            if (out + 3 >= outputSize)
                return false;

            output[out++] = '%';
            output[out++] = hex[c >> 4];
            output[out++] = hex[c & 0x0F];
        }
    }

    output[out] = '\0';
    return true;
}

int minutesUntil(const char* timestamp)
{
    if (!timestamp)
        return -1;

    int hour, minute;

    if (sscanf(timestamp, "%*d-%*d-%*dT%d:%d", &hour, &minute) != 2)
        return -1;

    time_t now = time(nullptr);
    tm* current = localtime(&now);

    if (!current)
        return -1;

    int target = hour * 60 + minute;
    int currentTime = current->tm_hour * 60 + current->tm_min;

    int diff = target - currentTime;

    if (diff < 0)
        diff += 1440;

    return diff;
}

bool equalsIgnoreCase(const char* a, const char* b)
{
    if (!a || !b)
        return false;

    while (*a && *b) {
        if (tolower((unsigned char)*a) !=
            tolower((unsigned char)*b)) {
            return false;
        }

        ++a;
        ++b;
    }

    return *a == *b;
}
