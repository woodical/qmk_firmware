#include QMK_KEYBOARD_H

// Callum oneshot mods and swapper
// https://github.com/callum-oakley/qmk_firmware/blob/master/users/callum
#include "oneshot.h"
#include "swapper.h"
#include "keycodes.h"

// Tap Dance declarations
void safe_reset(qk_tap_dance_state_t *state, void *user_data) {
  if (state->count >= 3) {
    // Reset the keyboard if you tap the key more than three times
    reset_keyboard();
    reset_tap_dance(state);
  }
}

void dance_quote_finished(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        register_code16(KC_QUOTE);
    } else {
        register_code16(KC_DQT);
    }
}

void dance_quote_reset(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        unregister_code16(KC_QUOTE);
    } else {
        unregister_code16(KC_DQT);
    }
}

enum {
    TD_RESET,
    TD_QUOTE,
};


// Tap Dance definitions
qk_tap_dance_action_t tap_dance_actions[] = {
    // Tap once for Escape, twice for Caps Lock
    // Reset if key tapped 3 times
    [TD_RESET] = ACTION_TAP_DANCE_FN(safe_reset),
    [TD_QUOTE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_quote_finished, dance_quote_reset),
};


#define TD_RESET TD(TD_RESET)
#define SFT_SPC RSFT_T(KC_SPC)
#define CTL_A LCTL_T(KC_A)
#define CMD_Z LGUI_T(KC_Z)
#define ALT_C LALT_T(KC_C)
#define CMD_CTRL LGUI_T(KC_LCTL)

#define CMD_SLSH RGUI_T(KC_SLSH)
#define CTL_QUOT RCTL_T(KC_QUOT)
#define ALT_COMM RALT_T(KC_COMM)
#define FUN_B LT(_FUN, KC_B) 
#define WNAV_N LT(_WNAV, KC_N) 
#define MY_LBRKT S(KC_LBRC) 
#define MY_RBRKT S(KC_RBRC) 
#define _____ KC_NO

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[_DEF] = LAYOUT_split_3x5_2(
KC_Q,        KC_W,       KC_E,        KC_R,        KC_T,                    KC_Y,       KC_U,       KC_I,           KC_O,         KC_P,   
CTL_A,       KC_S,       KC_D,        KC_F,        KC_G,                    KC_H,       KC_J,       KC_K,           KC_L,         CTL_QUOT,   
CMD_Z,       KC_X,       ALT_C,       KC_V,        FUN_B,                   WNAV_N,     KC_M,       ALT_COMM,       KC_DOT,       CMD_SLSH,   
                                      NUM,         NAV_BSPC,                SFT_SPC,    SYM),   
[_NUM] = LAYOUT_split_3x5_2(
SW_WAPP,     PMIN,       KC_CIRC,     _____,       KC_PERC,                 KC_PLUS,    KC_7,       KC_8,           KC_9,         KC_MINS,   
OS_SHFT,     OS_CTRL,    OS_ALT,      OS_CMD,      KC_ESC,                  KC_DOT,     KC_4,       KC_5,           KC_6,         KC_COLN,   
KC_LEAD,     KC_LEAD,    KC_LEAD,     KC_UNDS,     KC_EQL,                  KC_ASTR,    KC_1,       KC_2,           KC_3,         KC_SLSH,   
                                      KC_TRNS,     KC_TRNS,                 KC_0,       FUN),   
[_SYM] = LAYOUT_split_3x5_2(
KC_EXLM,     KC_AT,      MY_HASH,     KC_DLR,      KC_PERC,                 MY_GBPD,    KC_AMPR,    KC_ASTR,        KC_UNDS,      KC_MINS,   
KC_PIPE,     KC_SCLN,    MY_LBRKT,    MY_RBRKT,    KC_ESC,                  KC_LBRC,    OS_CMD,     OS_ALT,         OS_CTRL,      OS_SHFT,   
KC_LT,       KC_GT,      S(KC_9),     S(KC_0),     KC_GRV,                  KC_RBRC,    KC_EQL,     KC_COLN,        KC_PLUS,      KC_BSLS,   
                                      FUN,         KC_TRNS,                 KC_TRNS,    KC_TRNS),   
