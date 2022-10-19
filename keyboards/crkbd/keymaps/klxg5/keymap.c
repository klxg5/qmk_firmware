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

#define _QW 0
#define _NM 1
#define _RB 2
#define _MS 3

enum combos {
    R_LPRN,
    T_RPRN,
    F_LCBR,
    G_RCBR,
    V_LBRC,
    B_RBRC,
    C_TILD
};

const uint16_t PROGMEM lprn_combo[] = {KC_BSPC, KC_R, COMBO_END};
const uint16_t PROGMEM rprn_combo[] = {KC_BSPC, KC_T, COMBO_END};
const uint16_t PROGMEM lcbr_combo[] = {KC_BSPC, KC_F, COMBO_END};
const uint16_t PROGMEM rcbr_combo[] = {KC_BSPC, KC_G, COMBO_END};
const uint16_t PROGMEM lbrc_combo[] = {KC_BSPC, KC_V, COMBO_END};
const uint16_t PROGMEM rbrc_combo[] = {KC_BSPC, KC_B, COMBO_END};
const uint16_t PROGMEM tild_combo[] = {KC_BSPC, KC_C, COMBO_END};

combo_t key_combos[COMBO_COUNT] = {
    [R_LPRN] = COMBO(lprn_combo, KC_LPRN),
    [T_RPRN] = COMBO(rprn_combo, KC_RPRN),
    [F_LCBR] = COMBO(lcbr_combo, KC_LCBR),
    [G_RCBR] = COMBO(rcbr_combo, KC_RCBR),
    [V_LBRC] = COMBO(lbrc_combo, KC_LBRC),
    [B_RBRC] = COMBO(rbrc_combo, KC_RBRC),
    [C_TILD] = COMBO(tild_combo, KC_TILD)
};

typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_HOLD,
    TD_DOUBLE_HOLD,
    TD_TRIPLE_HOLD
} td_state_t;

typedef struct {
    bool is_press_action;
    td_state_t state;
} td_tap_t;

enum {   // based on how many times you tap and hold
    TMO, // MO(1) | MO(2) | MO(3)
    CMH, // ctrl | MEH | HYPER
    GAG, // gui | alt | gui/alt
    SGA, // shift | shift/gui | shift/gui/alt
    DAT, // Lalt | Ralt
    EQB  // ESC QK_BOOT after 5 presses
};

td_state_t cur_dance(qk_tap_dance_state_t *state);

