cmake_minimum_required(VERSION 3.22.1)
project(blacksheep)

add_library(blacksheep SHARED
        native_main.cpp
        core/time_utils.cpp
        platform/input.cpp
        platform/fs.cpp
        platform/proc_maps.cpp
        render/renderer_egl.cpp
        ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c
)

target_include_directories(blacksheep PRIVATE
        ${ANDROID_NDK}/sources/android/native_app_glue
)

# Clave para exportar ANativeActivity_onCreate desde native_app_glue
target_compile_definitions(blacksheep PRIVATE ANDROID_NATIVE_APP_GLUE)

target_link_libraries(blacksheep
        android
        log
        EGL
        GLESv2
)