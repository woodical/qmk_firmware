/* Copyright 2021 Jay Greco
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

#include "quantum.h"
#include "common/remote_kb.h"
#include "common/bitc_led.h"

typedef struct PACKED {
    uint8_t r;
    uint8_t c;
} encodermap_t;

// Map encoders to their respective virtual matrix entry
// Allows for encoder control using VIA
const encodermap_t encoder_map[4][2] = {
    {{1, 0}, {1, 1}},  // Encoder 1 matrix location
    {{2, 0}, {2, 1}},  // Encoder 2 matrix location
    {{3, 0}, {3, 1}},  // Encoder 3 matrix location
    {{4, 0}, {4, 1}},  // Encoder 4 matrix location
};

bool numlock_set = false;

#ifdef OLED_ENABLE
oled_rotation_t oled_init_kb(oled_rotation_t rotation) {
    return OLED_ROTATION_180;
}

static void render_logo(void) {
    static const char PROGMEM tidbit_oled[] = {
        0x00, 0x0e, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
        0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x0e, 0x00, 0x00, 0x00, 0xfe, 0xff, 0xff, 0xff, 
        0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xfe, 0xfe, 0xff, 0xff, 0xff, 0x1f, 0x1f, 0x1f, 
        0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x3e, 0x3e, 0x7e, 0xfc, 0xfc, 0xf8, 0xf0, 0xe0, 0xc0, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0xf8, 0xfe, 0xfe, 0xff, 0xff, 0xff, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 
        0x1f, 0x1f, 0x3f, 0x7e, 0xfe, 0xfe, 0xfc, 0xf8, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0xfe, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x0e, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 
        0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1e, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 
        0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xc0, 
        0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 
        0xe0, 0xf0, 0xf0, 0xf8, 0xff, 0xff, 0xbf, 0x1f, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 
        0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xe0, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x03, 
        0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 
        0x03, 0x03, 0x03, 0x07, 0x07, 0xbf, 0xff, 0xff, 0xff, 0xfe, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x7f, 0xff, 0xff, 0x7f, 0x3f, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 
        0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x7f, 0x7f, 0x7f, 0xff, 0xff, 0xf8, 0xf8, 0xf8, 
        0xf8, 0xf8, 0xf8, 0x7c, 0x7c, 0x7c, 0x7e, 0x3e, 0x3f, 0x3f, 0x1f, 0x0f, 0x07, 0x01, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x1f, 0x7f, 0x7f, 0x7f, 0xff, 0xff, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 
        0xf8, 0x7c, 0x7c, 0x7c, 0x7e, 0x3f, 0x3f, 0x1f, 0x1f, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x7f, 0xff, 0xff, 0x7f, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x3f, 0x7f, 0xff, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    
    oled_write_raw_P(tidbit_oled, sizeof(tidbit_oled));
};

bool oled_task_kb(void) {
    if (!oled_task_user()) return false;
    render_logo();
    return true;
}

#endif

static void process_encoder_matrix(encodermap_t pos) {
    action_exec(MAKE_KEYEVENT(pos.r, pos.c, true));
#if TAP_CODE_DELAY > 0
    wait_ms(TAP_CODE_DELAY);
#endif
    action_exec(MAKE_KEYEVENT(pos.r, pos.c, false));
}

bool encoder_update_kb(uint8_t index, bool clockwise) {
    if (!encoder_update_user(index, clockwise)) return false;
    process_encoder_matrix(encoder_map[index][clockwise ? 0 : 1]);
    return false;
}

// Use Bit-C LED to show NUM LOCK status
void led_update_ports(led_t led_state) {
    set_bitc_LED(led_state.num_lock ? LED_DIM : LED_OFF);
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    process_record_remote_kb(keycode, record);
    if (!process_record_user(keycode, record)) return false;

    // Get the current NLCK status & set if not set.
    // Only do this once, in case user has a NLCK key
    // and wants to disable it later on.
    if (!numlock_set && record->event.pressed) {
        led_t led_state = host_keyboard_led_state();
        if (!led_state.num_lock) {
            register_code(KC_NUM_LOCK);
        }
        numlock_set = true;
    }

    switch (keycode) {
        case QK_BOOT:
            if (record->event.pressed) {
                set_bitc_LED(LED_DIM);
                rgblight_disable_noeeprom();
                #ifdef OLED_ENABLE
                oled_off();
                #endif
                bootloader_jump();  // jump to bootloader
            }
            return false;

        default:
            break;
    }

    return true;
}

void matrix_init_kb(void) {
    set_bitc_LED(LED_OFF);
    matrix_init_remote_kb();
    matrix_init_user();
}

void matrix_scan_kb(void) {
    matrix_scan_remote_kb();
    matrix_scan_user();
}
