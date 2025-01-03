#include <OpenGL/OpenGL.h>
#include <chk/window/window.h>

#include <chk/common/logging.h>
#include <chk/common/status_code.h>

#include <epoxy/gl.h>
// epoxy/gl.h must be included before glfw3.h
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

static u32 g_chk_window_count = 0;

#pragma region CallbacksFwd

CHK_WINDOW_LOCAL void chk_window_cb_on_error(s32 code, const char *msg);
CHK_WINDOW_LOCAL void chk_window_cb_on_close(GLFWwindow *handle);
CHK_WINDOW_LOCAL void chk_window_cb_on_frame(GLFWwindow *handle);
CHK_WINDOW_LOCAL void chk_window_cb_on_pos(GLFWwindow *handle, s32 x, s32 y);
CHK_WINDOW_LOCAL void chk_window_cb_on_size(GLFWwindow *handle, s32 w, s32 h);
CHK_WINDOW_LOCAL void chk_window_cb_on_fb_size(GLFWwindow *handle, s32 w, s32 h);
CHK_WINDOW_LOCAL void chk_window_cb_on_dpi(GLFWwindow *handle, f32 x, f32 y);
CHK_WINDOW_LOCAL void chk_window_cb_on_focus(GLFWwindow *handle, s32 v);
CHK_WINDOW_LOCAL void chk_window_cb_on_hover(GLFWwindow *handle, s32 v);

#pragma endregion CallbacksFwd

#pragma region WindowImpl

CHK_WINDOW_API chk_status_code_t chk_window_create(chk_window_t *window, s32 w, s32 h, const char *caption) {
    if (!window) { return CHK_STATUS_BAD_PTR; }
    if (window->impl) { return CHK_STATUS_ALREADY_INITIALIZED; }

    *window = (chk_window_t){0};

    if (!g_chk_window_count) {
        // Initialize windowing system
        glfwSetErrorCallback(chk_window_cb_on_error);
        if (!glfwInit()) { return CHK_STATUS_FAILURE; }
        chk_info("Initialized GLFW");
    }
    ++g_chk_window_count;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    // Create window implementation
    window->impl = glfwCreateWindow(w, h, caption, NULL, NULL);
    if (!window->impl) { return CHK_STATUS_FAILURE; }
    chk_info("Created main window '%s' @ %dx%d", caption, w, h);

    glfwMakeContextCurrent(window->impl);
    glEnable(GL_BLEND);
    chk_info("Initialized OpenGL %d.%d", epoxy_gl_version());

    glfwSetWindowUserPointer(window->impl, window);
    glfwSetWindowCloseCallback(window->impl, chk_window_cb_on_close);
    glfwSetWindowRefreshCallback(window->impl, chk_window_cb_on_frame);
    glfwSetWindowPosCallback(window->impl, chk_window_cb_on_pos);
    glfwSetWindowSizeCallback(window->impl, chk_window_cb_on_size);
    glfwSetFramebufferSizeCallback(window->impl, chk_window_cb_on_fb_size);
    glfwSetWindowContentScaleCallback(window->impl, chk_window_cb_on_dpi);
    glfwSetWindowFocusCallback(window->impl, chk_window_cb_on_focus);
    glfwSetCursorEnterCallback(window->impl, chk_window_cb_on_hover);

    // Set default values
    window->data.w    = w;
    window->data.h    = h;
    window->data.fb_w = w;
    window->data.fb_h = h;
    window->data.dpi  = 1.0f;

    glfwGetWindowPos(window->impl, &window->data.x, &window->data.y);
    glfwGetWindowSize(window->impl, &window->data.w, &window->data.h);
    glfwGetFramebufferSize(window->impl, &window->data.fb_w, &window->data.fb_h);
    glfwGetWindowContentScale(window->impl, &window->data.dpi, NULL);

    window->is.visible = true;
    glfwShowWindow(window->impl);
    chk_info("Opened window '%s'", caption);

    window->is.running = true;
    return CHK_STATUS_SUCCESS;
}

CHK_WINDOW_API chk_status_code_t chk_window_destroy(chk_window_t *window) {
    if (!window) { return CHK_STATUS_BAD_PTR; }

    if (window->impl) {
        // Destroy window implementation
        glfwDestroyWindow(window->impl);
        chk_info("Destroyed window");
    }

    *window = (chk_window_t){0};

    --g_chk_window_count;
    if (!g_chk_window_count) {
        // Terminate windowing system
        glfwTerminate();
    }

    return CHK_STATUS_SUCCESS;
}

