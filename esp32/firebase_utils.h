#ifndef FIREBASE_UTILS_H
#define FIREBASE_UTILS_H

#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#include "display_utils.h"

#define FIREBASE_HOST "https://espressif-18857-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define API_KEY "AIzaSyA5P8jeIX-EPyuEE4nLZCy3UspKhNxe9S8"
#define USER_EMAIL "testuser@example.com"
#define USER_PASSWORD "TestUser@123"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
bool firebaseReady = false;

void initFirebase() {
  config.api_key = API_KEY;
  config.database_url = FIREBASE_HOST;

  configTime(28800, 0, "pool.ntp.org", "time.nist.gov");  // Malaysia time sync

  Firebase.begin(&config, nullptr);  // No authentication
  Firebase.reconnectWiFi(true);

  Serial.println("\nFirebase Initialized (No Auth)");
  firebaseReady = true;
}

void streamCallback(FirebaseStream data) {
  if (data.dataType() == "string") {
    String message = data.stringData();
    String timestamp = "-";

    Firebase.RTDB.getString(&fbdo, "/texts/last_updated", &timestamp);

    // Display on OLED
    updateDisplay("Device ID: " + devid, message, "At: " + timestamp);

    Serial.println("New Data: " + message);
  }
}

void streamTimeoutCallback(bool timeout) {
  if (timeout) {
    Serial.println("[Stream] Timeout");
  }
}

void startFirebaseStream() {
  Serial.println("Starting Firebase stream...");

  if (!Firebase.RTDB.beginStream(&fbdo, "/texts/sample_text")) {
    Serial.println("Stream failed: " + fbdo.errorReason());  // Add this!
    return;
  }

  Firebase.RTDB.setStreamCallback(&fbdo, streamCallback, streamTimeoutCallback);
  Serial.println("Stream started successfully!");
}

void maintainFirebaseConnection() {
  if (!Firebase.ready() && Firebase.isTokenExpired()) {
    Serial.println("Refreshing Firebase token...");
    Firebase.begin(&config, &auth);
  }
}

#endif
