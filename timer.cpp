#include "timer.h"
#include "display.h"

enum TimerInternalState { TIMER_SET, TIMER_RUNNING, TIMER_PAUSED, TIMER_ALARM };

static const int presetMinutes[] = { 5, 10, 15, 20, 25, 30, 45, 60 };
static const int presetCount = 8;
static int presetIndex = 0;

static TimerInternalState state = TIMER_SET;
static unsigned long targetMillis = 0;   // absolute millis() when countdown hits zero (RUNNING)
static unsigned long remainingMs = 0;    // snapshot of time left (PAUSED)
static unsigned long lastFlashToggleMs = 0;
static bool flashOn = true;

void timerInit() {
  state = TIMER_SET;
  presetIndex = 0;
}

static unsigned long currentDurationMs() {
  return (unsigned long)presetMinutes[presetIndex] * 60000UL;
}

void timerHandleSelect() {
  unsigned long now = millis();
  switch (state) {
    case TIMER_SET:
      targetMillis = now + currentDurationMs();
      state = TIMER_RUNNING;
      break;
    case TIMER_RUNNING:
      remainingMs = (targetMillis > now) ? (targetMillis - now) : 0;
      state = TIMER_PAUSED;
      break;
    case TIMER_PAUSED:
      targetMillis = now + remainingMs;
      state = TIMER_RUNNING;
      break;
    case TIMER_ALARM:
      state = TIMER_SET; // dismiss
      break;
  }
}

void timerHandleBack() {
  switch (state) {
    case TIMER_SET:
      presetIndex = (presetIndex + 1) % presetCount; // cycle through durations
      break;
    case TIMER_RUNNING:
    case TIMER_PAUSED:
      state = TIMER_SET; // cancel back to idle
      break;
    case TIMER_ALARM:
      state = TIMER_SET; // dismiss
      break;
  }
}

static String formatMMSS(unsigned long ms) {
  unsigned long totalSec = ms / 1000;
  unsigned int mm = totalSec / 60;
  unsigned int ss = totalSec % 60;
  char buf[6];
  snprintf(buf, sizeof(buf), "%02u:%02u", mm, ss);
  return String(buf);
}

void timerTick() {
  unsigned long now = millis();

  if (state == TIMER_RUNNING && (long)(targetMillis - now) <= 0) {
    state = TIMER_ALARM;
    lastFlashToggleMs = now;
    flashOn = true;
  }

  displayPrintLine(0, "TIMER");

  switch (state) {
    case TIMER_SET:
      displayPrintLine(1, "Set: " + String(presetMinutes[presetIndex]) + " min");
      displayPrintLine(2, formatMMSS(currentDurationMs()));
      displayPrintLine(3, "SEL start  BACK +dur");
      break;

    case TIMER_RUNNING: {
      unsigned long remaining = (targetMillis > now) ? (targetMillis - now) : 0;
      displayPrintLine(1, "Running");
      displayPrintLine(2, formatMMSS(remaining));
      displayPrintLine(3, "SEL pause  BACK reset");
      break;
    }

    case TIMER_PAUSED:
      displayPrintLine(1, "Paused");
      displayPrintLine(2, formatMMSS(remainingMs));
      displayPrintLine(3, "SEL resume BACK reset");
      break;

    case TIMER_ALARM:
      // No buzzer wired yet -- alert visually by flashing the whole
      // message on/off every 500ms until SELECT or BACK dismisses it.
      if (now - lastFlashToggleMs >= 500) {
        flashOn = !flashOn;
        lastFlashToggleMs = now;
      }
      if (flashOn) {
        displayPrintLine(1, "TIME'S UP!");
        displayPrintLine(2, "TIME'S UP!");
        displayPrintLine(3, "Press SEL/BACK");
      } else {
        displayPrintLine(1, "");
        displayPrintLine(2, "");
        displayPrintLine(3, "");
      }
      break;
  }
}