CHK_WINDOW_LOCAL chk_status_code_t chk_window_step(chk_window_t *window, bool process_events) {
    if (!window) { return CHK_STATUS_BAD_PTR; }
    if (!window->impl) { return CHK_STATUS_NOT_INITIALIZED; }
    chk_status_code_t result = CHK_STATUS_FAILURE;

    if (window->is.running) {
        glfwMakeContextCurrent(window->impl);

        // Begin frame
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Run frame callbacks
        // chk_trace("Running window frame");
        if (window->callbacks.on_frame) { window->callbacks.on_frame(window, window->callbacks.user_ptr); }

        if (process_events) {
            // chk_trace("Polling window events");
            glfwPollEvents();
        }

        // chk_trace("Swapping window buffers");
        glfwSwapBuffers(window->impl);

        // Reset window changes and calculate delta time
        window->changed = (chk_window_changed_t){0};

        f64 current_time        = glfwGetTime();
        window->data.delta_time = current_time - window->data.last_time;
        window->data.last_time  = current_time;

        result = CHK_STATUS_SUCCESS;
    }
    return result;
}

CHK_WINDOW_API chk_status_code_t chk_window_run(chk_window_t *window) {
    if (!window) { return CHK_STATUS_BAD_PTR; }
    if (!window->impl) { return CHK_STATUS_NOT_INITIALIZED; }

    chk_status_code_t result = CHK_STATUS_FAILURE;

    chk_dbg("Opening window");
    if (window->callbacks.on_open) { window->callbacks.on_open(window, window->callbacks.user_ptr); }

    chk_dbg("Running window");
    while (window->is.running) { result = chk_window_step(window, true); }

    chk_dbg("Closing window");
    if (window->callbacks.on_close) { window->callbacks.on_close(window, window->callbacks.user_ptr); }

    return result;
}

#pragma endregion WindowImpl

#pragma region CallbacksImpl

CHK_WINDOW_LOCAL void chk_window_cb_on_error(s32 code, const char *msg) { chk_error("GLFW(%d) | %s", code, msg); }

CHK_WINDOW_LOCAL void chk_window_cb_on_close(GLFWwindow *handle) {
    chk_window_t *window = glfwGetWindowUserPointer(handle);
    if (window) { window->is.running = false; }
}

CHK_WINDOW_LOCAL void chk_window_cb_on_frame(GLFWwindow *handle) {
    chk_window_t *window = glfwGetWindowUserPointer(handle);
    if (window) { chk_window_step(window, false); }
}

CHK_WINDOW_LOCAL void chk_window_cb_on_pos(GLFWwindow *handle, s32 x, s32 y) {
    chk_window_t *window = glfwGetWindowUserPointer(handle);
    if (window) {
        window->data.x      = x;
        window->data.y      = y;
        window->changed.pos = true;

        if (window->callbacks.on_pos) { window->callbacks.on_pos(window, window->callbacks.user_ptr); }
    }
}

CHK_WINDOW_LOCAL void chk_window_cb_on_size(GLFWwindow *handle, s32 w, s32 h) {
    chk_window_t *window = glfwGetWindowUserPointer(handle);
    if (window) {
        window->data.w       = w;
        window->data.h       = h;
        window->changed.size = true;

        if (window->callbacks.on_size) { window->callbacks.on_size(window, window->callbacks.user_ptr); }
    }
}

CHK_WINDOW_LOCAL void chk_window_cb_on_fb_size(GLFWwindow *handle, s32 w, s32 h) {
    chk_window_t *window = glfwGetWindowUserPointer(handle);
    if (window) {
        window->data.fb_w       = w;
        window->data.fb_h       = h;
        window->changed.fb_size = true;

        if (window->callbacks.on_fb_size) { window->callbacks.on_fb_size(window, window->callbacks.user_ptr); }
    }
}

CHK_WINDOW_LOCAL void chk_window_cb_on_dpi(GLFWwindow *handle, f32 x, f32 y) {
    chk_window_t *window = glfwGetWindowUserPointer(handle);
    if (window) {
        window->data.dpi    = (x + y) * 0.5f;
        window->changed.dpi = true;

        if (window->callbacks.on_dpi) { window->callbacks.on_dpi(window, window->callbacks.user_ptr); }
    }
}

CHK_WINDOW_LOCAL void chk_window_cb_on_focus(GLFWwindow *handle, s32 v) {
    chk_window_t *window = glfwGetWindowUserPointer(handle);
    if (window) {
        window->is.focused    = (!!v);
        window->changed.focus = true;

        if (window->callbacks.on_focus) { window->callbacks.on_focus(window, window->callbacks.user_ptr); }
    }
}

CHK_WINDOW_LOCAL void chk_window_cb_on_hover(GLFWwindow *handle, s32 v) {
    chk_window_t *window = glfwGetWindowUserPointer(handle);
    if (window) {
        window->is.hovered    = (!!v);
        window->changed.hover = true;

        if (window->callbacks.on_hover) { window->callbacks.on_hover(window, window->callbacks.user_ptr); }
    }
}

#pragma endregion CallbacksImpl