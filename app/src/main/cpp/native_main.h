#include <android/log.h>
#include <android_native_app_glue.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "blacksheep_native", __VA_ARGS__))

static void handle_cmd(struct android_app* app, int32_t cmd) {
    switch (cmd) {
        case APP_CMD_START:
            LOGI("APP_CMD_START");
            break;
        case APP_CMD_INIT_WINDOW:
            LOGI("APP_CMD_INIT_WINDOW (window ready)");
            break;
        default:
            break;
    }
}

void android_main(struct android_app* app) {
    app->onAppCmd = handle_cmd;

    LOGI("android_main() started");

    // Bucle principal
    int events;
    android_poll_source* source;

    while (true) {
        while (ALooper_pollAll(0, nullptr, &events, (void**)&source) >= 0) {
            if (source) source->process(app, source);
            if (app->destroyRequested) {
                LOGI("destroyRequested -> exiting");
                return;
            }
        }

        // Aquí iría tu lógica/renderer (OpenGL/Vulkan/etc.)
    }
}
