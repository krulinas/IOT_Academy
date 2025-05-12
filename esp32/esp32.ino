#include <WiFi.h>
#include "firebase_utils.h"

const char* ssid = "43BD44-Maxis Fibre";
const char* password = "b9f2kYPzrq";

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");

  initFirebase();
  startFirebaseStream(); 
}

void loop() {
  maintainFirebaseConnection();
}
