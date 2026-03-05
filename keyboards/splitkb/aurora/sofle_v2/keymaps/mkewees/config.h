/* SPDX-License-Identifier: GPL-2.0-or-later */
/* Copyright 2023 splitkb.com <support@splitkb.com> */

#pragma once

#define VIAL_INSECURE

#define ENCODER_RESOLUTION 2
#define VIAL_KEYBOARD_UID {0x7C, 0x39, 0x7C, 0x45, 0x65, 0xB7, 0x48, 0x82}
#define VIAL_UNLOCK_COMBO_ROWS { 0, 5 }
#define VIAL_UNLOCK_COMBO_COLS { 0, 0 }

#define RGB_MATRIX_FRAMEBUFFER_EFFECTS
#define RGB_MATRIX_KEYPRESSES

#define DYNAMIC_KEYMAP_LAYER_COUNT 8

// --- SPI bus selection (RP2040 + ChibiOS) ---

#define SPI_DRIVER_REQUIRED
#define SPI_DRIVER SPID0

// Your “OLED pads”
// ST7789 panels are write‑only devices, so there is no MISO line; only
// the clock and MOSI (labelled SDA or RGB data) are required.  The other
// control pins are:
//   RESX (reset)  - pulled low on power‑up, usually tied high by firmware
//   CSX  (chip select) - active low; the panel accepts commands/data only
//                       when CS is low.
//   D/CX (data/command) - low for commands, high for data.
// Timing details:  the module uses standard SPI mode 0 (CPOL=0, CPHA=0),
// i.e. data is sampled on the first (falling) edge of SCLK and the clock
// idles low.  Eight bits are transferred MSB‑first in each cycle.
//
// These pins must match how the panel is wired on your board.  When the
// display stays dark but the driver reports "OK", it generally means we
// are talking to the wrong pins.  On many SplitKB boards the display is
// connected to the secondary SPI bus (SPID1) rather than SPI0 – see the
// RP2040 pinout above.  To switch to SPI1, uncomment the block at the
// end of this file and comment out the SPID0 section below.
#define SPI_SCK_PIN  2U  // D1 = GP2
#define SPI_MOSI_PIN 3U  // D0 = GP3 (display SDA/DIN)

#define SPI_MISO_PIN NO_PIN   // ST7789 is write-only

// --- ST7789 control pins (your convenient block) ---

#define ST7789_DC_PIN 13U    // now maps to RP2040 pin 13U (bottom row)
// CS line wasn't B0 on the new converter; B0 no longer exists.  use a
// bottom‑row pin such as B7 or D5.  start with B7 here and move if necessary.
#define ST7789_CS_PIN 12U   // bottom row pin (B7)
// Reset/backlight: pick two unused bottom-row pins.  C7 and F0 are free
// according to the latest converter, so use those.
#define ST7789_RST_PIN 14U  // (C7)
#define ST7789_BL_PIN 15U  // (F0) optional; remove if BL tied to 3V3

#define ST7789_DISPLAY_WIDTH  240
#define ST7789_DISPLAY_HEIGHT 240
#define ST7789_NUM_DEVICES 2

// --------------------------------------------------------------------
// Alternative SPI1 configuration (uncomment to try)
// --------------------------------------------------------------------
//#undef SPI_DRIVER
//#define SPI_DRIVER SPID1

//#undef SPI_SCK_PIN
//#define SPI_SCK_PIN GP10   // often labelled SPI1 SCK on RP2040 boards
//#undef SPI_MOSI_PIN
//#define SPI_MOSI_PIN GP11  // labelled SPI1 TX (MOSI)

// Note: CS/DC/RST definitions may also need updating if the panel uses
// different GPIOs.  Inspect the PCB silkscreen or continuity‑probe from
// the display pads to the RP2040 pins to determine the correct values.
