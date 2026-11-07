#include "clock.h"
#include "config.h"
#include "wifi_manager.h"
#include <time.h>

static bool synced = false;
static unsigned long lastSyncAttemptMs = 0;

static const char* dayNames[]   = {"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
static const char* monthNames[] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};

void clockInit() {
  configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, NTP_SERVER);
  lastSyncAttemptMs = millis();
}

void clockTick() {
  if (!wifiIsConnected()) return;

  struct tm timeinfo;
  if (!synced) {
    // Short timeout keeps this non-blocking in practice; it just
    // returns false immediately if SNTP hasn't responded yet.
    if (getLocalTime(&timeinfo, 100)) {
      synced = true;
    }
    return;
  }

  unsigned long now = millis();
  if (now - lastSyncAttemptMs > NTP_RESYNC_INTERVAL_MS) {
    configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, NTP_SERVER);
    lastSyncAttemptMs = now;
  }
}

bool clockIsSynced() {
  return synced;
}

static bool getNow(struct tm &timeinfo) {
  return getLocalTime(&timeinfo, 10);
}

String clockGetTimeStr() {
  struct tm timeinfo;
  if (!getNow(timeinfo)) return "--:--:--";
  char buf[9];
  snprintf(buf, sizeof(buf), "%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  return String(buf);
}

String clockGetDateStr() {
  struct tm timeinfo;
  if (!getNow(timeinfo)) return "--- --,--";
  char buf[12];
  int yy = (timeinfo.tm_year + 1900) % 100;
  snprintf(buf, sizeof(buf), "%s %02d,%02d", monthNames[timeinfo.tm_mon], timeinfo.tm_mday, yy);
  return String(buf);
}

String clockGetDayStr() {
  struct tm timeinfo;
  if (!getNow(timeinfo)) return "--------";
  return String(dayNames[timeinfo.tm_wday]);
}

int clockGetDayOfMonth() {
  struct tm timeinfo;
  return getNow(timeinfo) ? timeinfo.tm_mday : 0;
}

int clockGetMonth() {
  struct tm timeinfo;
  return getNow(timeinfo) ? timeinfo.tm_mon + 1 : 0;
}

int clockGetYear() {
  struct tm timeinfo;
  return getNow(timeinfo) ? timeinfo.tm_year + 1900 : 0;
}