void tmo_fin(qk_tap_dance_state_t *state, void *user_data);
void tmo_rst(qk_tap_dance_state_t *state, void *user_data);
void sga_fin(qk_tap_dance_state_t *state, void *user_data);
void sga_rst(qk_tap_dance_state_t *state, void *user_data);
void cmh_fin(qk_tap_dance_state_t *state, void *user_data);
void cmh_rst(qk_tap_dance_state_t *state, void *user_data);
void gag_fin(qk_tap_dance_state_t *state, void *user_data);
void gag_rst(qk_tap_dance_state_t *state, void *user_data);


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_QW] = LAYOUT_split_3x6_3(
      TD(EQB), KC_Q, KC_W, KC_E, KC_R,    KC_T,                      KC_Y,    KC_U,   KC_I,    KC_O,   KC_P,    KC_MINS,
      KC_TAB,  KC_A, KC_S, KC_D, KC_F,    KC_G,                      KC_H,    KC_J,   KC_K,    KC_L,   KC_SCLN, KC_QUOT,
      TD(CMH), KC_Z, KC_X, KC_C, KC_V,    KC_B,                      KC_N,    KC_M,   KC_COMM, KC_DOT, KC_SLSH, KC_ENT,
                                 TD(GAG), TD(SGA), KC_BSPC,  KC_SPC, TD(TMO), TD(DAT)

  ),

  [_NM] = LAYOUT_split_3x6_3(
      _______, KC_EXLM, KC_AT,   KC_UP,   KC_HASH, KC_PERC,                    KC_PLUS, KC_7, KC_8, KC_9, KC_EQL,  _______,
      _______, KC_CIRC, KC_LEFT, KC_DOWN, KC_RGHT, KC_DLR,                     KC_DOT,  KC_4, KC_5, KC_6, _______, _______,
      _______, KC_AMPR, KC_PIPE, KC_GRV,  KC_QUES, KC_COLN,                    KC_0,    KC_1, KC_2, KC_3, KC_BSLS, _______,
                                          _______, _______, _______,  _______, _______, _______
  ),

  [_RB] = LAYOUT_split_3x6_3(
      QK_BOOT, RGB_TOG, RGB_HUI, RGB_SAI, RGB_VAI, RGB_SPI,                     XXXXXXX, KC_F7, KC_F8, KC_F9, KC_F10, XXXXXXX,
      QK_BOOT, RGB_MOD, RGB_HUD, RGB_SAD, RGB_VAD, RGB_SPD,                     XXXXXXX, KC_F4, KC_F5, KC_F6, KC_F11, XXXXXXX,
      QK_BOOT, RGB_M_P, RGB_M_B, RGB_M_K, RGB_M_G, RGB_M_TW,                    XXXXXXX, KC_F1, KC_F2, KC_F3, KC_F12, XXXXXXX,
                                          _______, _______,  _______,  _______, _______, _______
  ),

  [_MS] = LAYOUT_split_3x6_3(
      XXXXXXX, XXXXXXX, KC_HOME, KC_MS_U, KC_END,  KC_PGUP,                    KC_ACL2, XXXXXXX, KC_WH_U, XXXXXXX, XXXXXXX, XXXXXXX,
      XXXXXXX, XXXXXXX, KC_MS_L, KC_MS_D, KC_MS_R, KC_PGDN,                    KC_ACL1, KC_WH_L, KC_WH_D, KC_WH_R, XXXXXXX, XXXXXXX,
      XXXXXXX, KC_UNDO, KC_CUT,  KC_COPY, KC_PSTE, XXXXXXX,                    KC_ACL0, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                          KC_BTN3, KC_BTN2, KC_BTN1,  XXXXXXX, _______, XXXXXXX
  ),

};

td_state_t cur_dance(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        return TD_SINGLE_HOLD;
    } else if (state->count == 2) {
        return TD_DOUBLE_HOLD;
    } else if (state->count == 3) {
        return TD_TRIPLE_HOLD;
    } else return TD_UNKNOWN;
}

static td_tap_t tmo_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};
static td_tap_t cmh_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};
static td_tap_t gag_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};
static td_tap_t sga_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

void tmo_finish(qk_tap_dance_state_t *state, void *user_data) {
    tmo_tap_state.state = cur_dance(state);
    switch (tmo_tap_state.state) {
        case TD_SINGLE_HOLD: layer_on(1); break;
        case TD_DOUBLE_HOLD: layer_on(2); break;
        case TD_TRIPLE_HOLD: layer_on(3); break;
        default: break;
    }
}

void tmo_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (tmo_tap_state.state) {
        case TD_SINGLE_HOLD: layer_off(1); break;
        case TD_DOUBLE_HOLD: layer_off(2); break;
        case TD_TRIPLE_HOLD: layer_off(3); break;
        default: break;
    }
    tmo_tap_state.state = TD_NONE;
}

void cmh_finish(qk_tap_dance_state_t *state, void *user_data) {
    cmh_tap_state.state = cur_dance(state);
    switch (cmh_tap_state.state) {
        case TD_SINGLE_HOLD: register_code(KC_LCTL); break;
        case TD_DOUBLE_HOLD: register_code(KC_LCTL); register_code(KC_LSFT); register_code(KC_LALT); break;
        case TD_TRIPLE_HOLD: register_code(KC_LCTL); register_code(KC_LSFT); register_code(KC_LALT); register_code(KC_LGUI); break;
        default: break;
    }
}

