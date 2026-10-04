/*
 * Copyright (c) 2026 Martin N?mec
 *
 * File: main.cpp
 * Description:  Fixed Function Pipeline.
 */

#define GLAD_GL_IMPLEMENTATION

#include "./Application.h"





int main() {
    Application app;
    app.initialization();
    app.run();
    return 0;
}