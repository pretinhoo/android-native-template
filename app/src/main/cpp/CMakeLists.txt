cmake_minimum_required(VERSION 3.22.1)
project(hello)

add_library(hello SHARED
        native_main.cpp
        core/time_utils.cpp
        platform/input.cpp
        render/renderer_egl.cpp
        ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c
)

target_include_directories(hello PRIVATE
        ${ANDROID_NDK}/sources/android/native_app_glue
)

# Clave para exportar ANativeActivity_onCreate desde native_app_glue
target_compile_definitions(hello PRIVATE ANDROID_NATIVE_APP_GLUE)

target_link_libraries(hello
        android
        log
        EGL
        GLESv2
)