#pragma once
#include "matrix.hpp"
#include "perspective.hpp"
#include <math.h>
#ifndef M_PI
	#include <corecrt_math_defines.h>
#endif

#define LIGHT_COUNT(lights) (sizeof(lights) / sizeof(lights[0]))

struct PointLight {
    double pos[3];
    double col[3];
    double intensity;
};

Matrix center(const Matrix &points) {
    Matrix center(1, 3);
    center.clear();
    for(unsigned int i = 0; i < points.height; i++) {
        for(unsigned int j = 0; j < 3; j++) {
            center[0][j] += points[i][j];
        }
    }
    Matrix::inPlaceScale(center, 1.0 / (double)points.height, center);
    return center;
}

Matrix calcPointLighting(Matrix &calcCenter, Matrix normal, const PointLight *lights, unsigned int light_count) {
    Matrix col(1, 3);
    col.clear();
    normalize3(normal);
    Matrix dir(1, 3);
    for(unsigned int i = 0; i < light_count; i++) {
        double dist2 = 0.0;
        for(unsigned int j = 0; j < 3; j++) {
            dist2 += (lights[i].pos[j] - calcCenter[0][j]) * (lights[i].pos[j] - calcCenter[0][j]);
            dir[0][j] = lights[i].pos[j] - calcCenter[0][j];
        }
        normalize3(dir);
        double d = dot(normal[0], dir[0]) / dist2;
        d = clamp(d, 0.0, 1.0);
        for(unsigned int j = 0; j < 3; j++) {
            col[0][j] += d * lights[i].col[j] * lights[i].intensity;
        }
    }
    return col;
}

