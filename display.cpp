#include "display.h"
#include "config.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

static LiquidCrystal_I2C lcd(LCD_I2C_ADDR, LCD_COLS, LCD_ROWS);
static String lineCache[LCD_ROWS];
static bool initialized = false;

void displayInit() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
  for (uint8_t i = 0; i < LCD_ROWS; i++) {
    lineCache[i] = "";
  }
  initialized = true;
}

void displayClearLine(uint8_t row) {
  displayPrintLine(row, "");
}

void displayPrintLine(uint8_t row, const String &textIn) {
  if (!initialized || row >= LCD_ROWS) return;

  String text = textIn;
  if (text.length() > LCD_COLS) {
    text = text.substring(0, LCD_COLS);
  } else {
    while (text.length() < LCD_COLS) text += ' '; // pad so old chars get overwritten
  }

  if (text == lineCache[row]) return; // unchanged -> skip the I2C write entirely

  lcd.setCursor(0, row);
  lcd.print(text);
  lineCache[row] = text;
}

void displayTick() {
  // No-op for now. Placeholder for future scrolling/animation logic
  // (e.g. long quote text scrolling on the bottom row).
}

void displayCreateChar(uint8_t slot, byte charmap[8]) {
  if (!initialized || slot > 7) return;
  lcd.createChar(slot, charmap);
}
