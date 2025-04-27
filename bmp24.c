


#include "bmp24.h"
#include <stdio.h>
#include <stdlib.h>

t_pixel **bmp24_allocateDataPixels(int width, int height) {
    return NULL;
}

void bmp24_freeDataPixels(t_pixel **pixels, int height) {
}

t_bmp24 *bmp24_allocate(int width, int height, int colorDepth) {
    return NULL;
}

void bmp24_free(t_bmp24 *img) {
}

t_bmp24 *bmp24_loadImage(const char *filename) {
    return NULL;
}

void bmp24_saveImage(t_bmp24 *img, const char *filename) {
}

void bmp24_negative(t_bmp24 *img) {
}

void bmp24_grayscale(t_bmp24 *img) {
}

void bmp24_brightness(t_bmp24 *img, int value) {
}

t_pixel bmp24_convolution(t_bmp24 *img, int x, int y, float **kernel, int kernelSize) {
    t_pixel p = {0, 0, 0};
    return p;
}

void bmp24_boxBlur(t_bmp24 *img) {
}

void bmp24_gaussianBlur(t_bmp24 *img) {
}

void bmp24_outline(t_bmp24 *img) {
}

void bmp24_emboss(t_bmp24 *img) {
}

void bmp24_sharpen(t_bmp24 *img) {
}
