#pragma once
#include <android/native_window.h>

struct EGLState {
    void* display;
    void* surface;
    void* context;
    int width;
    int height;
};

bool egl_init(EGLState& s, ANativeWindow* window);
void egl_term(EGLState& s);
void egl_draw(EGLState& s, float r, float g, float b);
