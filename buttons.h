#pragma once
#include <Arduino.h>

enum ButtonEvent {
  BTN_EVENT_NONE = 0,
  BTN_EVENT_NEXT,
  BTN_EVENT_PREV,
  BTN_EVENT_SELECT,
  BTN_EVENT_BACK
};

void buttonsInit();

// Call every loop(). Returns the event for whichever button was just
// confirmed pressed (post-debounce), or BTN_EVENT_NONE otherwise.
// Fires once per press-down, not on release, and not while held.
ButtonEvent buttonsTick();
