#pragma once
#include <Arduino.h>

// Registers the custom LCD characters used by the memento-mori grid.
// Call once from setup(), after displayInit().
void homeInit();

// Renders the home/clock page: time, date, day (columns 0-8), a
// divider at column 9, a memento-mori grid (columns 10-19) mapped to
// the day of the month, and a rotating quote on the bottom row.
// Call every loop() while PAGE_CLOCK is the active page.
void homeTick();
