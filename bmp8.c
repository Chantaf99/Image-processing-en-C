
#include "bmp8.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

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
        fprintf(stderr, "Erreur : impossible de charger '%s'\n", srcFilename);
        return;

    bmp8_applyFilter(img, kernel, kernelSize);

    bmp8_saveImage(outFilename, img);
    
    printf("Image sauvegardee dans %s\n", outFilename);}
    free(img);

}



