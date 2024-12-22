#include "chk/common/status_code.h"
#include <chk/window/window.h>

void on_open(chk_window_t *window, void *user_ptr) { (void)user_ptr; }
void on_close(chk_window_t *window, void *user_ptr) { (void)user_ptr; }
void on_frame(chk_window_t *window, void *user_ptr) { (void)user_ptr; }

int main(int argc, char *argv[]) {
    chk_status_code_t result = CHK_STATUS_FAILURE;

    chk_window_t window = {0};
    if (chk_window_create(&window, 800, 600, "Test Window") == CHK_STATUS_SUCCESS) {
        window.callbacks.on_open  = on_open;
        window.callbacks.on_close = on_close;
        window.callbacks.on_frame = on_frame;

        result = chk_window_run(&window);
        chk_window_destroy(&window);
    }

    return result == CHK_STATUS_SUCCESS ? 0 : 1;
}