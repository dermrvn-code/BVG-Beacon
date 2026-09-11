#include "wifi_setup.h"

namespace WifiSetup {

ApStartedCallback apStartedCallback = nullptr;

void handlePortalStarted(WiFiManager *manager)
{
	(void)manager;
	if (apStartedCallback == nullptr) {
		return;
	}

	apStartedCallback(AP_NAME);
}

void begin(ApStartedCallback onApStarted)
{
	apStartedCallback = onApStarted;

	WiFiManager wifiManager;
	wifiManager.setConfigPortalTimeout(PORTAL_TIMEOUT_SECONDS);
	wifiManager.setConnectTimeout(20);
	wifiManager.setCaptivePortalEnable(true);
	wifiManager.setAPCallback(handlePortalStarted);
	std::vector<const char *> menu = {"wifi", "exit"};
	wifiManager.setMenu(menu);

    wifiManager.setCustomHeadElement("<style>button{background-color: #f0D722 }</style>");

	Serial.println("Starting Wi-Fi connection...");

	if (!wifiManager.autoConnect(AP_NAME)) {
		Serial.println("Wi-Fi setup failed, restarting...");
		delay(1000);
		ESP.restart();
	}

	Serial.print("Wi-Fi connected: ");
	Serial.println(WiFi.localIP());
}

}

