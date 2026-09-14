#include <Arduino.h>
#include <PNGdec.h>
#include <TFT_eSPI.h>
#include "display_config.h"
#include "display/ui_helpers.h"
#include "wifi/wifi_setup.h"
#include "web/web_manager.h"
#include "bvg/bvg.h"

#include "display/display_manager.h"

#define RIGHT_BUTTON_PIN 0
#define LEFT_BUTTON_PIN 1

int page = 0;
int lastRenderedPage = -2;

void onApStarted(const char *name)
{
  displayAP(name);
}

void renderPage()
{

  if (page == -2)
  {
    displayApiInaccessable();
  }
  else if (page == -1)
  {
    displayConfig(WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
  }
  else if(BVG::state.lineCount == 0)
  {
    displayNoLines(WiFi.localIP().toString().c_str());
  }
  else if (page >= 0 && page < BVG::state.lineCount)
  {
    BVG::state.activeLineIndex = page;
    displayTrain(BVG::state.currentDeparture);
  }

  lastRenderedPage = page;
}

void setup()
{
  configTime(0, 0, "pool.ntp.org");
  setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 1);
  tzset();

  Serial.begin(115200);
  pinMode(LEFT_BUTTON_PIN, INPUT);
  pinMode(RIGHT_BUTTON_PIN, INPUT);

  InitDisplay();
  displayStartupScreen();

  WifiSetup::begin(onApStarted);
  WebManager::begin();
  BVG::begin();
}

bool lastLeft = LOW;
bool lastRight = LOW;
void handleButtons()
{
  bool left = digitalRead(LEFT_BUTTON_PIN);
  bool right = digitalRead(RIGHT_BUTTON_PIN);

  bool leftPressed = left && !lastLeft;
  bool rightPressed = right && !lastRight;

  if (leftPressed && rightPressed)
  {
    page = -1;
    Serial.println("Both buttons pressed");
  }
  else if (leftPressed)
  {
    page = (page - 1 + BVG::state.lineCount) % BVG::state.lineCount;
    Serial.printf("Left button pressed. New page: %d from %d pages\n", page, BVG::state.lineCount);
  }
  else if (rightPressed)
  {
    page = (page + 1) % BVG::state.lineCount;
    Serial.printf("Right button pressed. New page: %d from %d pages\n", page, BVG::state.lineCount);
  }

  lastLeft = left;
  lastRight = right;
}

void loop()
{
  WebManager::handle();
  handleButtons();

  if (BVG::state.lineCount > 0)
  {
    BVG::loop();
    if (!BVG::state.apiWorking && page != -1)
    {
      page = -2;
    }

    if (BVG::state.apiWorking && page == -2)
    {
      page = 0;
    }
  }

  renderPage();
  delay(100);
}
