#include "display_manager.h"

TFT_eSPI display;
TFT_eSprite frame(&display);

void InitDisplay(){
	display.init();
	display.setRotation(DisplayConfig::ROTATION);
	if (frame.createSprite(display.width(), display.height()) == nullptr) {
		Serial.println("Display sprite allocation failed");
		while (true) {
			delay(1000);
		}
	}
}

void displayStartupScreen()
{
	const int16_t centerX = frame.width() / 2;
	const int16_t centerY = frame.height() / 2;

	frame.fillScreen(TFT_BLACK);
	drawText(frame, "BEACON", centerX, centerY,
						 12,
						 TFT_WHITE, TFT_BLACK);

	drawText(frame, DisplayConfig::SOFTWARE_VERSION, centerX, centerY + 25,
						 9,
						 TFT_LIGHTGREY, TFT_BLACK);

	delay(DisplayConfig::STARTUP_SCREEN_MS);
    
	frame.pushSprite(0, 0);
}

void displayAP(const char *apName){
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

void displayConfig(const char *ssid, const char *ipAddress){
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

void displayTrain(uint32_t secondsUntilNextTrain,
				  const char *trainName,
				  uint32_t secondsSinceLastTrain,
				  TransitType transitType)
{
	const String timeText = String(secondsUntilNextTrain / 60);

	const uint32_t trainIntervalSeconds =
		secondsSinceLastTrain + secondsUntilNextTrain;
	float progress = 0.0f;
	if (trainIntervalSeconds > 0) {
		progress = 1.0f - (static_cast<float>(secondsSinceLastTrain) /
						   trainIntervalSeconds);
		progress = constrain(progress, 0.0f, 1.0f);
	}

	const int16_t centerX = display.width() / 2;
	const int16_t centerY = display.height() / 2;
	const int16_t ringRadius = min(display.width(), display.height()) / 2 -
							 DisplayConfig::RING_MARGIN;

	frame.fillSprite(DisplayConfig::BACKGROUND);
	drawProgressArc(frame, centerX, centerY, ringRadius, progress);

	drawText(frame, timeText.c_str(), centerX,
					 centerY,
					 28,
					 DisplayConfig::TIME_COLOR);
	drawText(frame, trainName, centerX,
					 centerY + 65,
					 12,
					 DisplayConfig::LINE_COLOR);
    

	size_t iconSize = 0;
	const byte *transitIcon = getTransitIcon(transitType, iconSize);
	if (transitIcon != nullptr) {
		drawPng(frame, transitIcon, iconSize, centerX, centerY - 65, 0.5f);
	}
    
	frame.pushSprite(0, 0);
}
