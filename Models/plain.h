#pragma once

/**
 * @file plain.h
 * @brief Plain model extended with UV coordinates (8 floats per vertex: position, normal, uv coordinates)
 */

const float plain[] = {
    // vrchol,          normála,            uv souřadnice
     1.0f, 0.0f,  1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
     1.0f, 0.0f, -1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
    -1.0f, 0.0f, -1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,

    -1.0f, 0.0f,  1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
     1.0f, 0.0f,  1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
    -1.0f, 0.0f, -1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f
};
