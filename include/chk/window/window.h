#pragma once

#include <chk/common/status_code.h>
#include <chk/common/types.h>

#include <chk/window/visibility.h>

typedef struct chk_window chk_window_t;
typedef void (*chk_window_cb_t)(chk_window_t *window, void *user_ptr);

typedef struct chk_window_data {
    s32 x, y;
    s32 w, h;
    s32 fb_w, fb_h;
    f32 dpi;

    f64 delta_time;
    f64 last_time;
    f64 elapsed_time;
} chk_window_data_t;

typedef struct chk_window_state {
    u8 running    : 1;
    u8 visible    : 1;
    u8 focused    : 1;
    u8 hovered    : 1;
    u8 minimized  : 1;
    u8 maximized  : 1;
    u8 fullscreen : 1;
} chk_window_state_t;

typedef struct chk_window_changed {
    u8 pos        : 1;
    u8 size       : 1;
    u8 fb_size    : 1;
    u8 dpi        : 1;
    u8 focus      : 1;
    u8 hover      : 1;
    u8 minmax     : 1;
    u8 fullscreen : 1;
} chk_window_changed_t;

typedef struct chk_window_callbacks {
    void *user_ptr;

    chk_window_cb_t on_open;
    chk_window_cb_t on_close;
    chk_window_cb_t on_frame;
    chk_window_cb_t on_pos;
    chk_window_cb_t on_size;
    chk_window_cb_t on_fb_size;
    chk_window_cb_t on_dpi;
    chk_window_cb_t on_focus;
    chk_window_cb_t on_hover;
} chk_window_callbacks_t;

typedef struct chk_window {
    chk_window_data_t      data;
    chk_window_state_t     is;
    chk_window_changed_t   changed;
    chk_window_callbacks_t callbacks;

    void *impl;
} chk_window_t;

CHK_WINDOW_API chk_status_code_t chk_window_create(chk_window_t *window, s32 w,
                                                   s32 h, const char *caption);
CHK_WINDOW_API chk_status_code_t chk_window_destroy(chk_window_t *window);
CHK_WINDOW_API chk_status_code_t chk_window_run(chk_window_t *window);
