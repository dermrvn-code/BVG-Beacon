#include "web_manager.h"

namespace WebManager {

WebServer server(80);

void handleRoot()
{
	File page = LittleFS.open("/index.html", "r");
	if (!page) {
		server.send(500, "text/plain", "Configuration page is unavailable");
		return;
	}

	server.streamFile(page, "text/html; charset=utf-8");
	page.close();
}

void handleResetWifi()
{
	server.send(200, "text/html; charset=utf-8",
				"<h1>Wi-Fi settings reset</h1><p>Restarting...</p>");
	delay(500);

	WiFiManager wifiManager;
	wifiManager.resetSettings();
	ESP.restart();
}

String getContentType(const String& path)
{
	if (path.endsWith(".css")) return "text/css";
	if (path.endsWith(".js"))  return "application/javascript";
	return "text/plain";
}

void handleNotFound()
{
	File file = LittleFS.open(server.uri(), "r");

	if (!file) {
		server.send(404, "text/plain", "Not found");
		return;
	}

	server.streamFile(file, getContentType(server.uri()));
	file.close();
}

void begin()
{
	if (!LittleFS.begin(false)) {
		Serial.println("LittleFS mount failed");
	}

	server.on("/", HTTP_GET, handleRoot);
	server.on("/reset-wifi", HTTP_POST, handleResetWifi);
	server.onNotFound(handleNotFound);
	server.begin();

	Serial.print("Configuration page: http://");
	Serial.println(WiFi.localIP());
}

void handle()
{
	server.handleClient();
}

} // namespace WebManager
