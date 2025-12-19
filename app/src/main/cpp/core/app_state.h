#pragma once

struct AppState {
    // Color de fondo
    float clear_r = 0.10f;
    float clear_g = 0.20f;
    float clear_b = 0.60f;

    // FPS
    double last_fps_ts = 0.0;
    int frames = 0;

    // Reverse/dev actions
    bool request_dump_maps = false;

    // Input timing
    double last_tap_ts = 0.0;

};