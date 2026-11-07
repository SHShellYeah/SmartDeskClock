# SmartDeskClock — Stage 1: Core Firmware

Gets the clock physically working: LCD driver, WiFi manager, NTP clock,
4-button navigation, and a working Timer mode. Todo, Pomodoro, Weather,
PC Monitor, Calendar, and Settings pages are wired into the page
framework as stubs ("Coming soon") so navigation already cycles through
all 9 pages — their real logic gets filled in in later stages.

## Wiring

| LCD2004 (I2C backpack) | ESP32 |
|---|---|
| VCC | 5V (or 3.3V if your backpack supports it) |
| GND | GND |
| SDA | GPIO 21 (default I2C SDA) |
| SCL | GPIO 22 (default I2C SCL) |

| Button | ESP32 | Wiring |
|---|---|---|
| Next | GPIO 32 | switch to GND, `INPUT_PULLUP` |
| Prev | GPIO 33 | switch to GND, `INPUT_PULLUP` |
| Select | GPIO 25 | switch to GND, `INPUT_PULLUP` |
| Back | GPIO 26 | switch to GND, `INPUT_PULLUP` |

No external pull-up resistors needed — the ESP32's internal pull-ups are
enabled in software, so each button just needs one leg on its GPIO and
the other leg on GND. Pins are freely changeable in `config.h` if they
clash with anything else on your board.

If your backpack uses a different I2C address than `0x27`, run an I2C
scanner sketch once and update `LCD_I2C_ADDR` in `config.h`.

## Arduino IDE setup

1. Install the **ESP32 board package** (Boards Manager → search "esp32").
2. Install libraries (Library Manager):
   - `LiquidCrystal I2C` (for the LCD)
3. `WiFi.h`, `Wire.h`, and `time.h` ship with the ESP32 core — no install needed.
4. Open `SmartDeskClock.ino`, select your board (e.g. "ESP32 Dev Module"),
   pick the right COM/serial port, and upload.

## Before uploading

Edit `config.h`:
- `WIFI_SSID` / `WIFI_PASSWORD` — your network credentials
- `LCD_I2C_ADDR` — `0x27` or `0x3F`, whichever your backpack uses
- `GMT_OFFSET_SEC` — already set to UTC+6 (Bangladesh); change if needed
- `BTN_*_PIN` — adjust if you're not using GPIO 32/33/25/26

## Button behavior

- **Next / Prev** — always move between pages, on every page.
- **Select / Back** — currently only meaningful on the **Timer** page
  (reserved for other pages' context actions in later stages):
  - **Set** (idle): `Select` starts the countdown at the current preset,
    `Back` cycles the preset (5/10/15/20/25/30/45/60 min).
  - **Running**: `Select` pauses, `Back` cancels back to Set.
  - **Paused**: `Select` resumes, `Back` cancels back to Set.
  - **Alarm** (time's up): the whole message flashes on/off every 500ms
    since no buzzer is wired yet — `Select` or `Back` dismisses it.

## What's implemented

- **display.cpp** — LCD wrapper with per-line dirty-checking, so a line is
  only rewritten when its content actually changes. No `lcd.clear()` calls
  after boot, so no flicker. Also exposes `displayCreateChar()` for custom
  glyphs (used by the memento-mori grid).
- **wifi_manager.cpp** — connects on boot, retries on a timer if the
  connection drops or times out. Never blocks `loop()`.
- **clock.cpp** — syncs time via NTP once WiFi is up, resyncs hourly,
  exposes formatted time/date/day strings (used by other pages later).
- **home.cpp** — the clock/home page itself: 12-hour time, date, and day
  on columns 0-8 of rows 0-2, a `|` divider at column 9, and a 10x3
  memento-mori grid (columns 10-19) that fills in with the day of the
  month. Bottom row rotates through a quote list every 10 seconds.
- **buttons.cpp** — 4-pin debounced input, fires one event per confirmed
  press (not on release, not while held), fully `millis()`-based.
- **timer.cpp** — countdown timer with duration presets, pause/resume,
  and a flashing visual alarm on completion.
- **SmartDeskClock.ino** — page state machine wiring the above together.

## Next stages

Per the roadmap: task manager + persistent storage (Preferences), the
real Pomodoro timer (very similar shape to timer.cpp, but with
work/break cycles instead of a single countdown), weather API
integration, PC monitor receiver, then the web dashboard and polish
(OTA, animations, power-saving, and — once you know your buzzer type —
a real audible alarm for Timer mode).
