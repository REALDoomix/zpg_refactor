/*
 * Copyright (c) 2026 Martin Nemec
 *
 * File: main.cpp
 * Description: OpenGL application entry point.
 */

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#undef GLAD_GL_IMPLEMENTATION

#include "Application.h"

int main() {
    Application app;
    app.initialization();
    app.run();
    return 0;
}
