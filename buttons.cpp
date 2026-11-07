#include "buttons.h"
#include "config.h"

struct ButtonState {
  uint8_t pin;
  ButtonEvent event;
  bool lastReading;     // most recent raw reading (true = pressed)
  bool stableState;     // debounced/confirmed state
  unsigned long lastChangeMs;
};

static ButtonState buttons[4] = {
  { BTN_NEXT_PIN,   BTN_EVENT_NEXT,   false, false, 0 },
  { BTN_PREV_PIN,   BTN_EVENT_PREV,   false, false, 0 },
  { BTN_SELECT_PIN, BTN_EVENT_SELECT, false, false, 0 },
  { BTN_BACK_PIN,   BTN_EVENT_BACK,   false, false, 0 },
};

void buttonsInit() {
  for (uint8_t i = 0; i < 4; i++) {
    pinMode(buttons[i].pin, INPUT_PULLUP);
  }
}

ButtonEvent buttonsTick() {
  unsigned long now = millis();
  ButtonEvent fired = BTN_EVENT_NONE;

  for (uint8_t i = 0; i < 4; i++) {
    bool reading = (digitalRead(buttons[i].pin) == LOW); // active LOW: pressed pulls to GND

    if (reading != buttons[i].lastReading) {
      buttons[i].lastChangeMs = now;
      buttons[i].lastReading = reading;
    }

    if (now - buttons[i].lastChangeMs > BUTTON_DEBOUNCE_MS &&
        reading != buttons[i].stableState) {
      buttons[i].stableState = reading;
      if (reading) {
        // Just confirmed pressed -- fire once. If two buttons are
        // pressed in the exact same loop() tick only one wins; fine
        // for this UI since presses aren't expected simultaneously.
        fired = buttons[i].event;
      }
    }
  }

  return fired;
}
