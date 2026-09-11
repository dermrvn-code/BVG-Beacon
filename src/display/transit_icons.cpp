#include "transit_icons.h"

const byte *getTransitIcon(TransitType type, size_t &size)
{
	switch (type) {
	case TransitType::SBahn:
		size = sizeof(citytrain);
		return citytrain;
	case TransitType::UBahn:
		size = sizeof(subway);
		return subway;
	case TransitType::Tram:
		size = sizeof(tram);
		return tram;
	case TransitType::Bus:
		size = sizeof(bus);
		return bus;
	case TransitType::Ferry:
		size = sizeof(ferry);
		return ferry;
	}

	size = 0;
	return nullptr;
}
