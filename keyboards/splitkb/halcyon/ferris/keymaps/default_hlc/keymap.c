// Copyright 2024 splitkb.com (support@splitkb.com)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

// Unicode map indices
enum unicode_names {
    SV_AO_LOWER, // å
    SV_AO_UPPER, // Å
    SV_AE_LOWER, // ä
    SV_AE_UPPER, // Ä
    SV_OE_LOWER, // ö
    SV_OE_UPPER, // Ö
};

const uint32_t PROGMEM unicode_map[] = {
    [SV_AO_LOWER] = 0x00E5, // å
    [SV_AO_UPPER] = 0x00C5, // Å
    [SV_AE_LOWER] = 0x00E4, // ä
    [SV_AE_UPPER] = 0x00C4, // Ä
    [SV_OE_LOWER] = 0x00F6, // ö
    [SV_OE_UPPER] = 0x00D6, // Ö
};

// Custom keycodes for Swedish characters
enum custom_keycodes {
    SV_AO = SAFE_RANGE,
    SV_AE,
    SV_OE,
};

// Tap for the printed key, hold for the alternate. Layer 0 is the base layer, so
// the hold never actually switches layer -- it only borrows QMK's tap-hold engine
// so the timing matches the surrounding home-row mods.
#define TH(kc) LT(0, (kc))

static uint16_t tap_hold_alt(uint16_t keycode) {
    switch (keycode) {
        // Both index fingers double as Escape.
        case TH(KC_D):
        case TH(KC_H):    return KC_ESC;
        case TH(KC_SCLN): return KC_COLN;
        // Symbol layer
        case TH(KC_MINS): return KC_LABK; // <
        case TH(KC_EQL):  return KC_RABK; // >
        // Number layer
        case TH(KC_1):    return KC_EXLM;
        case TH(KC_2):    return KC_AT;
        case TH(KC_3):    return KC_HASH;
        case TH(KC_4):    return KC_DLR;
        case TH(KC_5):    return KC_PERC;
        case TH(KC_6):    return KC_CIRC;
        case TH(KC_7):    return KC_AMPR;
        case TH(KC_8):    return KC_ASTR;
        case TH(KC_9):    return KC_BSLS; // not "(" -- matches cradio.keymap
        default:          return KC_NO;
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    uint16_t hold_kc = tap_hold_alt(keycode);
    if (hold_kc != KC_NO) {
        if (!record->tap.count && record->event.pressed) {
            tap_code16(hold_kc);
            return false;
        }
        return true;
    }

    switch (keycode) {
        case SV_AO:
            if (record->event.pressed) {
                if (get_mods() & MOD_MASK_SHIFT) {
                    register_unicode(unicode_map[SV_AO_UPPER]);
                } else {
                    register_unicode(unicode_map[SV_AO_LOWER]);
                }
            }
            return false;
        case SV_AE:
            if (record->event.pressed) {
                if (get_mods() & MOD_MASK_SHIFT) {
                    register_unicode(unicode_map[SV_AE_UPPER]);
                } else {
                    register_unicode(unicode_map[SV_AE_LOWER]);
                }
            }
            return false;
        case SV_OE:
            if (record->event.pressed) {
                if (get_mods() & MOD_MASK_SHIFT) {
                    register_unicode(unicode_map[SV_OE_UPPER]);
                } else {
                    register_unicode(unicode_map[SV_OE_LOWER]);
                }
            }
            return false;
    }
    return true;
}

enum layers {
    _DEFAULT = 0,
    _SYMBOL,
    _NUMBER,
    _TRI,
    _MOUSE,
};

// ZMK conditional_layers: if-layers <1 2> then-layer <3>. Holding A and Z together
// reaches the Tri layer, the same way hold-Q does.
layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, _SYMBOL, _NUMBER, _TRI);
}

