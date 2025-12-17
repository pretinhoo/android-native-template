#include "input.h"
#include "../core/app_state.h"
#include "log.h"

static void cycle_color(AppState& st) {
    // Ciclo simple entre 3 colores (útil para confirmar input)
    if (st.clear_r > 0.5f) {         // rojo -> verde
        st.clear_r = 0.10f; st.clear_g = 0.70f; st.clear_b = 0.20f;
    } else if (st.clear_g > 0.5f) {  // verde -> azul
        st.clear_r = 0.10f; st.clear_g = 0.20f; st.clear_b = 0.80f;
    } else {                         // azul -> rojo
        st.clear_r = 0.80f; st.clear_g = 0.15f; st.clear_b = 0.15f;
    }
}

bool handle_input_event(AppState& st, AInputEvent* event) {
    const int32_t type = AInputEvent_getType(event);

    if (type == AINPUT_EVENT_TYPE_MOTION) {
        const int32_t action = AMotionEvent_getAction(event) & AMOTION_EVENT_ACTION_MASK;

        if (action == AMOTION_EVENT_ACTION_DOWN) {
            const float x = AMotionEvent_getX(event, 0);
            const float y = AMotionEvent_getY(event, 0);

            LOGI("Touch DOWN at (%.1f, %.1f) -> change color", x, y);
            cycle_color(st);
            return true; // consumido
        }
    }
    return false;
}
