#pragma once
#include <math.h>
#ifndef M_PI
	#include <corecrt_math_defines.h>
#endif
#include <cstdio>
#include <stdio.h>
#include <stdint.h>
#include "matrix.hpp"
#include "build.hpp"
#include "write.hpp"

#define MAX_LOGO_COMPONENT_COUNT (16)
#define MAX_LOGO_USE_COUNT (8)
#define LOGO_RAW_SIZE (30.0)
#define LOGO_POINT_COUNT(logo) ((1 + sizeof(logo) / sizeof((logo)[0])) / 2)
typedef const double LogoRaw[];
typedef const double *LogoRawP;
struct ShapeGroup {
    const char *name;
    unsigned int len;
    unsigned int pointCount[MAX_LOGO_COMPONENT_COUNT];
    LogoRawP shapes[MAX_LOGO_COMPONENT_COUNT];
    FILE *posFile[MAX_LOGO_USE_COUNT][MAX_LOGO_COMPONENT_COUNT];
    FILE *opaFile[MAX_LOGO_USE_COUNT];
};

void openShapeGroupFile(ShapeGroup &group, const char *suffix, unsigned int slot) {
    static char linkerNameBuffer[256];
    for (unsigned int i = 0; i < group.len; i++) {
        sprintf(linkerNameBuffer, "logo%s%s%d", group.name, suffix, i);
        group.posFile[slot][i] = newPolyFile(linkerNameBuffer);
        writePosHead(group.posFile[slot][i]);
    }
    sprintf(linkerNameBuffer, "logo%s%sopa", group.name, suffix);
    group.opaFile[slot] = newPolyFile(linkerNameBuffer);
    writeOpaHead(group.opaFile[slot]);
}

void closeShapeGroupFile(ShapeGroup &group, unsigned int slot) {
    for (unsigned int i = 0; i < group.len; i++) {
        writePosTail(group.posFile[slot][i]);
        fclose(group.posFile[slot][i]);
    }
    writeOpaTail(group.opaFile[slot]);
    fclose(group.opaFile[slot]);
}

Matrix logoRawToMatrix(LogoRaw logoRaw, unsigned int pointCount) {
    Matrix output(pointCount, 4);
    for (unsigned int i = 0; i < pointCount; i++) {
        output[i][2] = logoRaw[i * 2] / LOGO_RAW_SIZE - 0.5;
        output[i][0] = -logoRaw[i * 2 + 1] / LOGO_RAW_SIZE + 0.5;
        output[i][1] = 0;
        output[i][3] = 1;

        output[i][2] *= 2.0;
        output[i][0] *= 2.0;
    }
    return output;
}

LogoRaw logo_base_bg = {
    3,3,
    27,3,
    27,27,
    3,27,
};

LogoRaw logo_c_0 = {
    7,7,
    23,7,
    23,13,
    19,13,
    19,11,
    11,11,
    11,19,
    19,19,
    19,17,
    23,17,
    23,23,
    7,23,
};

LogoRaw logo_cpp_0 = {
    7,7,
    23,7,
    23,12,
    19,12,
    19,11,
    11,11,
    11,19,
    19,19,
    19,18,
    23,18,
    23,23,
    7,23
};

LogoRaw logo_cpp_1 = {
    20,14.5,
    21,14.5,
    21,13.5,
    22,13.5,
    22,14.5,
    23,14.5,
    23,13.5,
    24,13.5,
    24,14.5,
    25,14.5,
    25,15.5,
    24,15.5,
    24,16.5,
    23,16.5,
    23,15.5,
    22,15.5,
    22,16.5,
    21,16.5,
    21,15.5,
    20,15.5
};

LogoRaw logo_js_0 = {
    3,3,
    19,3,
    19,15,
    16,15,
    16,23,
    14,23,
    14,22.5,
    12,22.5,
    12,25,
    19,25,
    19,27,
    3,27
};

LogoRaw logo_js_1 = {
    18,3,
    27,3,
    27,27,
    18,27,
    18,25,
    25,25,
    25,19,
    21,19,
    21,17,
    25,17,
    25,15,
    18,15
};

LogoRaw logo_js_2 = {
    18,14,
    19,14,
    19,21,
    23,21,
    23,23,
    19,23,
    19,26,
    18,26
};

