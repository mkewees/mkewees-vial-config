#include QMK_KEYBOARD_H
#include "quantum.h"
#include <qp.h>
#include "qp_st7789.h"
#include "wait.h"

// =====================
// Display State
// =====================
static painter_device_t display;
static bool display_ok = false;

// =====================
// Simple Layer Screen
// =====================
static void draw_layer_screen(uint8_t layer) {
    uint8_t h = (uint8_t)(layer * 32);
    uint8_t s = 255;
    uint8_t v = 80;

    uint8_t wh = 0, ws = 0, wv = 255;

    // Draw the colored background based on the layer
    qp_rect(display,
            0, 0,
            ST7789_DISPLAY_WIDTH - 1,
            ST7789_DISPLAY_HEIGHT - 1,
            h, s, v,
            true);

    // Draw an outline
    qp_rect(display,
            0, 0,
            ST7789_DISPLAY_WIDTH - 1,
            ST7789_DISPLAY_HEIGHT - 1,
            wh, ws, wv,
            false);

    // Push the drawing to the screen
    qp_flush(display);
}

// =====================
// GPIO Probe Tool
// =====================
static const pin_t probe_pins[] = {
    2, 3, 4, 5, 6, 7, 8, 9,
    10, 11, 12, 13, 14
};

static bool probe_enabled __attribute__((unused)) = false;

static void __attribute__((unused)) gpio_probe_init(void) {
    for (uint8_t i = 0; i < ARRAY_SIZE(probe_pins); i++) {
        gpio_set_pin_input_high(probe_pins[i]);  // input + pull-up
    }
    uprintf("GPIO probe ready\n");
}

static void __attribute__((unused)) gpio_probe_task(void) {
    static uint32_t last_print = 0;
    if (timer_elapsed32(last_print) < 50) return;
    last_print = timer_read32();

    for (uint8_t i = 0; i < ARRAY_SIZE(probe_pins); i++) {
        if (!gpio_read_pin(probe_pins[i])) {
            uprintf("Pin %lu LOW\n",
                    (unsigned long)probe_pins[i]);
        }
    }
}

// ======================================================
// IMPORTANT: Prevent duplicate hooks
// ======================================================
#if !defined(KEYMAP_INTROSPECTION)

// =====================
// QMK Init Hook
// =====================
void keyboard_post_init_user(void) {

    uprintf("post_init_user start\n");

#ifdef ST7789_BL_PIN
    gpio_set_pin_output(ST7789_BL_PIN);

    // Blink BL to prove code runs
    for (int i = 0; i < 3; i++) {
        gpio_write_pin_low(ST7789_BL_PIN);
        wait_ms(120);
        gpio_write_pin_high(ST7789_BL_PIN);
        wait_ms(120);
    }
#endif

    // // Delay for WSL/usbipd console re-attach
    // uprintf("post_init_user: delaying to allow console re-attach\n");
    // for (int i = 0; i < 50; i++) {
    //     uprintf("post_init_user waiting for console... %d\n", i);
    //     wait_ms(200);
    // }
    // uprintf("post_init_user: continue\n");

    // Initialize the display hardware handle
    display = qp_st7789_make_spi_device(
        ST7789_DISPLAY_WIDTH,
        ST7789_DISPLAY_HEIGHT,
        ST7789_CS_PIN,
        ST7789_DC_PIN,
        ST7789_RST_PIN,
        64,     // Slow SPI start (stable)
        3       // SPI mode 0
    );

    // Initialize the screen using the handle
    display_ok = qp_init(display, QP_ROTATION_0);
    uprintf("QP init: %s\n", display_ok ? "OK" : "FAIL");

    // If it failed, stop here to avoid crashing

    if (!display_ok) return;

    qp_set_viewport_offsets(display, 0, 80);

    // Wake up the screen and clear it
    qp_power(display, true);
    qp_clear(display);

    // Draw the initial layer color
    draw_layer_screen(get_highest_layer(layer_state));

    // Diagnostic draw: force a full bright blue fill
    qp_rect(display,
        0, 0,
        ST7789_DISPLAY_WIDTH - 1,
        ST7789_DISPLAY_HEIGHT - 1,
        0, 0, 255,
        true);

    // Push diagnostic draw to screen
    qp_flush(display);
    uprintf("diagnostic draw done\n");

}

// =====================
// QMK Housekeeping Hook
// =====================
void housekeeping_task_user(void) {

    // Stop if the screen isn't properly initialized
    if (!display_ok) return;

    // Rate limit the drawing to 20 FPS (every 50ms) to save CPU
    static uint32_t last_ms = 0;
    if (timer_elapsed32(last_ms) < 33) return;
    last_ms = timer_read32();

    // Check if the layer actually changed
    static uint8_t last_layer = 255;
    uint8_t layer = get_highest_layer(layer_state);
    if (layer == last_layer) return;

    // Redraw the screen for the new layer
    last_layer = layer;
    draw_layer_screen(layer);
}

#endif  // !KEYMAP_INTROSPECTION
