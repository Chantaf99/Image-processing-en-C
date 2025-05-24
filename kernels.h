// kernels.h
#ifndef KERNELS_H
#define KERNELS_H

#include <stddef.h>

// Taille standard de tes noyaux
#define KERNEL_SIZE_3 3

// Pour chaque noyau : déclaration extern du tableau contigu
extern const float box_blur_arr[KERNEL_SIZE_3][KERNEL_SIZE_3];
extern const float gaussian_blur_arr[KERNEL_SIZE_3][KERNEL_SIZE_3];
extern const float outline_arr[KERNEL_SIZE_3][KERNEL_SIZE_3];
extern const float emboss_arr[KERNEL_SIZE_3][KERNEL_SIZE_3];
extern const float sharpen_arr[KERNEL_SIZE_3][KERNEL_SIZE_3];

// Et du coup, déclarer aussi les tableaux de pointeurs
extern float *box_blur[KERNEL_SIZE_3];
extern float *gaussian_blur[KERNEL_SIZE_3];
extern float *outline[KERNEL_SIZE_3];
extern float *emboss[KERNEL_SIZE_3];
extern float *sharpen[KERNEL_SIZE_3];

#endif // KERNELS_H
