#pragma once

#include <Arduino.h>
#include "icons.h"

enum class TransitType : uint8_t {
	SBahn,
	UBahn,
	Tram,
	Bus,
	Ferry
};

const byte *getTransitIcon(TransitType type, size_t &size);
