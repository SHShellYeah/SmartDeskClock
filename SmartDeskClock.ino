#include "config.h"
#include "display.h"
#include "wifi_manager.h"
#include "clock.h"
#include "buttons.h"
#include "home.h"
#include "timer.h"

static Page currentPage = PAGE_CLOCK;

static void renderSysInfoPage() {
  displayPrintLine(0, "WiFi");
  displayPrintLine(1, wifiIsConnected() ? wifiGetIP() : "Disconnected");
  displayPrintLine(2, "RSSI " + String(wifiGetRSSI()) + " dBm");
  displayPrintLine(3, "Up " + String(millis() / 60000) + "m");
}

static void renderStubPage(const char* title) {
  displayPrintLine(0, title);
  displayPrintLine(1, "");
  displayPrintLine(2, "");
  displayPrintLine(3, "Coming soon");
}

static void renderCurrentPage() {
  switch (currentPage) {
    case PAGE_CLOCK:    homeTick(); break;
    case PAGE_TIMER:    timerTick(); break;
    case PAGE_SYSINFO:  renderSysInfoPage(); break;
    case PAGE_TODO:     renderStubPage("Todo"); break;
    case PAGE_POMODORO: renderStubPage("Pomodoro"); break;
    case PAGE_WEATHER:  renderStubPage("Weather"); break;
    case PAGE_PCMON:    renderStubPage("PC Monitor"); break;
    case PAGE_CALENDAR: renderStubPage("Calendar"); break;
    case PAGE_SETTINGS: renderStubPage("Settings"); break;
    default: break;
  }
}

static void nextPage() {
  currentPage = (Page)((currentPage + 1) % PAGE_COUNT);
}

static void prevPage() {
  currentPage = (Page)((currentPage + PAGE_COUNT - 1) % PAGE_COUNT);
}

static void handleButtonEvent(ButtonEvent e) {
  switch (e) {
    case BTN_EVENT_NEXT: nextPage(); break;
    case BTN_EVENT_PREV: prevPage(); break;
    case BTN_EVENT_SELECT:
      if (currentPage == PAGE_TIMER) timerHandleSelect();
      break;
    case BTN_EVENT_BACK:
      if (currentPage == PAGE_TIMER) timerHandleBack();
      break;
    default: break;
  }
}

void setup() {
  Serial.begin(115200);

  displayInit();
  displayPrintLine(0, "Booting...");
  homeInit();
  timerInit();

  buttonsInit();
  wifiManagerInit();
  clockInit();
}

void loop() {
  wifiManagerTick();
  clockTick();

  ButtonEvent e = buttonsTick();
  if (e != BTN_EVENT_NONE) {
    handleButtonEvent(e);
  }

  renderCurrentPage();
  displayTick();

  delay(5); // small yield; keeps I2C traffic and button polling sane
}
