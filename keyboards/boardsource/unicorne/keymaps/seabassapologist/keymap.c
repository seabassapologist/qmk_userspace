/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>
Copyright 2024 seabassapologist

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

enum layers {
    BA,
    LO,
    RA,
    AD,
};

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
       EE_CLR,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,                      RGB_TOG,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          QK_BOOT,   KC_NO, KC_TRNS,   KC_TRNS,    KC_NO,   KC_NO
                                      //`--------------------------'  `--------------------------'
  ),
};

layer_state_t layer_state_set_user(layer_state_t state) {
    state = update_tri_layer_state(state, RA, LO, AD);
    return state;
}

#ifdef OLED_ENABLE
#include <oled_bitmaps.h>

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  if (is_keyboard_master()) {
    return OLED_ROTATION_180;  // flips the display 180 degrees if offhand
  }
  return OLED_ROTATION_0;
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        switch (get_highest_layer(layer_state)) {
            case 0:
                oled_write_raw_P(layer_base, sizeof(layer_base));
                break;
            case 1:
                oled_write_raw_P(layer_lower, sizeof(layer_lower));
                break;
            case 2:
                oled_write_raw_P(layer_raise, sizeof(layer_raise));
                break;
            case 3:
                oled_write_raw_P(layer_adjust, sizeof(layer_adjust));
                break;
        }
    } else {
        oled_write_raw_P(logo_custom, sizeof(logo_custom));
    }
    return false;
}
#endif // OLED_ENABLE
