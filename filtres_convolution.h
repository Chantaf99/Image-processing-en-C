#ifndef FILTRES_CONV_H
#define FILTRES_CONV_H

#include "bmp8.h"
#include "bmp24.h"

// Appliquer filtre  8 bits 
void bmp8_applyFilter(t_bmp8 *img, float **kernel, int kernelSize);
void apply_and_save(const char *srcFilename, const char *outFilename, float **kernel, int kernelSize);

// Pour appliquer un filtre à une image 24 bits et sauvegarder le résultat
t_pixel bmp24_convolution(t_bmp24 *img, int x, int y, float **kernel, int kernelSize);
void bmp24_applyFilter(t_bmp24 *img, float **kernel, int kernelSize);
void apply_and_save_24(const char *srcFilename, const char *outFilename, float **kernel, int kernelSize);


#endif // FILTRES_CONV_H
