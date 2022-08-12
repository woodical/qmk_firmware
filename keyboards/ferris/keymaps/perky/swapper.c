// Copyright Callum Oakley
// https://github.com/callum-oakley/qmk_firmware/tree/master/users/callum

#include "swapper.h"

bool update_swapper(
    bool active,
    bool keycode_matched,
    uint16_t cmdish,
    uint16_t tabish,
    keyrecord_t *record
) {
    bool newstate = active;
    if (keycode_matched) {
        if (record->event.pressed) {
            /* On first press we send CMD then TAB thereafter just TAB */
            if (!active) {
                newstate = true;
                register_code16(cmdish);
            }
            register_code16(tabish);
        } else {
            unregister_code16(tabish);
            // Don't unregister cmdish until some other key is hit or released.
        }
    } else if (active) {
        unregister_code16(cmdish);
        newstate = false;
    }

    return newstate;
}
