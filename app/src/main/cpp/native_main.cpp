#include <android_native_app_glue.h>
#include <unistd.h> // usleep

#include "platform/log.h"
#include "platform/input.h"
#include "core/app_state.h"
#include "core/time_utils.h"
#include "render/renderer_egl.h"

static EGLState* gEgl = nullptr;
static AppState  gState;

static int32_t on_input(android_app* app, AInputEvent* event) {
    (void)app;
    return handle_input_event(gState, event) ? 1 : 0;
}

static void handle_cmd(android_app* app, int32_t cmd) {
    switch (cmd) {
        case APP_CMD_START:
            LOGI("APP_CMD_START");
            break;

        case APP_CMD_RESUME:
            LOGI("APP_CMD_RESUME");
            break;

        case APP_CMD_PAUSE:
            LOGI("APP_CMD_PAUSE");
            break;

        case APP_CMD_INIT_WINDOW:
            LOGI("APP_CMD_INIT_WINDOW (window ready)");
            if (app->window && !gEgl) {
                gEgl = new EGLState();
                if (!egl_init(*gEgl, app->window)) {
                    LOGE("EGL init failed");
                    delete gEgl;
                    gEgl = nullptr;
                }
            }
            break;

        case APP_CMD_TERM_WINDOW:
            LOGI("APP_CMD_TERM_WINDOW");
            if (gEgl) {
                egl_term(*gEgl);
                delete gEgl;
                gEgl = nullptr;
            }
            break;

        default:
            break;
    }
}

void android_main(android_app* app) {
    app_dummy();

    LOGI("android_main() started");

    app->onAppCmd = handle_cmd;
    app->onInputEvent = on_input;

    gState.last_fps_ts = now_seconds();

    int events;
    android_poll_source* source;

    while (true) {
        while (true) {
            // Procesa eventos del sistema (no bloqueante: timeout=0)
            while (ALooper_pollOnce(0, nullptr, &events, (void**)&source) >= 0) {
                if (source) source->process(app, source);

                if (app->destroyRequested) {
                    LOGI("destroyRequested -> exiting");
                    return;
                }
            }

            if (gEgl) {
                egl_draw(*gEgl, gState.clear_r, gState.clear_g, gState.clear_b);

                gState.frames++;
                const double t = now_seconds();
                const double dt = t - gState.last_fps_ts;
                if (dt >= 1.0) {
                    const double fps = (double)gState.frames / dt;
                    LOGI("FPS: %.1f", fps);
                    gState.frames = 0;
                    gState.last_fps_ts = t;
                }
            }

            usleep(16000);
        }


        usleep(16000); // ~60 fps, sin quemar CPU
    }
}