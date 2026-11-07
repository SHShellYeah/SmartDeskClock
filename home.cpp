#include "home.h"
#include "display.h"
#include <time.h>

// Custom character slots. Slot 0 is intentionally avoided -- a raw
// 0x00 byte inside a String risks being treated as a terminator by
// some code paths, so the grid uses slots 1 and 2 instead.
static const uint8_t CHAR_FILLED = 1;
static const uint8_t CHAR_EMPTY  = 2;

static byte filledBox[8] = { B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111 };
static byte emptyBox[8]  = { B11111, B10001, B10001, B10001, B10001, B10001, B10001, B11111 };

// Every quote must be <= 20 chars -- it fills the bottom row.
static const char* quotes[] = {
  "Carpe Diem",
  "Make today count",
  "Waiver = Monitor",
  "Focus on the now",
  "Time waits for none"
};
static const int totalQuotes = 5;
static int quoteIndex = 0;
static unsigned long lastQuoteChangeMs = 0;
static const unsigned long QUOTE_INTERVAL_MS = 10000;

void homeInit() {
  displayCreateChar(CHAR_FILLED, filledBox);
  displayCreateChar(CHAR_EMPTY, emptyBox);
  lastQuoteChangeMs = millis();
}

// Builds the 10-character grid segment (columns 10-19) for one of the
// three grid rows, given how many of the 30 total slots are filled.
static String buildGridSegment(int row, int blocksToFill) {
  String seg = "";
  int startIndex = row * 10;
  for (int col = 0; col < 10; col++) {
    int blockIndex = startIndex + col;
    seg += (char)(blockIndex < blocksToFill ? CHAR_FILLED : CHAR_EMPTY);
  }
  return seg;
}

static void renderTimeRows() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo, 10)) return; // not synced yet -- leave rows as-is

  char timeStr[10];
  char dateStr[10];
  char dayStr[16];
  strftime(timeStr, sizeof(timeStr), "%I:%M:%S", &timeinfo);
  strftime(dateStr, sizeof(dateStr), "%b %d,%y", &timeinfo);
  strftime(dayStr, sizeof(dayStr), "%A", &timeinfo);

  int blocksToFill = timeinfo.tm_mday; // 1:1 with day-of-month, out of 30 grid slots

  char leftBuf[10];

  snprintf(leftBuf, sizeof(leftBuf), "%-9.9s", timeStr);
  displayPrintLine(0, String(leftBuf) + "|" + buildGridSegment(0, blocksToFill));

  snprintf(leftBuf, sizeof(leftBuf), "%-9.9s", dateStr);
  displayPrintLine(1, String(leftBuf) + "|" + buildGridSegment(1, blocksToFill));

  snprintf(leftBuf, sizeof(leftBuf), "%-9.9s", dayStr);
  displayPrintLine(2, String(leftBuf) + "|" + buildGridSegment(2, blocksToFill));
}

static void renderQuoteRow() {
  unsigned long now = millis();
  if (now - lastQuoteChangeMs >= QUOTE_INTERVAL_MS) {
    quoteIndex = (quoteIndex + 1) % totalQuotes;
    lastQuoteChangeMs = now;
  }

  char buf[21];
  snprintf(buf, sizeof(buf), "%-20.20s", quotes[quoteIndex]);
  displayPrintLine(3, String(buf));
}

void homeTick() {
  renderTimeRows();
  renderQuoteRow();
}
