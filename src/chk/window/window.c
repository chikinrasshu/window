#include <chk/window/window.h>

static u32 g_chk_window_count = 0;

CHK_WINDOW_API chk_status_code_t chk_window_create(chk_window_t *window, s32 w,
                                                   s32 h, const char *caption) {
    if (!window) { return CHK_STATUS_BAD_PTR; }
    if (window->impl) { return CHK_STATUS_ALREADY_INITIALIZED; }

    if (!g_chk_window_count) {
        // Initialize windowing system
    }
    ++g_chk_window_count;

    // Create window implementation

    return CHK_STATUS_SUCCESS;
}

CHK_WINDOW_API chk_status_code_t chk_window_destroy(chk_window_t *window) {
    if (!window) { return CHK_STATUS_BAD_PTR; }

    if (window->impl) {
        // Destroy window implementation
    }

    *window = (chk_window_t){0};

    --g_chk_window_count;
    if (!g_chk_window_count) {
        // Terminate windowing system
    }

    return CHK_STATUS_SUCCESS;
}

CHK_WINDOW_LOCAL chk_status_code_t chk_window_step(chk_window_t *window,
                                                   bool process_events) {
    if (!window) { return CHK_STATUS_BAD_PTR; }
    if (!window->impl) { return CHK_STATUS_NOT_INITIALIZED; }

    if (process_events) {}
    return CHK_STATUS_SUCCESS;
}

CHK_WINDOW_API chk_status_code_t chk_window_run(chk_window_t *window) {
    if (!window) { return CHK_STATUS_BAD_PTR; }
    if (!window->impl) { return CHK_STATUS_NOT_INITIALIZED; }

    chk_status_code_t result = CHK_STATUS_FAILURE;

    while (window->is.running) { result = chk_window_step(window, true); }

    return result;
}
