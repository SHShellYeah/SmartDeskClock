#pragma once
#include <Arduino.h>

// Starts the initial WiFi connection attempt. Does not block.
void wifiManagerInit();

// Call every loop(). Handles connection timeout and periodic
// reconnect attempts without ever blocking the main loop.
void wifiManagerTick();

bool wifiIsConnected();
String wifiGetIP();
int wifiGetRSSI();
