// Copyright 2024 splitkb.com (support@splitkb.com)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#define ENCODER_RESOLUTION 2

// Linux/ibus Unicode input: Ctrl+Shift+U, hex digits, confirm.
// Matches HOST_OS 1 in zmk-config/config/cradio.keymap, which selects
// OS_UNICODE_LEAD `&kp LS(LC(U))`.
#define UNICODE_SELECTED_MODES UNICODE_MODE_LINUX

// Solid yellow backlight
#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_SOLID_COLOR
#define RGB_MATRIX_DEFAULT_HUE 43
#define RGB_MATRIX_DEFAULT_SAT 255
#define RGB_MATRIX_DEFAULT_VAL 128

// Tap-hold timing, ported from the `ht` behavior in zmk-config/config/cradio.keymap
#define TAPPING_TERM 150   // ZMK tapping-term-ms
#define QUICK_TAP_TERM 140 // ZMK quick-tap-ms
#define FLOW_TAP_TERM 100  // ZMK require-prior-idle-ms
// QMK's default tap-hold resolution already matches ZMK's "tap-preferred" flavor,
// so PERMISSIVE_HOLD and HOLD_ON_OTHER_KEY_PRESS deliberately stay off.

// Faster pointer, approximating ZMK_POINTING_DEFAULT_MOVE_VAL 1000 (default 600)
// and ZMK_POINTING_DEFAULT_SCRL_VAL 20 (default 10)
#define MOUSEKEY_MAX_SPEED 16
#define MOUSEKEY_TIME_TO_MAX 25
#define MOUSEKEY_WHEEL_MAX_SPEED 16
#define MOUSEKEY_WHEEL_TIME_TO_MAX 30

// CONFIG_ZMK_KEYBOARD_NAME
#undef PRODUCT
#define PRODUCT "Simons Sweep"