LogoRaw logo_html_0 = {
    8,6,
    15,6,
    15,9,
    11,9,
    11,12,
    15,12,
    15,15,
    8,15
};

LogoRaw logo_html_1 = {
    8,19,
    11,19,
    11,21,
    15,21,
    15,24,
    8,24
};

LogoRaw logo_html_2 = {
    15,6,
    22,6,
    22,9,
    15,9
};

LogoRaw logo_html_3 = {
    15,12,
    22,12,
    22,24,
    15,24,
    15,21,
    19,21,
    19,15,
    15,15
};

LogoRaw logo_css_0 = {
    8,6,
    15,6,
    15,9,
    8,9
};

LogoRaw logo_css_1 = {
    8,12,
    15,12,
    15,15,
    8,15
};

LogoRaw logo_css_2 = {
    8,19,
    11,19,
    11,21,
    15,21,
    15,24,
    8,24
};

LogoRaw logo_css_3 = {
    15,6,
    22,6,
    22,24,
    15,24,
    15,21,
    19,21,
    19,15,
    15,15,
    15,12,
    19,12,
    19,9,
    15,9
};

LogoRaw logo_rust_0 = {
    3,3,
    15,3,
    15,6,
    6,6,
    6,24,
    15,24,
    15,27,
    3,27
};

LogoRaw logo_rust_1 = {
    14,3,
    27,3,
    27,27,
    14,27,
    14,24,
    24,24,
    24,6,
    14,6
};

LogoRaw logo_rust_2 = {
    8,12,
    9.5,12,
    9.5,18,
    8,18
};

LogoRaw logo_rust_3 = {
    12,12,
    17,12,
    17,14,
    12,14
};

LogoRaw logo_rust_4 = {
    8,8,
    22,8,
    22,17,
    20.5,17,
    20.5,18,
    18.5,18,
    18.5,16,
    19.5,16,
    19.5,10,
    8,10
};

LogoRaw logo_rust_5 = {
    12,16,
    16,16,
    16,20,
    22,20,
    22,22,
    8,22,
    8,20,
    14,20,
    14,18,
    12,18
};

LogoRaw logo_go_0 = {
    22,15.5,
    23.5,15.5,
    23.5,18,
    22,18
};

LogoRaw logo_go_1 = {
    7,12,
    11,12,
    11,13,
    7,13
};

LogoRaw logo_go_2 = {
    5,14,
    11,14,
    11,15,
    5,15
};

LogoRaw logo_go_3 = {
    9,16,
    11,16,
    11,17,
    9,17
};

LogoRaw logo_go_4 = {
    12,11,
    19,11,
    19,13,
    14.5,13,
    14.5,18,
    17,18,
    17,16.5,
    16,16.5,
    16,15,
    19,15,
    19,20,
    12,20
};

LogoRaw logo_go_5 = {
    20,14,
    25,14,
    25,20,
    20,20
};

LogoRaw logo_octave_0 = {
    10,8,
    22,8,
    22,20,
    10,20
};

LogoRaw logo_octave_1 = {
    7,7,
    23,7,
    23,23,
    7,23
};

LogoRaw logo_octave_2 = {
    5,11,
    11,11,
    11,17,
    5,17
};

LogoRaw logo_octave_3 = {
    20,18,
    24,18,
    24,22,
    20,22
};

LogoRaw logo_octave_4 = {
    18,6,
    21,6,
    21,9,
    18,9
};

LogoRaw logo_opengl_0 = {
    7,13.5,
    8.5,13.5,
    8.5,16.5,
    7,16.5
};

LogoRaw logo_opengl_1 = {
    11,15.5,
    11.5,15.5,
    11.5,16.5,
    11,16.5
};

LogoRaw logo_opengl_2 = {
    5,9,
    20,9,
    20,11.5,
    19,11.5,
    19,10.5,
    7,10.5,
    7,12.5,
    9.5,12.5,
    9.5,17.5,
    7,17.5,
    7,19.5,
    19,19.5,
    19,18.5,
    20,18.5,
    20,21,
    5,21
};

LogoRaw logo_opengl_3 = {
    10,14.5,
    12.5,14.5,
    12.5,17.5,
    11,17.5,
    11,19,
    10,19
};

LogoRaw logo_opengl_4 = {
    13,14.5,
    15.5,14.5,
    15.5,16,
    14.5,16,
    14.5,15.5,
    14,15.5,
    14,16.5,
    15.5,16.5,
    15.5,17.5,
    13,17.5
};

