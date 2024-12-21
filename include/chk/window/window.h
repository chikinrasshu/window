#pragma once

#include <chk/common/status_code.h>
#include <chk/common/types.h>

#include <chk/window/visibility.h>

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
    u32 running    : 1;
    u32 visible    : 1;
    u32 focused    : 1;
    u32 minimized  : 1;
    u32 maximized  : 1;
    u32 fullscreen : 1;
} chk_window_state_t;

typedef struct chk_window_changed {
    u32 pos     : 1;
    u32 size    : 1;
    u32 fb_size : 1;
} chk_window_changed_t;

typedef struct chk_window {
    chk_window_data_t    data;
    chk_window_state_t   is;
    chk_window_changed_t changed;

    void *impl;
} chk_window_t;

CHK_WINDOW_API chk_status_code_t chk_window_create(chk_window_t *window, s32 w,
                                                   s32 h, const char *caption);
CHK_WINDOW_API chk_status_code_t chk_window_destroy(chk_window_t *window);
CHK_WINDOW_API chk_status_code_t chk_window_run(chk_window_t *window);