[_NAV] = LAYOUT_split_3x5_2(
SW_APP,      _____,      TAB_L,       TAB_R,       KC_TAB,                  KC_HOME,    KC_PGDN,    KC_PGUP,        KC_END,       KC_DEL,   
OS_SHFT,     OS_CTRL,    OS_ALT,      OS_CMD,      KC_ESC,                  KC_LEFT,    KC_DOWN,    KC_UP,          KC_RGHT,      _____,   
M_UNDO,      M_CUT,      M_COPY,      M_PSTE,      M_SAVE,                  _____,      ALF,        _____,          _____,        KC_BSPC,   
                                      KC_TRNS,     KC_TRNS,                 WNAV,       KC_TRNS),   
[_WNAV] = LAYOUT_split_3x5_2(
_____,       KC_VOLD,    KC_MUTE,     KC_VOLU,     _____,                   HOOK,       LHLF,       FULL,           RHLF,         _____,   
_____,       KC_SCRL,    _____,       KC_PAUS,     KC_ESC,                  _____,      W4,         W5,             W6,           _____,   
TG_QWTY,     KC_MPRV,    KC_MPLY,     KC_MNXT,     _____,                   _____,      W1,         W2,             W3,           _____,   
                                      KC_TRNS,     KC_TRNS,                 KC_TRNS,    KC_TRNS),   
[_FUN] = LAYOUT_split_3x5_2(
_____,       _____,      _____,       _____,       KC_CAPS,                 KC_F12,     KC_F7,      KC_F8,          KC_F9,        _____,   
OS_SHFT,     OS_CTRL,    OS_ALT,      OS_CMD,      KC_ESC,                  KC_F11,     KC_F4,      KC_F5,          KC_F6,        _____,   
TD_RESET,    _____,      _____,       _____,       _____,                   KC_F10,     KC_F1,      KC_F2,          KC_F3,        _____,   
                                      KC_TRNS,     KC_TRNS,                 KC_TRNS,    KC_TRNS)
};

enum combo_events {
  // qwerty layer combos
  CAPS_COMBO_Q,
  ENTER_COMBO_Q,
  TAB_COMBO_Q,
  BSP_COMBO_Q,
  DEL_COMBO_Q,
  CTRLC_COMBO_Q,
  ESC_COMBO_Q,
  // Other combos...
  COMBO_LENGTH
};
uint16_t COMBO_LEN = COMBO_LENGTH;
// qwerty combos
const uint16_t PROGMEM caps_combo_q[] = {KC_F, KC_J, COMBO_END};
const uint16_t PROGMEM enter_combo_q[] = {KC_J, KC_K, COMBO_END};
const uint16_t PROGMEM tab_combo_q[] = {KC_F, KC_D, COMBO_END};
const uint16_t PROGMEM bsp_combo_q[] = {KC_F, KC_S, COMBO_END};
const uint16_t PROGMEM del_combo_q[] = {KC_J, KC_L, COMBO_END};
// Ctrl-c combo mostly for Emacs
const uint16_t PROGMEM ctrlc_combo_q[] = {KC_J, KC_K, KC_L, COMBO_END};
const uint16_t PROGMEM esc_combo_q[] = {KC_F, KC_D, KC_S, COMBO_END};

combo_t key_combos[] = {
  [CAPS_COMBO_Q] = COMBO_ACTION(caps_combo_q),
  [ENTER_COMBO_Q] = COMBO(enter_combo_q, KC_ENT),
  [TAB_COMBO_Q] = COMBO(tab_combo_q, KC_TAB),
  [BSP_COMBO_Q] = COMBO(bsp_combo_q, KC_BSPC),
  [DEL_COMBO_Q] = COMBO(del_combo_q, KC_DEL),
  [CTRLC_COMBO_Q] = COMBO(ctrlc_combo_q, LCTL(KC_C)),
  [ESC_COMBO_Q] = COMBO(esc_combo_q, KC_ESC),
};

void process_combo_event(uint16_t combo_index, bool pressed) {
  switch(combo_index) {
    case CAPS_COMBO_Q:
      if (pressed) {
        caps_word_on();
      }
      break;

    // Other combos...
  }
}


