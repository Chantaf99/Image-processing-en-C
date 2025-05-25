#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "bmp8.h" 
#include "bmp24.h"
#include "filtres_convolution.h"

//Traitement de filtres sur BMP 8 bits
void bmp8_applyFilter(t_bmp8 *img, float **kernel, int kernelSize) {
    int w = img->width;
    int h = img->height;
    int half = kernelSize / 2;

    // 1) Allouer un nouveau buffer pour les pixels
    unsigned char *newData = malloc(img->dataSize);
    if (!newData) {
        perror("Erreur d'allocation de mémoire pour le filtre");
        return;
    }

    // 2) Copier les bords tels quels (on n'y applique pas le filtre)
    memcpy(newData, img->data, img->dataSize);

    // 3) Parcourir uniquement l'intérieur de l'image
    for (int y = half; y < h - half; y++) {
        for (int x = half; x < w - half; x++) {
            float acc = 0.0f;

            // 4) Convoluer le noyau centré sur (x,y)
            for (int ky = -half; ky <= half; ky++) {
                for (int kx = -half; kx <= half; kx++) {
                    int ix = x + kx;
                    int iy = y + ky;
                    float coef = kernel[ky + half][kx + half];
                    acc += img->data[iy * w + ix] * coef;
                }
            }

            // 5) Clamper la somme entre 0 et 255
            if      (acc < 0.0f)   acc = 0.0f;
            else if (acc > 255.0f) acc = 255.0f;

            // 6) Stocker le résultat
            newData[y * w + x] = (unsigned char)(acc + 0.5f);
        }
    }

    // 7) Remplacer l'ancien buffer par le nouveau
    free(img->data);
    img->data = newData;
}
void apply_and_save(const char *srcFilename, const char *outFilename, float **kernel, int kernelSize){
    
    t_bmp8 *img = bmp8_loadImage(srcFilename);
    
    if (!img) {
        printf("Erreur : impossible de charger '%s'\n", srcFilename);
        return;
    }
    bmp8_applyFilter(img, kernel, kernelSize);
    bmp8_saveImage(outFilename, img);

    
    free(img);


}

//Traitement de filtres sur BMP 24 bits
t_pixel bmp24_convolution(t_bmp24 *img, int x, int y, float **kernel, int kernelSize) {
    int half = kernelSize / 2;
    float r = 0.0f, g = 0.0f, b = 0.0f;

    for (int ky = -half; ky <= half; ky++) {
        for (int kx = -half; kx <= half; kx++) {
            int ix = x + kx;
            int iy = y + ky;

            // Ignore les pixels hors limites
            if (ix < 0 || ix >= img->width || iy < 0 || iy >= img->height)
                continue;

            float coef = kernel[ky + half][kx + half];
            t_pixel px = img->data[iy][ix];

            r += px.red   * coef;
            g += px.green * coef;
            b += px.blue  * coef;
        }
    }

    // Clamping + arrondi
    if (r < 0.0f) r = 0.0f; else if (r > 255.0f) r = 255.0f;
    if (g < 0.0f) g = 0.0f; else if (g > 255.0f) g = 255.0f;
    if (b < 0.0f) b = 0.0f; else if (b > 255.0f) b = 255.0f;

    t_pixel result = {
        .red   = (uint8_t)(r + 0.5f),
        .green = (uint8_t)(g + 0.5f),
        .blue  = (uint8_t)(b + 0.5f)
    };

    return result;
}
void bmp24_applyFilter(t_bmp24 *img, float **kernel, int kernelSize) {
    int w = img->width;
    int h = img->height;

    // Allouer un nouveau tableau de pixels
    t_pixel **newData = malloc(h * sizeof(t_pixel *));
    for (int y = 0; y < h; y++) {
        newData[y] = malloc(w * sizeof(t_pixel));
    }

    // Appliquer la convolution à chaque pixel
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            newData[y][x] = bmp24_convolution(img, x, y, kernel, kernelSize);
        }
    }

    // Libérer l'ancien tableau
    for (int y = 0; y < h; y++) {
        free(img->data[y]);
    }
    free(img->data);

    // Remplacer par le nouveau
    img->data = newData;
}

void apply_and_save_24(const char *srcFilename, const char *outFilename, float **kernel, int kernelSize) {
    t_bmp24 *img = bmp24_loadImage(srcFilename);

    if (!img) {
        printf("Erreur : impossible de charger l'image '%s'\n", srcFilename);
        return;
    }

    bmp24_applyFilter(img, kernel, kernelSize);
    bmp24_saveImage(img, outFilename);

    printf("Image traitee et sauvegardee dans '%s'\n", outFilename);
    bmp24_free(img);
}