void cmh_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (cmh_tap_state.state) {
        case TD_SINGLE_HOLD: unregister_code(KC_LCTL); break;
        case TD_DOUBLE_HOLD: unregister_code(KC_LCTL); unregister_code(KC_LSFT); unregister_code(KC_LALT); break;
        case TD_TRIPLE_HOLD: unregister_code(KC_LCTL); unregister_code(KC_LSFT); unregister_code(KC_LALT); unregister_code(KC_LGUI); break;
        default: break;
    }
    cmh_tap_state.state = TD_NONE;
}

void gag_finish(qk_tap_dance_state_t *state, void *user_data) {
    gag_tap_state.state = cur_dance(state);
    switch (gag_tap_state.state) {
        case TD_SINGLE_HOLD: register_code(KC_LGUI); break;
        case TD_DOUBLE_HOLD: register_code(KC_LALT); break;
        case TD_TRIPLE_HOLD: register_code(KC_LGUI); register_code(KC_LALT); break;
        default: break;
    }
}

void gag_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (gag_tap_state.state) {
        case TD_SINGLE_HOLD: unregister_code(KC_LGUI); break;
        case TD_DOUBLE_HOLD: unregister_code(KC_LALT); break;
        case TD_TRIPLE_HOLD: unregister_code(KC_LGUI); unregister_code(KC_LALT); break;
        default: break;
    }
    gag_tap_state.state = TD_NONE;
}

void sga_finish(qk_tap_dance_state_t *state, void *user_data) {
    sga_tap_state.state = cur_dance(state);
    switch (sga_tap_state.state) {
        case TD_SINGLE_HOLD: register_code(KC_LSFT); break;
        case TD_DOUBLE_HOLD: register_code(KC_LSFT); register_code(KC_LGUI); break;
        case TD_TRIPLE_HOLD: register_code(KC_LSFT); register_code(KC_LGUI); register_code(KC_LALT); break;
        default: break;
    }
}

void sga_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (sga_tap_state.state) {
        case TD_SINGLE_HOLD: unregister_code(KC_LSFT); break;
        case TD_DOUBLE_HOLD: unregister_code(KC_LSFT); unregister_code(KC_LGUI); break;
        case TD_TRIPLE_HOLD: unregister_code(KC_LSFT); unregister_code(KC_LGUI); unregister_code(KC_LALT); break;
        default: break;
    }
    sga_tap_state.state = TD_NONE;
}

void eqb_func(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count >= 5) {
        reset_keyboard();
        reset_tap_dance(state);
    } else {
        tap_code(KC_ESC);
        reset_tap_dance(state);
    }
}

qk_tap_dance_action_t tap_dance_actions[] = {
    [TMO] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, tmo_finish, tmo_reset),
    [CMH] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, cmh_finish, cmh_reset),
    [GAG] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, gag_finish, gag_reset),
    [SGA] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, sga_finish, sga_reset),
    [DAT] = ACTION_TAP_DANCE_DOUBLE(KC_LALT, KC_RALT),
    [EQB] = ACTION_TAP_DANCE_FN(eqb_func)
};

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (!is_keyboard_master()) {
        return OLED_ROTATION_180;
    }
    return rotation;
}

void oled_render_layer_status(void) {
    oled_write_P(PSTR("Layer: "), false);

    switch (get_highest_layer(layer_state)) {
        case 0:
            oled_write_P(PSTR("MAIN\n"), false);
            break;
        case 1:
            oled_write_P(PSTR("UTILITY\n"), false);
            break;
        case 2:
            oled_write_P(PSTR("RGB\n"), false);
            break;
        case 3:
            oled_write_P(PSTR("MOUSE\n"), false);
            break;
        default:
            oled_write_ln_P(PSTR("Undefined"), false);
    }
}

static void render_logo(void) {
    static const char PROGMEM qmk_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94,
        0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4,
        0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0x00
    };

    oled_write_P(qmk_logo, false);
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        oled_render_layer_status();  // Renders the current keyboard state (layer, lock, caps, scroll, etc)
    } else {
        render_logo();  // Renders a static logo
    }
    return false;
}

#endif
