#pragma once

#include <Arduino.h>
#include "icons.h"

enum class TransitType : uint8_t {
	Suburban,
	Subway,
	Tram,
	Bus,
	Ferry
};

const byte *getTransitIcon(TransitType type, size_t &size);
