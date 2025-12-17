#pragma once
#include <android/input.h>

struct AppState;

bool handle_input_event(AppState& st, AInputEvent* event);
