
#include "bmp8.h"
#include <stdio.h>
#include <stdlib.h>

t_bmp8 *bmp8_loadImage(const char *filename)
{
        //Ouvre le fichier en mode lecture binaire
        FILE *file = fopen(filename, "rb");
        if (!file) {
            printf("Erreur. Impossible d""'""ouvrir le fichier %s\n", filename);
            return NULL;
        }

        t_bmp8 *img = malloc(sizeof(t_bmp8));
        if (!img) {
            printf("Erreur. Allocation mémoire ""é""chou""é""e.\n");
            fclose(file);
            return NULL;
        }

        // On lit les 54 octets de l'en-tete
        fread(img->header, sizeof(unsigned char), 54, file);

        // On lit la table de couleurs, qui contient 1024 octets
        fread(img->colorTable, sizeof(unsigned char), 1024, file);

        // On extrait les informations à partir de l'en-tete bmp
        img->width = *(unsigned int *)&img->header[18];
        img->height = *(unsigned int *)&img->header[22];
        img->colorDepth = *(unsigned short *)&img->header[28];
        img->dataSize = *(unsigned int *)&img->header[34];

        // On vérifie la profondeur(si l'image n'a pas 8 bits, ce n'est pas le bon format)
        if (img->colorDepth != 8) {
            printf("Erreur. L’image n’est pas en niveaux de gris (8 bits).\n");
            free(img);
            fclose(file);
            return NULL;
        }

        // Allocation dynamique de la mémoire pour stocker les données des pixels
        img->data = malloc(img->dataSize);
        if (!img->data) {
            printf("Erreur. Echec de l’allocation des données.\n");
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






void bmp8_saveImage(const char *filename, t_bmp8 *img)
{
      //Ouvre le fichier en mode ecriture binaire et vérifie que l'ouverture a ete reussi
    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("Erreur. Il est impossible d’ouvrir le fichier %s\n", filename);
        return;
    }

    fwrite(img->header, sizeof(unsigned char), 54, file);
    fwrite(img->colorTable, sizeof(unsigned char), 1024, file);
    fwrite(img->data, sizeof(unsigned char), img->dataSize, file);

    fclose(file); //ferme le fichier, afin de finaliser l'écriture

}




void bmp8_free(t_bmp8 *img)
{
  //On vérifie que img n'est pas un pointeur NULL
    if (img) {
        if (img->data) free(img->data);
        free(img);
    }
}


void bmp8_printInfo(t_bmp8 *img)
{
    //On affiche les infos principales de l'image bmp
    printf("Image Info:\n");
    printf("Width: %u\n", img->width);
    printf("Height: %u\n", img->height);
    printf("Color Depth: %u\n", img->colorDepth);
    printf("Data Size: %u\n", img->dataSize);

}

void bmp8_negative(t_bmp8 *img) {
}

void bmp8_brightness(t_bmp8 *img, int value) {
}

void bmp8_threshold(t_bmp8 *img, int threshold) {
}

void bmp8_applyFilter(t_bmp8 *img, float **kernel, int kernelSize) {
}