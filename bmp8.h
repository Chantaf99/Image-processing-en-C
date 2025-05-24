//
// Created by alilo on 02/04/2025.
//

#ifndef BMP8_H
#define BMP8_H

// Type structuré bmp8 pour représente une image en niveaux de gris
typedef struct {
    unsigned char header[54];
    unsigned char colorTable[1024];
    unsigned char *data;
    unsigned int width;
    unsigned int height;
    unsigned int colorDepth;
    unsigned int dataSize;
} t_bmp8;

// Lecture et écriture d’une image en niveaux de gris
t_bmp8 *bmp8_loadImage(const char *filename);
void bmp8_saveImage(const char *filename, t_bmp8 *img);
void bmp8_free(t_bmp8 *img);
void bmp8_printInfo(t_bmp8 *img);

// Traitement d'une image en niveaux de gris
void bmp8_negative(t_bmp8 *img);
void bmp8_brightness(t_bmp8 *img, int value);
void bmp8_threshold(t_bmp8 *img, int threshold);
void bmp8_applyFilter(t_bmp8 *img, float **kernel, int kernelSize);
void apply_and_save(const char *srcFilename, const char *outFilename, float **kernel, int kernelSize);

// Egalisation d'histogrammes
unsigned int * bmp8_computeHistogram(t_bmp8 * img);
unsigned int * bmp8_computeCDF(unsigned int * hist);
void bmp8_equalize(t_bmp8 * img, unsigned int * hist_eq);



#endif






