/* Copyright 2019 Danny Nguyen <danny@keeb.io>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

enum my_layers {
    _BASE = 0,
    _MOUSE
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
        | Knob 1: Vol Dn/Up |      | Knob 2: Page Dn/Up |
        | Press: Play/Pause |Layer2| Press: RGB Toggle  |
        | Browser Back      | Up   | Browser Forward    |
        | Left              | Down | Right              |
     */
    [_BASE] = LAYOUT(KC_MPLY, TG(1), RGB_TOG, KC_WBAK, KC_UP, KC_WFWD, KC_LEFT, KC_DOWN, KC_RGHT),
    /*
        | Knob 1: Vol Dn/Up |      | Knob 2: Page Dn/Up |
        | Press: Play/Pause |Layer2| Press: RGB Toggle  |
        | Mouse L click     | M. Up| Mouse R click      |
        | Mouse Left        | M. Down | Mouse Right     |
     */
    [_MOUSE] = LAYOUT(KC_MPLY, KC_TRNS, RGB_TOG, KC_BTN1, KC_MS_U, KC_BTN2, KC_MS_L, KC_MS_D, KC_MS_R),
};

bool base_mode = false;
bool mouse_mode = false;

uint32_t layer_state_set_user(uint32_t state) {
    base_mode = false;
    mouse_mode = false;

    switch (biton32(state)) {
    case _MOUSE:
        mouse_mode = true; //for encoder
        rgblight_sethsv_noeeprom(HSV_RED);
        break;
    default: //  for any other layers, or the default layer
        base_mode = true; //for encoder
        rgblight_sethsv_noeeprom(HSV_CYAN);
        break;
    }
    return state;
}

void keyboard_post_init_user(void) {
    // Call the post init code.
    rgblight_enable_noeeprom(); // enables Rgb, without saving settings
    rgblight_mode_noeeprom(RGBLIGHT_MODE_BREATHING + 1); // sets mode to Slow breathing without saving
    rgblight_sethsv_noeeprom(HSV_CYAN);
}

void encoder_update_user(uint8_t index, bool clockwise) {
    if(base_mode == true) {
        // index 0 == left encoder
        if (index == 0) {
            if (clockwise) {
                tap_code(KC_VOLD);
            } else {
                tap_code(KC_VOLU);
            }
        //index 1 == right encoder
        } else if (index == 1) {
            if (clockwise) {
                tap_code(KC_PGUP);
            } else {
                tap_code(KC_PGDN);
            }
        }
    }
    if(mouse_mode == true) {
        if (index == 0) {
            if (clockwise) {
                tap_code(KC_VOLD);
            } else {
                tap_code(KC_VOLU);
            }
        //index 1 == right encoder
        } else if (index == 1) {
            if (clockwise) {
                tap_code(KC_WH_U);
            } else {
                tap_code(KC_WH_D);
            }
        }
    }
}
