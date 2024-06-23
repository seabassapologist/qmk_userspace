/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include <stdio.h>

enum layers {
    BA,
    LO,
    RA,
    AD,
};

// Backlight timeout feature
#define BACKLIGHT_TIMEOUT 5    // in minutes
static uint16_t idle_timer = 0;
static uint8_t halfmin_counter = 0;
static bool led_on = true;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [BA] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
  LCTL_T(KC_ESC), KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                         KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                         KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, RSFT_T(KC_MINS),
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                 KC_LALT,  KC_SPC,LT(LO,KC_ENT),      LT(RA,KC_ENT),KC_SPC,  LGUI_T(KC_DEL)
                                      //`--------------------------'  `--------------------------'
  ),

  [LO] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_TAB,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
  LCTL_T(KC_ESC), KC_6,    KC_7,    KC_8,    KC_9,    KC_0,                      KC_LEFT, KC_DOWN,   KC_UP, KC_RGHT,   KC_NO,   KC_NO,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                        KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,  KC_F12,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_TRNS, KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS,  KC_TRNS
                                      //`--------------------------'  `--------------------------
  ),

  [RA] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_TAB, KC_EXLM,   KC_AT, KC_LBRC, KC_RBRC, KC_CIRC,                       KC_EQL, KC_AMPR, KC_PIPE, KC_ASTR, KC_PLUS, KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
  LCTL_T(KC_ESC),KC_HASH,KC_DLR, KC_LPRN, KC_RPRN,  KC_GRV,                      KC_LEFT, KC_DOWN,   KC_UP, KC_RGHT, KC_BSLS, KC_QUOT,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT, KC_PERC,   KC_NO, KC_LCBR, KC_RCBR, KC_TILD,                      KC_PGUP, KC_PGDN,  KC_END, KC_HOME,  KC_APP, RSFT_T(KC_MINS),
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_TRNS, KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS
                                      //`--------------------------'  `--------------------------'
  ),

  [AD] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
        KC_NO,   KC_NO, RGB_M_T, RGB_M_G, RGB_M_X, RGB_M_K,                      RGB_HUI, RGB_SAI, RGB_VAI, RGB_SPI, RGB_MOD,   KC_NO,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        KC_NO,RGB_M_SN,RGB_M_SW, RGB_M_R, RGB_M_B, RGB_M_P,                      RGB_HUD, RGB_SAD, RGB_VAD, RGB_SPD,RGB_RMOD,   KC_NO,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,                      RGB_TOG,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                            KC_NO,   KC_NO, KC_TRNS,   KC_TRNS,    KC_NO,   KC_NO
                                      //`--------------------------'  `--------------------------'
  ),
};

layer_state_t layer_state_set_user(layer_state_t state) {
    state = update_tri_layer_state(state, RA, LO, AD);
    return state;
}

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  if (is_keyboard_master()) {
    return OLED_ROTATION_180;  // flips the display 180 degrees if offhand
  }
  return OLED_ROTATION_0;
}

void oled_render_layer_state(void) {
    // Host Keyboard Layer Status
    oled_write_P(PSTR("\nLayer: "), false);
    switch (get_highest_layer(layer_state)) {
        case BA:
            oled_write_ln_P(PSTR("Base"), false);
            break;
        case LO:
            oled_write_ln_P(PSTR("Lower"), false);
            break;
        case RA:
            oled_write_ln_P(PSTR("Raise"), false);
            break;
        case AD:
            oled_write_ln_P(PSTR("Adjust"), false);
            break;
        default:
            oled_write_ln_P(PSTR("Undefined"), false);
    }
}

void oled_render_logo(void) {
    static const char PROGMEM crkbd_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94,
        0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4,
        0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4,
        0};
    oled_write_P(PSTR("\n"), false);
    
    oled_write_P(crkbd_logo, false);
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        //oled_render_layer_state();
        oled_render_layer_state();
    } else {
        oled_render_logo();
    }
    return false;
}

#endif // OLED_ENABLE

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        #ifdef RGBLIGHT_ENABLE
        if (!led_on) {
            rgblight_enable();
            led_on = true;
        }
        #endif
        idle_timer = timer_read();
        halfmin_counter = 0;
    }
    return true;
  }

void matrix_scan_user(void) {
    // idle_timer needs to be set one time
    if (idle_timer == 0) idle_timer = timer_read();

    #ifdef RGBLIGHT_ENABLE
        if ( led_on && timer_elapsed(idle_timer) > 30000) {
            halfmin_counter++;
            idle_timer = timer_read();
        }

        if ( led_on && halfmin_counter >= BACKLIGHT_TIMEOUT * 2) {
            rgblight_disable();
            led_on = false;
            halfmin_counter = 0;
        }
    #endif
}
