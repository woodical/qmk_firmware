// Copyright Callum Oakley
// https://github.com/callum-oakley/qmk_firmware/tree/master/users/callum

#pragma once

#include QMK_KEYBOARD_H

/* Implements cmd-tab like behaviour on a single key. On first tap of trigger
 * cmdish is held and tabish is tapped -- cmdish then remains held until some
 * other key is hit or released. For example:
 *
 *     trigger, trigger, a -> cmd down, tab, tab, cmd up, a
 *     nav down, trigger, nav up -> nav down, cmd down, tab, cmd up, nav up
 *
 * Usage :
 *
 * sw_app_active = update_swapper(
 *			   sw_app_active, 
 *			   SW_APP == keycode,
 *			   KC_LGUI, 
 *			   KC_TAB, 
 *			   record
 *			   );
 *
 * This behaviour is useful for more than just cmd-tab, hence: cmdish, tabish.
 */
bool update_swapper(
    bool active,
    bool keycode_matched,
    uint16_t cmdish,
    uint16_t tabish,
    keyrecord_t *record
);
