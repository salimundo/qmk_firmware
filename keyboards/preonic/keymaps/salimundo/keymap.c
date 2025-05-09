/* Copyright 2015-2017 Jack Humbert
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
#include "muse.h"

enum preonic_layers {
  _QWERTY,
  _RAISE,
};

enum preonic_keycodes {
  QWERTY = SAFE_RANGE,
  RAISE,
  BACKLIT,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Qwerty
 * ,-----------------------------------------------------------------------------------.
 * |   `  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  |  _   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  |  =   |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Esc  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |  "   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  |  \   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Ctrl | GUI  | Alt  |Raise | Bksp |    Space    |Enter |Raise |  RGB |   [  |  ]   |
 * `-----------------------------------------------------------------------------------'
 */
[_QWERTY] = LAYOUT_preonic_grid(
  KC_GRV,        KC_1,    KC_2,    KC_3,  KC_4,    KC_5,   KC_6,   KC_7,   KC_8,    KC_9,    KC_0,    KC_MINS,
  KC_TAB,        KC_Q,    KC_W,    KC_E,  KC_R,    KC_T,   KC_Y,   KC_U,   KC_I,    KC_O,    KC_P,    KC_EQL,
  ALL_T(KC_ESC), KC_A,    KC_S,    KC_D,  KC_F,    KC_G,   KC_H,   KC_J,   KC_K,    KC_L,    KC_SCLN, KC_QUOT,
  KC_LSFT,       KC_Z,    KC_X,    KC_C,  KC_V,    KC_B,   KC_N,   KC_M,   KC_COMM, KC_DOT,  KC_SLSH, KC_BSLASH,
  KC_LCTL,       KC_LGUI, KC_LALT, RAISE, KC_BSPC, KC_SPC, KC_SPC, KC_ENT, RAISE,   RGB_TOG,  KC_LBRC, KC_RBRC
),

/* Raise
 * ,-----------------------------------------------------------------------------------.
 * |      |  F1  |  F2  |  F3  |  F4  |  F5  | F6   |  F7  |  F8  |  F9  |  F10 |  F11 |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |RGB Mo| Hue  | Sat  |Bright|      |AudRR |AudSt |AudPl |AudFF |      |  F12 |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * |Voice+|Aud On|AudOff|      |      |      | Left | Down |  Up  |Right |      |Vol up|
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |Voice-|Mus On|MusOff|Mus Cy|      |      | Home | PgDn | PgUp | End  |      |Vol dn|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |MidiOn|MidOff|      |      |      |      |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_RAISE] = LAYOUT_preonic_grid(
  _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,
  _______, RGB_MOD, RGB_HUI, RGB_SAI, RGB_VAI, _______, KC_MPRV, KC_MSTP, KC_MPLY, KC_MNXT, _______, KC_F12,
  MUV_IN,  AU_ON,   AU_OFF,  _______, _______, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______, KC_VOLU,
  MUV_DE,  MU_ON,   MU_OFF,  MU_MOD,  _______, _______, KC_HOME, KC_PGDN, KC_PGUP, KC_END,  _______, KC_VOLD,
  _______, MI_ON,   MI_OFF,  _______, _______, _______, _______, _______, _______, _______, _______, _______
)
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
        case QWERTY:
          if (record->event.pressed) {
            set_single_persistent_default_layer(_QWERTY);
          }
          return false;
          break;
        case RAISE:
          if (record->event.pressed) {
            layer_on(_RAISE);
          } else {
            layer_off(_RAISE);
          }
          return false;
          break;
      }
    return true;
};

bool muse_mode = false;
uint8_t last_muse_note = 0;
uint16_t muse_counter = 0;
uint8_t muse_offset = 70;
uint16_t muse_tempo = 50;

bool encoder_update_user(uint8_t index, bool clockwise) {
  if (muse_mode) {
    if (IS_LAYER_ON(_RAISE)) {
      if (clockwise) {
        muse_offset++;
      } else {
        muse_offset--;
      }
    } else {
      if (clockwise) {
        muse_tempo+=1;
      } else {
        muse_tempo-=1;
      }
    }
  } else {
    if (clockwise) {
      register_code(KC_PGDN);
      unregister_code(KC_PGDN);
    } else {
      register_code(KC_PGUP);
      unregister_code(KC_PGUP);
    }
  }
    return true;
}


void matrix_scan_user(void) {
#ifdef AUDIO_ENABLE
    if (muse_mode) {
        if (muse_counter == 0) {
            uint8_t muse_note = muse_offset + SCALE[muse_clock_pulse()];
            if (muse_note != last_muse_note) {
                stop_note(compute_freq_for_midi_note(last_muse_note));
                play_note(compute_freq_for_midi_note(muse_note), 0xF);
                last_muse_note = muse_note;
            }
        }
        muse_counter = (muse_counter + 1) % muse_tempo;
    } else {
        if (muse_counter) {
            stop_all_notes();
            muse_counter = 0;
        }
    }
#endif
}

bool music_mask_user(uint16_t keycode) {
  switch (keycode) {
    case RAISE:
    default:
      return true;
  }
}
