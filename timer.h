#pragma once
#include <Arduino.h>

void timerInit();

// Call every loop() while PAGE_TIMER is the active page. Advances the
// countdown and renders the current state to the LCD.
void timerTick();

// Wire these to the SELECT and BACK buttons only while PAGE_TIMER is
// the active page -- they're no-ops for every other page.
void timerHandleSelect();
void timerHandleBack();
