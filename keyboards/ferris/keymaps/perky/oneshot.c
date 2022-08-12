#include "oneshot.h"

// modified version to return the state instead of passing it in to modify.
oneshot_state update_oneshot(
    oneshot_state state,
    uint16_t mod,
    uint16_t trigger,
    uint16_t keycode,
    keyrecord_t *record
) {
    oneshot_state newstate = state;
    if (keycode == trigger) {
        if (record->event.pressed) {
            // Trigger keydown
            if (state == os_up_unqueued) {
                register_code(mod);
            }
            newstate = os_down_unused;
        } else {
            // Trigger keyup
            switch (state) {
            case os_down_unused:
                // If we didn't use the mod while trigger was held, queue it.
                newstate = os_up_queued;
                break;
            case os_down_used:
                // If we did use the mod while trigger was held, unregister it.
                newstate = os_up_unqueued;
                unregister_code(mod);
                break;
            default:
                break;
            }
        }
    } else {
        if (record->event.pressed) {
            if (is_oneshot_cancel_key(keycode) && state != os_up_unqueued) {
                // Cancel oneshot on designated cancel keydown.
                newstate = os_up_unqueued;
                unregister_code(mod);
            }
        } else {
            if (!is_oneshot_ignored_key(keycode)) {
                // On non-ignored keyup, consider the oneshot used.
                switch (state) {
                case os_down_unused:
                    newstate = os_down_used;
                    break;
                case os_up_queued:
                    newstate = os_up_unqueued;
                    unregister_code(mod);
                    break;
                default:
                    break;
                }
            }
        }
    }
    return newstate;
}
