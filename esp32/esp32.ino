#include "wifi_utils.h"
#include "firebase_utils.h"
#include "display_utils.h"
#include "eeprom_utils.h"
#include "web_server.h"

unsigned long lastHeartbeat = 0;
const unsigned long heartbeatInterval = 10000; 

void setup() {
  Serial.begin(115200);
  initDisplay();
  readCredentialsFromEEPROM();

  showStartupScreen();

  if (connectToWiFi()) {
    showWiFiSuccessScreen(ssid, devid);
    initFirebase();
    startFirebaseStream();
  } else {
    startAccessPointMode();
    showAPModeScreen();
  }
}

void loop() {
  handleWebRequests(); // Web server for AP mode
  maintainFirebaseConnection(); // Refresh token if needed

  //  Send heartbeat every 10 seconds
  if (millis() - lastHeartbeat >= heartbeatInterval) {
    sendHeartbeat();
    lastHeartbeat = millis();
  }
}
