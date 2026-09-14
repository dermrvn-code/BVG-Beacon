#include "display_manager.h"

TFT_eSPI display;
TFT_eSprite frame(&display);

void InitDisplay()
{
	display.init();
	display.setRotation(DisplayConfig::ROTATION);
	if (frame.createSprite(display.width(), display.height()) == nullptr)
	{
		Serial.println("Display sprite allocation failed");
		while (true)
		{
			delay(1000);
		}
	}
}

void displayStartupScreen()
{
	const int16_t centerX = frame.width() / 2;
	const int16_t centerY = frame.height() / 2;

	frame.fillScreen(TFT_YELLOW);

	drawText(frame, "BVG", centerX, centerY - 15,
			 28,
			 TFT_BLACK, TFT_YELLOW);

	drawText(frame, "BEACON", centerX, centerY + 45,
			 12,
			 TFT_BLACK, TFT_YELLOW);

	drawText(frame, DisplayConfig::SOFTWARE_VERSION, centerX, centerY + 75,
			 9,
			 TFT_DARKGREY, TFT_YELLOW);

	frame.pushSprite(0, 0);
}

void displayAP(const char *apName)
{
	const int16_t centerX = frame.width() / 2;
	const int16_t centerY = frame.height() / 2;

	frame.fillScreen(TFT_BLACK);

	drawText(frame, "WIFI-SETUP", centerX, centerY - 65,
			 12,
			 TFT_WHITE, TFT_BLACK);

	drawText(frame, apName, centerX, centerY,
			 12,
			 TFT_YELLOW, TFT_BLACK);

	drawText(frame, "Connect to this AP", centerX, centerY + 45,
			 9,
			 TFT_LIGHTGREY, TFT_BLACK);

	drawText(frame, "and setup new network", centerX, centerY + 65,
			 9,
			 TFT_LIGHTGREY, TFT_BLACK);

	frame.pushSprite(0, 0);
}

void displayConfig(const char *ssid, const char *ipAddress)
{
	const int16_t centerX = frame.width() / 2;
	const int16_t centerY = frame.height() / 2;

	frame.fillScreen(TFT_BLACK);
	drawText(frame, "Wifi status:", centerX, centerY - 60,
			 12,
			 TFT_WHITE, TFT_BLACK);

	drawText(frame, "Connected", centerX, centerY - 35,
			 12,
			 TFT_GREEN, TFT_BLACK);

	drawText(frame, "IP address:", centerX, centerY - 5,
			 12,
			 TFT_WHITE, TFT_BLACK);

	drawText(frame, ipAddress, centerX, centerY + 20,
			 12,
			 TFT_LIGHTGREY, TFT_BLACK);

	drawText(frame, "SSID:", centerX, centerY + 55,
			 12,
			 TFT_WHITE, TFT_BLACK);

	drawText(frame, ssid, centerX, centerY + 75,
			 12,
			 TFT_LIGHTGREY, TFT_BLACK);

	frame.pushSprite(0, 0);
}

void displayNoLines(const char *ipAddress)
{
	const int16_t centerX = frame.width() / 2;
	const int16_t centerY = frame.height() / 2;

	const int16_t ringRadius = min(display.width(), display.height()) / 2 -
							   DisplayConfig::RING_MARGIN;

	frame.fillSprite(DisplayConfig::BACKGROUND);
	frame.drawArc(centerX, centerY, ringRadius,
				  ringRadius - DisplayConfig::RING_WIDTH, 0,
				  360.0f, TFT_WHITE, DisplayConfig::BACKGROUND);

	drawText(frame, "No lines", centerX, centerY - 25,
			 18,
			 TFT_WHITE, TFT_BLACK);

	drawText(frame, "Add lines via", centerX, centerY + 15,
			 9,
			 TFT_LIGHTGREY, TFT_BLACK);
	drawText(frame, ipAddress, centerX, centerY + 35,
			 9,
			 TFT_DARKCYAN, TFT_BLACK);

	frame.pushSprite(0, 0);
}

void displayTrain(BVG::Departure &departure)
{
	if (departure.line == nullptr)
	{
		return;
	}

	double progress = 1.0;
	if (departure.next <= 5)
	{
		progress = (double)departure.next / 5.0;
	}

	const int16_t centerX = display.width() / 2;
	const int16_t centerY = display.height() / 2;
	const int16_t ringRadius = min(display.width(), display.height()) / 2 -
							   DisplayConfig::RING_MARGIN;

	uint32_t progressColor = TFT_WHITE;
	switch (departure.line->type)
	{
	case BVG::LineType::SUBURBAN:
		progressColor = DisplayConfig::PROGRESS_SUBURBAN_COLOR;
		break;
	case BVG::LineType::SUBWAY:
		progressColor = DisplayConfig::PROGRESS_SUBWAY_COLOR;
		break;
	case BVG::LineType::TRAM:
		progressColor = DisplayConfig::PROGRESS_TRAM_COLOR;
		break;
	case BVG::LineType::BUS:
		progressColor = DisplayConfig::PROGRESS_BUS_COLOR;
		break;
	case BVG::LineType::FERRY:
		progressColor = DisplayConfig::PROGRESS_FERRY_COLOR;
		break;
	default:
		progressColor = TFT_WHITE;
		break;
	}

	frame.fillSprite(DisplayConfig::BACKGROUND);
	drawProgressArc(frame, centerX, centerY, ringRadius, progress, progressColor);

	drawText(frame, departure.line->lineName, centerX,
			 centerY - 70,
			 12,
			 TFT_LIGHTGREY);

	char directionName[32];
	strncpy(directionName, departure.line->directionName, sizeof(directionName) - 1);
	directionName[sizeof(directionName) - 1] = '\0';

	shortenText(directionName);

	drawText(frame, directionName,
			 centerX,
			 centerY - 45,
			 8,
			 TFT_DARKGREY);

	if (departure.next < 0)
	{
		drawText(frame, "--", centerX,
				 centerY + 5,
				 28,
				 TFT_WHITE);
	}
	else
	{
		char timeText[4];
		snprintf(timeText, sizeof(timeText), "%d", departure.next);

		drawText(frame, timeText, centerX,
				 centerY + 5,
				 28,
				 TFT_WHITE);
	}

	char stationName[32];
	strncpy(stationName, departure.line->stationName, sizeof(stationName) - 1);
	stationName[sizeof(stationName) - 1] = '\0';

	shortenText(stationName, 15);
	drawText(frame, stationName, centerX,
			 centerY + 65,
			 8,
			 TFT_DARKGREY);

	frame.pushSprite(0, 0);
}

void displayApiInaccessable()
{
	const int16_t centerX = frame.width() / 2;
	const int16_t centerY = frame.height() / 2;

	const int16_t ringRadius = min(display.width(), display.height()) / 2 -
							   DisplayConfig::RING_MARGIN;

	frame.fillSprite(DisplayConfig::BACKGROUND);
	frame.drawArc(centerX, centerY, ringRadius,
				  ringRadius - DisplayConfig::RING_WIDTH, 0,
				  360.0f, TFT_RED, DisplayConfig::BACKGROUND);

	drawText(frame, "ERROR", centerX, centerY - 15,
			 18,
			 TFT_RED, TFT_BLACK);

	drawText(frame, "API Inaccessable", centerX, centerY + 15,
			 9,
			 TFT_LIGHTGREY, TFT_BLACK);

	frame.pushSprite(0, 0);
}
