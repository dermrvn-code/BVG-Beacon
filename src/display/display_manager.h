#pragma once

#include <Arduino.h>
#include <PNGdec.h>
#include <TFT_eSPI.h>
#include "display_config.h"
#include "transit_icons.h"
#include "ui_helpers.h"

extern TFT_eSPI display;
extern TFT_eSprite frame;

void InitDisplay();

void displayStartupScreen();

void displayAP(const char *apName);

void displayConfig(const char *ssid, const char *ipAddress);

void displayTrain(uint32_t secondsUntilNextTrain,
				  const char *trainName,
				  uint32_t secondsSinceLastTrain,
				  TransitType transitType);