bool is_oneshot_cancel_key(uint16_t keycode) {
    switch (keycode) {
	case CLEAR:
    case NUM:
    case SYM:
        return true;
    default:
        return false;
    }
}

bool is_oneshot_ignored_key(uint16_t keycode) {
    switch (keycode) {
	case CLEAR:
    case NUM:
    case SYM:
    case OS_SHFT:
    case OS_CTRL:
    case OS_ALT:
    case OS_CMD:
        return true;
    default:
        return false;
    }
}

bool sw_app_active = false;
bool sw_wapp_active = false;

oneshot_state os_shft_state = os_up_unqueued;
oneshot_state os_ctrl_state = os_up_unqueued;
oneshot_state os_alt_state = os_up_unqueued;
oneshot_state os_cmd_state = os_up_unqueued;


bool process_record_user(uint16_t keycode, keyrecord_t* record) {
	if (!process_caps_word(keycode, record)) { return false; }

	sw_app_active = update_swapper(
                       sw_app_active, 
                       SW_APP == keycode, 
                       KC_LGUI, 
                       KC_TAB, 
                       record
				   );

	sw_wapp_active = update_swapper(
                       sw_wapp_active, 
                       SW_WAPP == keycode, 
                       KC_LALT, 
                       KC_TAB, 
                       record
				   );

	os_shft_state = update_oneshot(
                       os_shft_state, 
                       KC_LSFT, 
                       OS_SHFT,
                       keycode, 
                       record
				   );
	os_ctrl_state = update_oneshot(
                       os_ctrl_state, 
                       KC_LCTL, 
                       OS_CTRL,
                       keycode, 
                       record
				   );
	os_alt_state = update_oneshot(
                       os_alt_state, 
                       KC_LALT, 
                       OS_ALT,
                       keycode, 
                       record
				   );
	os_cmd_state = update_oneshot(
                       os_cmd_state, 
                       KC_LCMD, 
                       OS_CMD,
                       keycode, 
                       record
				   );

	switch (keycode) {
        case MY_GBPD:
          if(record->event.pressed) {
              // TODO not sure these work as expected
              SEND_STRING("£");
          }
          return false; // We handled this keypress
        case MY_HASH:
          if(record->event.pressed) {
              // TODO not sure these work as expected
              SEND_STRING("#");
          }
          return false; // We handled this keypress
		case CLEAR:
            // TODO NOT SURE what this was for.
			clear_oneshot_mods();
			if (get_oneshot_layer() != 0) {
				clear_oneshot_layer_state(ONESHOT_OTHER_KEY_PRESSED);
			}
			layer_move(_DEF);
			return false;
	}

	return true;
}

/*
 *
 * Leader key
 *
 */
LEADER_EXTERNS();

void matrix_scan_user(void) {
  LEADER_DICTIONARY() {
    // This is really needed 
    leading = false;

    SEQ_TWO_KEYS(KC_D, KC_D) {
      SEND_STRING(SS_LCTL("a") SS_LCTL("k"));
    }
    SEQ_TWO_KEYS(KC_G, KC_U) {
        // TOP 
      register_code(KC_LCTL);
      register_code(KC_HOME);
      unregister_code16(KC_HOME);
      unregister_code16(KC_LCTL);
    }
    SEQ_TWO_KEYS(KC_G, KC_O) {
        // BOTTOM 
      register_code(KC_LCTL);
      register_code(KC_END);
      unregister_code16(KC_END);
      unregister_code16(KC_LCTL);
    }
    SEQ_TWO_KEYS(KC_T, KC_Y) {
      // TEST FILE 
      register_code(KC_F9);
      unregister_code16(KC_F9);
    }
    SEQ_TWO_KEYS(KC_T, KC_H) {
      // TEST CLASS 
      register_code(KC_LCTL);
      register_code(KC_LSFT);
      register_code(KC_F10);
      unregister_code16(KC_F10);
      unregister_code16(KC_LSFT);
      unregister_code16(KC_LCTL);
    }
    SEQ_TWO_KEYS(KC_T, KC_N) {
      // TEST FUNCTION 
      register_code(KC_LCTL);
      register_code(KC_F10);
      unregister_code16(KC_F10);
      unregister_code16(KC_LCTL);
    }
    leader_end();
  }
}
