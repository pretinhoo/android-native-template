# android-native-template

Android NativeActivity template using **C/C++ only** (no Java/Kotlin application code).

This project is intended as a reusable base for:
- Native game development (EGL / OpenGL ES)
- Android reverse engineering & low-level experimentation
- Performance-critical native components

## Features

- NativeActivity (no Java/Kotlin app logic)
- EGL + OpenGL ES rendering (clear screen example)
- Native input handling (touch → color change)
- Main loop with FPS logging
- Clean modular C++ structure
- Gradle + CMake setup
- Initialization script (`init-template.sh`) for fast reuse

---

## Requirements

- Android Studio (Giraffe or newer recommended)
- Android NDK (r25+ / r26+ / r27 tested)
- Linux or macOS (Windows via WSL also works)

---

## Quick start

Clone the template:

```bash
git clone https://github.com/pretinhoo/android-native-template.git MyProject
cd MyProject
chmod +x init-template.sh
