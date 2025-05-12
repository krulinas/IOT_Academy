#include "display_utils.h"
#include "eeprom_utils.h"
#include "wifi_utils.h"
#include "firebase_utils.h"
#include "web_server.h"

void setup() {
  Serial.begin(115200);

  initDisplay();
  showStartupScreen();

  readCredentialsFromEEPROM();
  bool connected = connectToWiFi();

  if (!connected) {
    startAccessPointMode(); // fallback
  } else {
    updateDisplay("WiFi Connected", "Connecting Firebase...", "");
    initFirebase();
    startFirebaseStream();
  }
}

void loop() {
  handleWebRequests();          // If in AP mode, keep web server running
  maintainFirebaseConnection(); // For token refresh
}
