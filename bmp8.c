
#include "bmp8.h"
#include <stdio.h>
#include <stdlib.h>

t_bmp8 *bmp8_loadImage(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Erreur d'ouverture du fichier");
        return NULL;
    }
    t_bmp8 *img = (t_bmp8 *)malloc(sizeof(t_bmp8));
    if (!img) {
        perror("Erreur d'allocation de memoire");
        fclose(file);
        return NULL;
    }
    // On lit les 54 octets de l'en-tete
    fread(img->header, sizeof(unsigned char), 54, file);
    // On extrait les informations à partir de l'en-tete bmp
    img->width = *(unsigned int *)&img->header[18];
    img->height = *(unsigned int *)&img->header[22];
    img->colorDepth = *(unsigned short *)&img->header[28];
    img->dataSize = *(unsigned int *)&img->header[34];
    if(img->dataSize == 0) {
        img->dataSize = img->width * img->height;
    }
    // Allocation dynamique de la mémoire pour stocker les données des pixels
    if (img->colorDepth != 8) {
        perror("Erreur: l'image n'est pas en niveaux de gris (8 bits)\n");
        free(img);
        fclose(file);
        return NULL;
    }
    fread(img->colorTable, sizeof(unsigned char), 1024, file);
    img->data = malloc(img->dataSize);
    if (!img->data) {
        perror("Erreur. Echec de l'allocation des donnees.\n");
        free(img);
        fclose(file);
        return NULL;
    }

    fread(img->data, sizeof(unsigned char), img->dataSize, file);
    //On ferme le fichier, et on retourne l'adresse de l'image
    fclose(file);
    return img;

}

void bmp8_saveImage(const char *filename, t_bmp8 *img) {
    //Ouvre le fichier en mode ecriture binaire et vérifie que l'ouverture a ete reussi
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Erreur. Il est impossible d'ouvrir le fichier \n");
        return;
    }

    fwrite(img->header, sizeof(unsigned char), 54, file);
    fwrite(img->colorTable, sizeof(unsigned char), 1024, file);
    fwrite(img->data, sizeof(unsigned char), img->dataSize, file);

    fclose(file); //ferme le fichier, afin de finaliser l'écriture
    printf("L'image a ete sauvegardee avec succes dans %s\n", filename);
    
}

void bmp8_free(t_bmp8 *img) {
    //On vérifie que img n'est pas un pointeur NULL
    if (img) {
        if (img->data) free(img->data);
        free(img);
    }

}

void bmp8_printInfo(t_bmp8 *img) {
    printf("Image Info:\n   ");
    printf("Width: %u\n   ", img->width);
    printf("Height: %u\n    ", img->height);
    printf("Color Depth: %u\n   ", img->colorDepth);
    printf("Data Size: %u\n   ", img->dataSize);
}


void bmp8_negative(t_bmp8 *img) {
    for (unsigned int i = 0; i < img->dataSize; i++) {
        img->data[i] = 255 - img->data[i];
    }
}

void bmp8_brightness(t_bmp8 *img, int value) {
    for (unsigned int i = 0; i < img->dataSize; i++) {
        int newValue = img->data[i] + value;
        if (newValue > 255) newValue = 255;
        if (newValue < 0) newValue = 0;
        img->data[i] = (unsigned char)newValue;
    }
}

void bmp8_threshold(t_bmp8 *img, int threshold) {
    for (unsigned int i = 0; i < img->dataSize; i++) {
        if (img->data[i] < threshold) {
            img->data[i] = 0;
        } else {
            img->data[i] = 255;
        }
    }
}

void bmp8_applyFilter(t_bmp8 *img, float **kernel, int kernelSize) {
    unsigned char *newData = (unsigned char *)malloc(img->dataSize);
    if (!newData) {
        perror("Erreur d'allocation de memoire pour le filtre\n");
        return;
    }
    int halfKernel = kernelSize / 2;
    for (unsigned int y = 0; y < img->height; y++) {
        for (unsigned int x = 0; x < img->width; x++) {
            float sum = 0.0f;
            for (int ky = -halfKernel; ky <= halfKernel; ky++) {
                for (int kx = -halfKernel; kx <= halfKernel; kx++) {
                    int pixelX = x + kx;
                    int pixelY = y + ky;
                    if (pixelX >= 0 && pixelX < img->width && pixelY >= 0 && pixelY < img->height) {
                        sum += img->data[pixelY * img->width + pixelX] * kernel[ky + halfKernel][kx + halfKernel];
                    }
                }
            }
            newData[y * img->width + x] = (unsigned char)sum;
        }
    }
    free(img->data);
    img->data = newData;
}