LogoRaw logo_opengl_5 = {
    16,14.5,
    18.5,14.5,
    18.5,17.5,
    17.5,17.5,
    17.5,15.5,
    17,15.5,
    17,17.5,
    16,17.5
};

LogoRaw logo_opengl_6 = {
    19,12.5,
    22.5,12.5,
    22.5,14,
    20.5,14,
    20.5,16,
    21,16,
    21,14.5,
    22.5,14.5,
    22.5,17.5,
    19,17.5
};

LogoRaw logo_opengl_7 = {
    23,12.5,
    24.5,12.5,
    24.5,16,
    26,16,
    26,17.5,
    23,17.5
};

LogoRaw logo_vulkan_0 = {
    21,17,
    21.5,17,
    21.5,17.5,
    21,17.5
};

LogoRaw logo_vulkan_1 = {
    4.5,10.5,
    17,10.5,
    17,12.5,
    16,12.5,
    16,12,
    6.5,12,
    6.5,16,
    7.5,16,
    7.5,17,
    4.5,17
};

LogoRaw logo_vulkan_2 = {
    8,13,
    9,13,
    9,17.5,
    10,17.5,
    10,13,
    11,13,
    11,18,
    10.5,18,
    10.5,18.5,
    8.5,18.5,
    8.5,18,
    8,18
};

LogoRaw logo_vulkan_3 = {
    11.5,15,
    12.5,15,
    12.5,17.5,
    13,17.5,
    13,15,
    14,15,
    14,18.5,
    11.5,18.5
};

LogoRaw logo_vulkan_4 = {
    14.5,13,
    15.5,13,
    15.5,17.5,
    16,17.5,
    16,18.5,
    14.5,18.5
};

LogoRaw logo_vulkan_5 = {
    16.5,13,
    17.5,13,
    17.5,15,
    19.5,15,
    19.5,16,
    19,16,
    19,17,
    19.5,17,
    19.5,18.5,
    18.5,18.5,
    18.5,17.5,
    18,17.5,
    18,16.5,
    17.5,16.5,
    17.5,18.5,
    16.5,18.5
};

LogoRaw logo_vulkan_6 = {
    20,15,
    22.5,15,
    22.5,18.5,
    20,18.5,
    20,16.5,
    21.5,16.5,
    21.5,16,
    20,16
};

LogoRaw logo_vulkan_7 = {
    23,15,
    25.5,15,
    25.5,18.5,
    24.5,18.5,
    24.5,16,
    24,16,
    24,18.5,
    23,18.5
};

LogoRaw logo_desmos_0 = {
    6.5,20.5,
    7,20.5,
    7,22,
    6.5,22
};

LogoRaw logo_desmos_1 = {
    20,20.5,
    20.5,20.5,
    20.5,22,
    20,22
};

LogoRaw logo_desmos_2 = {
    11.5,6,
    12.5,6,
    12.5,12.5,
    11.5,12.5
};

LogoRaw logo_desmos_3 = {
    18,6,
    19,6,
    19,10.5,
    18,10.5
};

LogoRaw logo_desmos_4 = {
    7,11,
    10,11,
    10,13,
    14.5,13,
    14.5,11,
    21,11,
    21,13,
    23,13,
    23,14,
    20,14,
    20,12,
    15.5,12,
    15.5,14,
    9,14,
    9,12,
    7,12
};

LogoRaw logo_desmos_5 = {
    12,14.5,
    13,14.5,
    13,16,
    17,16,
    17,12.5,
    18,12.5,
    18,17,
    12,17
};

LogoRaw logo_desmos_6 = {
    5.5,19.5,
    7,19.5,
    7,18,
    8,18,
    8,23,
    5.5,23
};

LogoRaw logo_desmos_7 = {
    8.5,19.5,
    11,19.5,
    11,21.5,
    10,21.5,
    10,20.5,
    9.5,20.5,
    9.5,22,
    11,22,
    11,23,
    8.5,23
};

LogoRaw logo_desmos_8 = {
    11.5,19.5,
    14,19.5,
    14,20.5,
    12.5,20.5,
    12.5,21,
    14,21,
    14,23,
    11.5,23,
    11.5,22,
    13,22,
    13,21.5,
    11.5,21.5
};

