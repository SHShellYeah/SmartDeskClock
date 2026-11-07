#pragma once
#include <Arduino.h>

// Configures the SNTP client. Safe to call before WiFi connects.
void clockInit();

// Call every loop(). Detects first sync and handles periodic resync.
void clockTick();

bool clockIsSynced();

String clockGetTimeStr();  // "08:42:35"
String clockGetDateStr();  // "Jul 11,26"
String clockGetDayStr();   // "Saturday"

int clockGetDayOfMonth();
int clockGetMonth();
int clockGetYear();
