#include QMK_KEYBOARD_H

enum layers {
    _QWERTY,
    _LOWER,
    _RAISE,
    _ADJUST,
};

#define RAISE MO(_RAISE)
#define LOWER MO(_LOWER)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {


 [_QWERTY] = LAYOUT(
    KC_ESC,  KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                        KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_GRV,
    KC_TAB,  KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                        KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_MINS,
    KC_LSFT, KC_A,   KC_S,    KC_D,    KC_F,    KC_G,                        KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
    KC_LCTL, KC_Z,   KC_X,    KC_C,    KC_V,    KC_B,    KC_LBRC,  KC_RBRC,  KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_BSPC,
                     KC_LCTL, KC_LGUI, KC_LALT, LOWER,   KC_SPC,   KC_ENT,   RAISE,   KC_BSPC, KC_RGUI, KC_RALT
),

[_LOWER] = LAYOUT(
    _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                     KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,
    _______, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_F12,
    KC_GRV,  KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                   KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_TILD,
    _______, _______, _______, _______, _______, _______, _______, _______, XXXXXXX, KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_BSLS,
                      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
),

[_RAISE] = LAYOUT(
    _______, _______, _______, _______, _______, _______,                     _______, _______, _______, _______, _______, _______,
    KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                        KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______,
    KC_F1,  KC_F2,    KC_F3,   KC_F4,   KC_F5,   KC_F6,                       XXXXXXX, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, XXXXXXX,
    KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,   _______, _______,  KC_PLUS, KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, KC_BSLS,
                      _______, _______, _______, _______,  _______, _______,  _______, _______, _______, _______
),

[_ADJUST] = LAYOUT(
    QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    RGB_TOG, RGB_HUI, RGB_SAI, RGB_VAI, XXXXXXX, XXXXXXX,                   KC_MPRV, KC_MPLY, KC_MNXT, XXXXXXX, XXXXXXX, XXXXXXX,
    RGB_MOD, RGB_HUD, RGB_SAD, RGB_VAD, XXXXXXX, XXXXXXX,                   KC_VOLU, KC_MUTE, KC_VOLD, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                      _______, _______, _______, _______, _______, _______,  _______, _______, _______, _______
  )
};

layer_state_t layer_state_set_user(layer_state_t state) {
    state = update_tri_layer_state(state, _RAISE, _LOWER, _ADJUST);
    return state;
}

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = { ENCODER_CCW_CW(KC_DOWN, KC_UP),          ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [1] =  { ENCODER_CCW_CW(RGB_HUI, KC_TAB),         ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [2] =  { ENCODER_CCW_CW(RGB_VAD, RGB_VAI),        ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [3] = { ENCODER_CCW_CW(RGB_RMOD, RGB_MOD),       ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
};
#endif // ENCODER_MAP_ENABLE

#ifdef OLED_ENABLE
static uint16_t enc_oled_timer = 0;
static uint8_t  enc_last_idx   = 0;  // 0=left, 1=right
static int8_t   enc_last_dir   = 0;  // +1=CW, -1=CCW
static uint8_t  enc_last_layer = 0;

static void oled_write_action_label(uint16_t keycode) {
    switch (keycode) {
        case KC_DOWN:  oled_write_P(PSTR("DOWN"), false); break;
        case KC_UP:    oled_write_P(PSTR("UP  "), false); break;

        case KC_VOLD:  oled_write_P(PSTR("VOL-"), false); break;
        case KC_VOLU:  oled_write_P(PSTR("VOL+"), false); break;

        case RGB_HUI:  oled_write_P(PSTR("RGB+H"), false); break;
        case RGB_VAI:  oled_write_P(PSTR("RGB+V"), false); break;
        case RGB_VAD:  oled_write_P(PSTR("RGB-V"), false); break;
        case RGB_MOD:  oled_write_P(PSTR("RGB> "), false); break;
        case RGB_RMOD: oled_write_P(PSTR("RGB< "), false); break;

        case KC_TAB:   oled_write_P(PSTR("TAB "), false); break;

        default:       oled_write_P(PSTR("----"), false); break;
    }
}
#endif // OLED_ENABLE

bool encoder_update_user(uint8_t index, bool clockwise) {
#ifdef OLED_ENABLE
    enc_last_idx   = index;
    enc_last_dir   = clockwise ? 1 : -1;
    enc_last_layer = get_highest_layer(layer_state | default_layer_state);
    enc_oled_timer = timer_read();
#endif
    return true; // IMPORTANT: allow encoder_map handling to proceed
}

#ifdef OLED_ENABLE
bool oled_task_user(void) {
    // Show widget for 800ms after encoder movement
    if (timer_elapsed(enc_oled_timer) < 800) {
        uint8_t layer = enc_last_layer;

        // Clamp to your encoder_map layers (0..3)
        if (layer > 3) layer = 3;

        // CCW=0, CW=1 for encoder_map direction index
        uint8_t dir_idx = (enc_last_dir > 0) ? 1 : 0;

        // Read the mapped action keycode from PROGMEM
#ifdef ENCODER_MAP_ENABLE
        uint16_t action = pgm_read_word(&encoder_map[layer][enc_last_idx][dir_idx]);
#else
        uint16_t action = KC_NO;
#endif

        oled_set_cursor(0, 0);
        oled_write_P(PSTR("E"), false);
        oled_write_char('0' + enc_last_idx, false);
        oled_write_P(PSTR(": "), false);
        oled_write_action_label(action);
    }

    return true;
}
#endif // OLED_ENABLE