LogoRaw logo_desmos_9 = {
    14.5,19.5,
    18.5,19.5,
    18.5,23,
    17.5,23,
    17.5,20.5,
    17,20.5,
    17,22.5,
    16,22.5,
    16,20.5,
    15.5,20.5,
    15.5,23,
    14.5,23
};

LogoRaw logo_desmos_10 = {
    19,19.5,
    21.5,19.5,
    21.5,23,
    19,23
};

LogoRaw logo_desmos_11 = {
    22,19.5,
    24.5,19.5,
    24.5,20.5,
    23,20.5,
    23,21,
    24.5,21,
    24.5,23,
    22,23,
    22,22,
    23.5,22,
    23.5,21.5,
    22,21.5
};

LogoRaw logo_mc_0 = {
    6,6,
    24,6,
    24,10,
    6,10
};

LogoRaw logo_mc_1 = {
    6,10,
    24,10,
    24,24,
    6,24
};

ShapeGroup logo_c = {
    "c",
    2,
    {
        LOGO_POINT_COUNT(logo_base_bg),
        LOGO_POINT_COUNT(logo_c_0)
    },
    {
        logo_base_bg,
        logo_c_0
    }
};

ShapeGroup logo_cpp = {
    "cpp",
    3,
    {
        LOGO_POINT_COUNT(logo_base_bg),
        LOGO_POINT_COUNT(logo_cpp_0),
        LOGO_POINT_COUNT(logo_cpp_1)
    },
    {
        logo_base_bg,
        logo_cpp_0,
        logo_cpp_1
    }
};

ShapeGroup logo_js = {
    "js",
    3,
    {
        LOGO_POINT_COUNT(logo_js_0),
        LOGO_POINT_COUNT(logo_js_1),
        LOGO_POINT_COUNT(logo_js_2)
    },
    {
        logo_js_0,
        logo_js_1,
        logo_js_2
    }
};

ShapeGroup logo_html = {
    "html",
    5,
    {
        LOGO_POINT_COUNT(logo_base_bg),
        LOGO_POINT_COUNT(logo_html_0),
        LOGO_POINT_COUNT(logo_html_1),
        LOGO_POINT_COUNT(logo_html_2),
        LOGO_POINT_COUNT(logo_html_3)
    },
    {
        logo_base_bg,
        logo_html_0,
        logo_html_1,
        logo_html_2,
        logo_html_3
    }
};

ShapeGroup logo_css = {
    "css",
    5,
    {
        LOGO_POINT_COUNT(logo_base_bg),
        LOGO_POINT_COUNT(logo_css_0),
        LOGO_POINT_COUNT(logo_css_1),
        LOGO_POINT_COUNT(logo_css_2),
        LOGO_POINT_COUNT(logo_css_3)
    },
    {
        logo_base_bg,
        logo_css_0,
        logo_css_1,
        logo_css_2,
        logo_css_3
    }
};

ShapeGroup logo_rust = {
    "rust",
    6,
    {
        LOGO_POINT_COUNT(logo_rust_0),
        LOGO_POINT_COUNT(logo_rust_1),
        LOGO_POINT_COUNT(logo_rust_2),
        LOGO_POINT_COUNT(logo_rust_3),
        LOGO_POINT_COUNT(logo_rust_4),
        LOGO_POINT_COUNT(logo_rust_5)
    },
    {
        logo_rust_0,
        logo_rust_1,
        logo_rust_2,
        logo_rust_3,
        logo_rust_4,
        logo_rust_5
    }
};

ShapeGroup logo_go = {
    "go",
    7,
    {
        LOGO_POINT_COUNT(logo_base_bg),
        LOGO_POINT_COUNT(logo_go_0),
        LOGO_POINT_COUNT(logo_go_1),
        LOGO_POINT_COUNT(logo_go_2),
        LOGO_POINT_COUNT(logo_go_3),
        LOGO_POINT_COUNT(logo_go_4),
        LOGO_POINT_COUNT(logo_go_5)
    },
    {
        logo_base_bg,
        logo_go_0,
        logo_go_1,
        logo_go_2,
        logo_go_3,
        logo_go_4,
        logo_go_5
    }
};

