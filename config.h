#pragma once

// ================= LCD =================
#define LCD_I2C_ADDR        0x27   // change to 0x3F if your backpack uses that address
#define LCD_COLS            20
#define LCD_ROWS            4

// ================= Buttons =================
// 4 push switches, each wired: GPIO -> switch -> GND (INPUT_PULLUP, active LOW).
// Pins chosen to avoid ESP32 strapping pins; change freely to match your wiring.
#define BTN_NEXT_PIN         32
#define BTN_PREV_PIN         33
#define BTN_SELECT_PIN       25
#define BTN_BACK_PIN         26
#define BUTTON_DEBOUNCE_MS   40   // ms a reading must hold steady before it counts

// ================= WiFi =================
#define WIFI_SSID               "YOUR_WIFI_SSID"
#define WIFI_PASSWORD           "YOUR_WIFI_PASSWORD"
#define WIFI_CONNECT_TIMEOUT_MS 15000
#define WIFI_RETRY_INTERVAL_MS  10000

// ================= NTP =================
#define NTP_SERVER               "pool.ntp.org"
#define GMT_OFFSET_SEC           (6 * 3600)  // Bangladesh = UTC+6
#define DAYLIGHT_OFFSET_SEC      0
#define NTP_RESYNC_INTERVAL_MS   (60UL * 60UL * 1000UL) // resync hourly

// ================= Pages =================
// Full page list from the roadmap plus Timer. PAGE_CLOCK, PAGE_TIMER,
// and PAGE_SYSINFO render for real; the rest show a "Coming soon" stub
// so button navigation and the page framework are already exercised.
enum Page {
  PAGE_CLOCK = 0,
  PAGE_TODO,
  PAGE_POMODORO,
  PAGE_TIMER,
  PAGE_WEATHER,
  PAGE_PCMON,
  PAGE_CALENDAR,
  PAGE_SYSINFO,
  PAGE_SETTINGS,
  PAGE_COUNT
};