// The Halcyon module buttons are not part of this layout any more.
// They live in halcyon_keys.c as left_halcyon_buttons / right_halcyon_buttons.

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * Default Layer: Colemak-DH
     *
     * ,-----------------------------------.  ,-----------------------------------.
     * | Q/Tri|   W  | F/Alt|   P  |   G   |  |   J  |   L  |   U  |   Y  | ; / :|
     * |------+------+------+------+-------|  |------+------+------+------+-------|
     * | A/Sym| R/Sft| S/Gui| T/Ctl| D/Esc |  | H/Esc| N/Ctl| E/Gui| I/Sft| O/Sym|
     * |------+------+------+------+-------|  |------+------+------+------+-------|
     * | Z/Num|   X  |   C  |   V  |   B   |  |   K  |   M  |  , < |  . > | / /Nm|
     * `------+------+------+------+-------'  `------+------+------+------+------'
     *                      | Space| Bksp |  |Tb/Mse| Enter|
     *                      `-------------'  `-------------'
     */
    [_DEFAULT] = LAYOUT(
        LT(3,KC_Q) , KC_W        , LALT_T(KC_F), KC_P        , KC_G        , KC_J        , KC_L        , KC_U        , KC_Y        , TH(KC_SCLN)  ,
        LT(1,KC_A) , LSFT_T(KC_R), LGUI_T(KC_S), LCTL_T(KC_T), TH(KC_D)    , TH(KC_H)    , LCTL_T(KC_N), LGUI_T(KC_E), LSFT_T(KC_I), LT(1,KC_O)   ,
        LT(2,KC_Z) , KC_X        , KC_C        , KC_V        , KC_B        , KC_K        , KC_M        , KC_COMM     , KC_DOT      , LT(2,KC_SLSH),
                                                 KC_SPC      , KC_BSPC     , LT(4,KC_TAB), KC_ENT
    ),

    /*
     * Symbol Layer
     *
     * ,-----------------------------------.  ,-----------------------------------.
     * |  Tab |   +  |   {  |   }  |   |   |  | Down | Right|   !  |   "  |  ' " |
     * |------+------+------+------+-------|  |------+------+------+------+-------|
     * |      |   ~  |   (  |   )  |   `   |  | Left |   å  |   ä  |   ö  |      |
     * |------+------+------+------+-------|  |------+------+------+------+-------|
     * |      |   \  |   [  |   ]  |   \   |  |  Up  |   _  | - / <| = / >|   ?  |
     * `------+------+------+------+-------'  `------+------+------+------+------'
     *                      |      |  Del  |  |G+Spc |CpWrd |
     *                      `-------------'  `-------------'
     */
    [_SYMBOL] = LAYOUT(
        KC_TAB  , KC_PLUS , KC_LCBR , KC_RCBR     , KC_PIPE ,    KC_DOWN     , KC_RGHT     , KC_EXLM     , KC_DQUO , KC_QUOT ,
        KC_TRNS , KC_TILD , KC_LPRN , KC_RPRN     , KC_GRV  ,    KC_LEFT     , SV_AO       , SV_AE       , SV_OE   , KC_TRNS ,
        KC_TRNS , KC_BSLS , KC_LBRC , KC_RBRC     , KC_BSLS ,    KC_UP       , KC_UNDS     , TH(KC_MINS) , TH(KC_EQL) , KC_QUES ,
                                      KC_TRNS     , KC_DEL  ,    LGUI(KC_SPC), CW_TOGG
    ),

    /*
     * Number Layer
     *
     * ,-----------------------------------.  ,-----------------------------------.
     * |      |      |      |      |       |  | 7 / &| 8 / *| 9 / \| Home |  End |
     * |------+------+------+------+-------|  |------+------+------+------+-------|
     * |      | Shift|Alt/Gu|  Ctl |       |  | 4 / $| 5 / %| 6 / ^|      |      |
     * |------+------+------+------+-------|  |------+------+------+------+-------|
     * |      |      |      |      |       |  | 1 / !| 2 / @| 3 / #|      |      |
     * `------+------+------+------+-------'  `------+------+------+------+------'
     *                      | G+Z  | G+Spc|  | G+Tab|   0  |
     *                      `-------------'  `-------------'
     */
    [_NUMBER] = LAYOUT(
        KC_TRNS , KC_TRNS , KC_TRNS          , KC_TRNS     , KC_TRNS ,    TH(KC_7)    , TH(KC_8) , TH(KC_9) , KC_HOME , KC_END  ,
        KC_TRNS , KC_LSFT , LGUI_T(KC_LALT)  , KC_LCTL     , KC_TRNS ,    TH(KC_4)    , TH(KC_5) , TH(KC_6) , KC_TRNS , KC_TRNS ,
        KC_TRNS , KC_TRNS , KC_TRNS          , KC_TRNS     , KC_TRNS ,    TH(KC_1)    , TH(KC_2) , TH(KC_3) , KC_TRNS , KC_TRNS ,
                                               LGUI(KC_Z)  , LGUI(KC_SPC), LGUI(KC_TAB), KC_0
    ),

    /*
     * Tri Layer (Q held, or Sym + Num together)
     *
     * ,-----------------------------------.  ,-----------------------------------.
     * |      |      |      |      |       |  | G+7  | G+8  | G+9  |      |      |
     * |------+------+------+------+-------|  |------+------+------+------+-------|
     * |      |      |      |      |       |  | G+4  | G+5  | G+6  |      |      |
     * |------+------+------+------+-------|  |------+------+------+------+-------|
     * |      |      |      |      |       |  | G+1  | G+2  | G+3  |      |      |
     * `------+------+------+------+-------'  `------+------+------+------+------'
     *                      | Play | Next |  |      | G+0  |
     *                      `-------------'  `-------------'
     */
    [_TRI] = LAYOUT(
        KC_TRNS , KC_NO   , KC_NO   , KC_TRNS , KC_NO   ,    LGUI(KC_7) , LGUI(KC_8), LGUI(KC_9), KC_TRNS , KC_TRNS ,
        KC_TRNS , KC_TRNS , KC_TRNS , KC_TRNS , KC_NO   ,    LGUI(KC_4) , LGUI(KC_5), LGUI(KC_6), KC_TRNS , KC_TRNS ,
        KC_TRNS , KC_TRNS , KC_TRNS , KC_NO   , KC_NO   ,    LGUI(KC_1) , LGUI(KC_2), LGUI(KC_3), KC_TRNS , KC_TRNS ,
                                      KC_MPLY , KC_MNXT ,    KC_TRNS    , LGUI(KC_0)
    ),

    /*
     * Mouse Layer
     *
     * ,-----------------------------------.  ,-----------------------------------.
     * | Boot |      |      |      |       |  | MsDn |MsRght| WhlUp|      |      |
     * |------+------+------+------+-------|  |------+------+------+------+-------|
     * |      |      | Btn2 | Btn1 |       |  | MsLft|      | WhlDn|      |      |
     * |------+------+------+------+-------|  |------+------+------+------+-------|
     * |      |      |      |      |       |  | MsUp |      |      |      |      |
     * `------+------+------+------+-------'  `------+------+------+------+------'
     *                      |      |      |  |      |      |
     *                      `-------------'  `-------------'
     */
    [_MOUSE] = LAYOUT(
        QK_BOOT , KC_TRNS , KC_TRNS , KC_TRNS , KC_TRNS ,    MS_DOWN , MS_RGHT , MS_WHLU , KC_TRNS , KC_TRNS ,
        KC_TRNS , KC_TRNS , MS_BTN2 , MS_BTN1 , KC_TRNS ,    MS_LEFT , KC_TRNS , MS_WHLD , KC_TRNS , KC_TRNS ,
        KC_TRNS , KC_TRNS , KC_TRNS , KC_TRNS , KC_TRNS ,    MS_UP   , KC_TRNS , KC_TRNS , KC_TRNS , KC_TRNS ,
                                      KC_TRNS , KC_TRNS ,    KC_TRNS , KC_TRNS
    ),
};
// clang-format on

#if defined(ENCODER_MAP_ENABLE)
// Encoder 0 is the left half, which carries the TFT display rather than an
// encoder module -- those entries never fire. Encoder 1 is the real one.
// A wheel click is roughly three lines, where KC_PGDN was a whole viewport.
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_DEFAULT] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(MS_WHLU, MS_WHLD) },
    [_SYMBOL]  = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(MS_WHLU, MS_WHLD) },
    [_NUMBER]  = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(MS_WHLU, MS_WHLD) },
    [_TRI]     = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(MS_WHLU, MS_WHLD) },
    [_MOUSE]   = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(MS_WHLU, MS_WHLD) },
};
#endif