ShapeGroup logo_octave = {
    "octave",
    7,
    {
        LOGO_POINT_COUNT(logo_base_bg),
        LOGO_POINT_COUNT(logo_octave_0),
        LOGO_POINT_COUNT(logo_octave_1),
        LOGO_POINT_COUNT(logo_octave_2),
        LOGO_POINT_COUNT(logo_octave_3),
        LOGO_POINT_COUNT(logo_octave_4)
    },
    {
        logo_base_bg,
        logo_octave_0,
        logo_octave_1,
        logo_octave_2,
        logo_octave_3,
        logo_octave_4
    }
};

ShapeGroup logo_opengl = {
    "opengl",
    9,
    {
        LOGO_POINT_COUNT(logo_base_bg),
        LOGO_POINT_COUNT(logo_opengl_0),
        LOGO_POINT_COUNT(logo_opengl_1),
        LOGO_POINT_COUNT(logo_opengl_2),
        LOGO_POINT_COUNT(logo_opengl_3),
        LOGO_POINT_COUNT(logo_opengl_4),
        LOGO_POINT_COUNT(logo_opengl_5),
        LOGO_POINT_COUNT(logo_opengl_6),
        LOGO_POINT_COUNT(logo_opengl_7)

    },
    {
        logo_base_bg,
        logo_opengl_0,
        logo_opengl_1,
        logo_opengl_2,
        logo_opengl_3,
        logo_opengl_4,
        logo_opengl_5,
        logo_opengl_6,
        logo_opengl_7
    }
};

ShapeGroup logo_vulkan = {
    "vulkan",
    9,
    {
        LOGO_POINT_COUNT(logo_base_bg),
        LOGO_POINT_COUNT(logo_vulkan_0),
        LOGO_POINT_COUNT(logo_vulkan_1),
        LOGO_POINT_COUNT(logo_vulkan_2),
        LOGO_POINT_COUNT(logo_vulkan_3),
        LOGO_POINT_COUNT(logo_vulkan_4),
        LOGO_POINT_COUNT(logo_vulkan_5),
        LOGO_POINT_COUNT(logo_vulkan_6),
        LOGO_POINT_COUNT(logo_vulkan_7)
    },
    {
        logo_base_bg,
        logo_vulkan_0,
        logo_vulkan_1,
        logo_vulkan_2,
        logo_vulkan_3,
        logo_vulkan_4,
        logo_vulkan_5,
        logo_vulkan_6,
        logo_vulkan_7
    }
};

ShapeGroup logo_desmos = {
    "desmos",
    13,
    {
        LOGO_POINT_COUNT(logo_base_bg),
        LOGO_POINT_COUNT(logo_desmos_0),
        LOGO_POINT_COUNT(logo_desmos_1),
        LOGO_POINT_COUNT(logo_desmos_2),
        LOGO_POINT_COUNT(logo_desmos_3),
        LOGO_POINT_COUNT(logo_desmos_4),
        LOGO_POINT_COUNT(logo_desmos_5),
        LOGO_POINT_COUNT(logo_desmos_6),
        LOGO_POINT_COUNT(logo_desmos_7),
        LOGO_POINT_COUNT(logo_desmos_8),
        LOGO_POINT_COUNT(logo_desmos_9),
        LOGO_POINT_COUNT(logo_desmos_10),
        LOGO_POINT_COUNT(logo_desmos_11)
    },
    {
        logo_base_bg,
        logo_desmos_0,
        logo_desmos_1,
        logo_desmos_2,
        logo_desmos_3,
        logo_desmos_4,
        logo_desmos_5,
        logo_desmos_6,
        logo_desmos_7,
        logo_desmos_8,
        logo_desmos_9,
        logo_desmos_10,
        logo_desmos_11
    }
};

ShapeGroup logo_mc = {
    "mc",
    3,
    {
        LOGO_POINT_COUNT(logo_base_bg),
        LOGO_POINT_COUNT(logo_mc_0),
        LOGO_POINT_COUNT(logo_mc_1)
    },
    {
        logo_base_bg,
        logo_mc_0,
        logo_mc_1
    }
};

ShapeGroup *all_logos[] = {
    &logo_c,
    &logo_cpp,
    &logo_js,
    &logo_html,
    &logo_css,
    &logo_rust,
    &logo_go,
    &logo_octave,
    &logo_opengl,
    &logo_vulkan,
    &logo_desmos,
    &logo_mc
};

#define ALL_LOGO_COUNT (sizeof(all_logos) / sizeof(all_logos[0]))