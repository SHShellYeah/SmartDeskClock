#include "wifi_manager.h"
#include "config.h"
#include <WiFi.h>

static unsigned long lastAttemptMs = 0;
static unsigned long connectStartMs = 0;
static bool connecting = false;

void wifiManagerInit() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  connecting = true;
  connectStartMs = millis();
}

void wifiManagerTick() {
  if (WiFi.status() == WL_CONNECTED) {
    connecting = false;
    return;
  }

  unsigned long now = millis();

  if (connecting) {
    // Still trying the current attempt -- just wait, don't block.
    if (now - connectStartMs > WIFI_CONNECT_TIMEOUT_MS) {
      connecting = false;
      lastAttemptMs = now;
    }
    return;
  }

  // Not connected and not currently trying -> retry on an interval.
  if (now - lastAttemptMs > WIFI_RETRY_INTERVAL_MS) {
    WiFi.disconnect();
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    connecting = true;
    connectStartMs = now;
  }
}

bool wifiIsConnected() {
  return WiFi.status() == WL_CONNECTED;
}

String wifiGetIP() {
  return wifiIsConnected() ? WiFi.localIP().toString() : String("0.0.0.0");
}

int wifiGetRSSI() {
  return wifiIsConnected() ? WiFi.RSSI() : 0;
}
