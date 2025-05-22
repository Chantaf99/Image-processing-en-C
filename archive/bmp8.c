
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
        perror("Erreur d'allocation de mémoire");
        fclose(file);
        return NULL;
    }
    if (img->colorDepth != 8) {
        perror("Erreur: l'image n'est pas en niveaux de gris (8 bits)\n");
        free(img);
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
    // Allocation dynamique de la mémoire pour stocker les données des pixels
    img->data = malloc(img->dataSize);
    if (!img->data) {
        perror("Erreur. Echec de l'allocation des données.\n");
        free(img);
        fclose(file);
        return NULL;
    }
    // On lit les octets du fichier bmp  et on les stocke
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
    printf("L'image a été sauvegardée avec succès dans %s\n", filename);
    
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
}

void bmp8_brightness(t_bmp8 *img, int value) {
}

void bmp8_threshold(t_bmp8 *img, int threshold) {
}

void bmp8_applyFilter(t_bmp8 *img, float **kernel, int kernelSize) {
}



