#pragma once

#include <Arduino.h>
#include <PNGdec.h>
#include <TFT_eSPI.h>
#include "display_config.h"
#include "ui_helpers.h"
#include "bvg/bvg.h"

extern TFT_eSPI display;
extern TFT_eSprite frame;

void InitDisplay();

void displayStartupScreen();

void displayAP(const char *apName);

void displayConfig(const char *ssid, const char *ipAddress);

void displayNoLines(const char *ipAddress);

void displayTrain(BVG::Departure &departure);

void displayApiInaccessable();
