/**
 * Copyright 2021 Charly Delay <charly@codesink.dev> (@0xcharly)
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
#pragma once

#ifdef VIA_ENABLE
/* VIA configuration. */
#    define DYNAMIC_KEYMAP_LAYER_COUNT 7
#endif // VIA_ENABLE

#ifndef __arm__
/* Disable unused features. */
#    define NO_ACTION_ONESHOT
#endif // __arm__

#include "raw_hid.h"
#include "usb_descriptor.h"

// Notify about layer changes
layer_state_t layer_state_set_user(layer_state_t state) {
    uint8_t data[RAW_EPSIZE] = {0};
    data[0] = 0xFF;
    data[1] = sizeof(layer_state_t);
    memcpy(&data[2], &default_layer_state, sizeof(layer_state_t));
    memcpy(&data[2 + sizeof(layer_state_t)], &state, sizeof(layer_state_t));
    raw_hid_send(data, RAW_EPSIZE);
    return state;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record){
    static uint8_t data[RAW_EPSIZE];
    data[0] = 0xF1;
    data[1] = record->event.key.row;
    data[2] = record->event.key.col;
    data[3] = record->event.pressed ? 1 : 0;
    raw_hid_send(data, RAW_EPSIZE);
    return true;
}

/* Charybdis-specific features. */

#ifdef POINTING_DEVICE_ENABLE
// Automatically enable the pointer layer when moving the trackball.  See also:
// - `CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS`
// - `CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD`
// #define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
#endif // POINTING_DEVICE_ENABLE
