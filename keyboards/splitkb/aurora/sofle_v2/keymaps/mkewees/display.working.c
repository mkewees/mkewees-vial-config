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

//The qp_init function is used to initialise a display device after it has been created. This accepts a rotation parameter (QP_ROTATION_0, QP_ROTATION_90, QP_ROTATION_180, QP_ROTATION_270), which makes sure that the orientation of what's drawn on the display is correct.

bool qp_init(painter_device_t device, painter_rotation_t rotation);

static painter_device_t display;
void keyboard_post_init_kb(void) {
    display = qp_make_.......;         // Create the display
    qp_init(display, QP_ROTATION_0);   // Initialise the display
}
