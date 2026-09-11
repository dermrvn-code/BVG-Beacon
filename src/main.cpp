#include <Arduino.h>
#include <PNGdec.h>
#include <TFT_eSPI.h>
#include "display_config.h"
#include "display/transit_icons.h"
#include "display/ui_helpers.h"
#include "wifi/wifi_setup.h"
#include "web/web_manager.h"

#include "display/display_manager.h"

#define RIGHT_BUTTON_PIN 0
#define LEFT_BUTTON_PIN 1

int page = 0;
int maxPages = 5;
int lastRenderedPage = -2;

void onApStarted(const char *name)
{
  displayAP(name);
}

void renderPage()
{
  if (page == lastRenderedPage) {
    return;
  }

  switch (page) {
  case -1:
    displayConfig(WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
    break;
  case 0:
    displayTrain(120, "U2", 30, TransitType::UBahn);
    break;
  case 1:
    displayTrain(300, "S5", 60, TransitType::Suburban);
    break;
  case 2:
    displayTrain(90, "M4", 15, TransitType::Tram);
    break;
  case 3:
    displayTrain(600, "Bus 100", 120, TransitType::Bus);
    break;
  case 4:
    displayTrain(180, "Ferry 1", 60, TransitType::Ferry);
    break;
  default:
    return;
  }

  lastRenderedPage = page;
}

void setup()
{
  Serial.begin(115200);
  pinMode(LEFT_BUTTON_PIN, INPUT);
  pinMode(RIGHT_BUTTON_PIN, INPUT);

  InitDisplay();
	displayStartupScreen();
  
  WifiSetup::begin(onApStarted);
  WebManager::begin();
}

void loop()
{
  WebManager::handle();

	renderPage();

  if (digitalRead(LEFT_BUTTON_PIN) == HIGH && digitalRead(RIGHT_BUTTON_PIN) == HIGH) {
    Serial.println("Both buttons pressed");
    page = -1;
    delay(200);
  }else if (digitalRead(LEFT_BUTTON_PIN) == HIGH) {
    Serial.println("Left button pressed");
    page = (page - 1 + maxPages) % maxPages;
    delay(200);
  } else if (digitalRead(RIGHT_BUTTON_PIN) == HIGH) {
    Serial.println("Right button pressed");
    page = (page + 1) % maxPages;
    delay(200);
  }
}
