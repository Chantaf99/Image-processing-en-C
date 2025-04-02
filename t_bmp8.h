//
// Created by alilo on 02/04/2025.
//

#ifndef T_BMP8_H
#define T_BMP8_H

#endif //T_BMP8_H

// Type structuré t_bmp8 pour représente une image en niveaux de gris
typedef struct {
    unsigned char header[54];
    unsigned char colorTable[1024];
    unsigned char * data;
    unsigned int width;
    unsigned int height;
    unsigned int colorDepth;
    unsigned int dataSize;
} t_bmp8;


// Lecture et écriture d’une image en niveaux de gris
t_bmp8 * bmp8_loadImage(const char * filename);
void bmp8_saveImage(const char * filename, t_bmp8 * img);
void bmp8_free(t_bmp8 * img);
void bmp8_printInfo(t_bmp8 * img);







