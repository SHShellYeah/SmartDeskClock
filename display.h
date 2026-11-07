#pragma once
#include <Arduino.h>

// Initializes the I2C LCD and clears the internal line cache.
void displayInit();

// Clears a single row (writes 20 spaces) without a full lcd.clear().
void displayClearLine(uint8_t row);

// Writes text to a row, but only touches the physical LCD if the
// padded/truncated content differs from what's already there.
// This is what keeps the UI flicker-free.
void displayPrintLine(uint8_t row, const String &text);

// Reserved for future use (scrolling, animations). Call once per loop().
void displayTick();

// Registers a custom character in one of the LCD's 8 CGRAM slots.
// Avoid slot 0 for anything embedded inside a String elsewhere in the
// codebase -- a raw 0x00 byte risks being treated as a string terminator.
void displayCreateChar(uint8_t slot, byte charmap[8]);
