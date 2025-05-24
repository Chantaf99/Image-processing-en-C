// kernels.c
#include "kernels.h"

// 1) Définition des tableaux contigus
const float box_blur_arr[3][3] = {
    {1.0f/9, 1.0f/9, 1.0f/9},
    {1.0f/9, 1.0f/9, 1.0f/9},
    {1.0f/9, 1.0f/9, 1.0f/9}
};

const float gaussian_blur_arr[3][3] = {
    {1.0f/16, 2.0f/16, 1.0f/16},
    {2.0f/16, 4.0f/16, 2.0f/16},
    {1.0f/16, 2.0f/16, 1.0f/16}
};

const float outline_arr[3][3] = {
    {-1, -1, -1},
    {-1,  8, -1},
    {-1, -1, -1}
};

const float emboss_arr[3][3] = {
    {-2, -1, 0},
    {-1,  1, 1},
    { 0,  1, 2}
};

const float sharpen_arr[3][3] = {
    { 0, -1,  0},
    {-1,  5, -1},
    { 0, -1,  0}
};

// 2) Initialisation des tableaux de pointeurs sur chaque ligne
float *box_blur[3] = {
    (float*)box_blur_arr[0],
    (float*)box_blur_arr[1],
    (float*)box_blur_arr[2]
};

float *gaussian_blur[3] = {
    (float*)gaussian_blur_arr[0],
    (float*)gaussian_blur_arr[1],
    (float*)gaussian_blur_arr[2]
};

float *outline[3] = {
    (float*)outline_arr[0],
    (float*)outline_arr[1],
    (float*)outline_arr[2]
};

float *emboss[3] = {
    (float*)emboss_arr[0],
    (float*)emboss_arr[1],
    (float*)emboss_arr[2]
};

float *sharpen[3] = {
    (float*)sharpen_arr[0],
    (float*)sharpen_arr[1],
    (float*)sharpen_arr[2]
};
