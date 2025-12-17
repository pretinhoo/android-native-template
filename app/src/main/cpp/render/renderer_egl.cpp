#include "renderer_egl.h"
#include "../platform/log.h"

#include <EGL/egl.h>
#include <GLES2/gl2.h>

bool egl_init(EGLState& s, ANativeWindow* window) {
    s.display = EGL_NO_DISPLAY;
    s.surface = EGL_NO_SURFACE;
    s.context = EGL_NO_CONTEXT;
    s.width = 0;
    s.height = 0;

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (display == EGL_NO_DISPLAY) return false;

    if (!eglInitialize(display, nullptr, nullptr)) return false;

    const EGLint configAttribs[] = {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_SURFACE_TYPE,    EGL_WINDOW_BIT,
            EGL_RED_SIZE,        8,
            EGL_GREEN_SIZE,      8,
            EGL_BLUE_SIZE,       8,
            EGL_ALPHA_SIZE,      8,
            EGL_DEPTH_SIZE,      16,
            EGL_NONE
    };

    EGLConfig config;
    EGLint numConfigs;
    if (!eglChooseConfig(display, configAttribs, &config, 1, &numConfigs) || numConfigs < 1) {
        LOGE("eglChooseConfig failed");
        eglTerminate(display);
        return false;
    }

    const EGLint ctxAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
    EGLContext context = eglCreateContext(display, config, EGL_NO_CONTEXT, ctxAttribs);
    if (context == EGL_NO_CONTEXT) {
        LOGE("eglCreateContext failed");
        eglTerminate(display);
        return false;
    }

    EGLSurface surface = eglCreateWindowSurface(display, config, window, nullptr);
    if (surface == EGL_NO_SURFACE) {
        LOGE("eglCreateWindowSurface failed");
        eglDestroyContext(display, context);
        eglTerminate(display);
        return false;
    }

    if (!eglMakeCurrent(display, surface, surface, context)) {
        LOGE("eglMakeCurrent failed");
        eglDestroySurface(display, surface);
        eglDestroyContext(display, context);
        eglTerminate(display);
        return false;
    }

    EGLint w, h;
    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);

    glViewport(0, 0, w, h);

    s.display = display;
    s.surface = surface;
    s.context = context;
    s.width = (int)w;
    s.height = (int)h;

    LOGI("EGL init OK: %dx%d", s.width, s.height);
    return true;
}

void egl_term(EGLState& s) {
    EGLDisplay display = (EGLDisplay)s.display;
    EGLSurface surface = (EGLSurface)s.surface;
    EGLContext context = (EGLContext)s.context;

    if (display == EGL_NO_DISPLAY) return;

    eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

    if (context != EGL_NO_CONTEXT) eglDestroyContext(display, context);
    if (surface != EGL_NO_SURFACE) eglDestroySurface(display, surface);

    eglTerminate(display);

    s.display = EGL_NO_DISPLAY;
    s.surface = EGL_NO_SURFACE;
    s.context = EGL_NO_CONTEXT;
    s.width = 0;
    s.height = 0;
}

void egl_draw(EGLState& s, float r, float g, float b) {
    EGLDisplay display = (EGLDisplay)s.display;
    EGLSurface surface = (EGLSurface)s.surface;

    if (display == EGL_NO_DISPLAY || surface == EGL_NO_SURFACE) return;

    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    eglSwapBuffers(display, surface);
}
