#include "bmp8.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Charge une image BMP 8 bits (niveaux de gris)
t_bmp8 *bmp8_loadImage(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Erreur d'ouverture du fichier");
        return NULL;
    }

    // Allocation de la structure t_bmp8
    t_bmp8 *img = (t_bmp8 *)malloc(sizeof(t_bmp8));
    if (!img) {
        perror("Erreur d'allocation de memoire");
        fclose(file);
        return NULL;
    }

    // Lecture de l'en-tête BMP (54 octets)
    fread(img->header, sizeof(unsigned char), 54, file);

    // Extraction des métadonnées de l'image
    img->width = *(unsigned int *)&img->header[18];
    img->height = *(unsigned int *)&img->header[22];
    img->colorDepth = *(unsigned short *)&img->header[28];
    img->dataSize = *(unsigned int *)&img->header[34];
    if(img->dataSize == 0) {
        img->dataSize = img->width * img->height;
    }

    // Vérifie que l'image est bien en niveaux de gris (8 bits)
    if (img->colorDepth != 8) {
        perror("Erreur: l'image n'est pas en niveaux de gris (8 bits)\n");
        free(img);
        fclose(file);
        return NULL;
    }

    // Lecture de la table des couleurs (1024 octets pour 256 couleurs)
    fread(img->colorTable, sizeof(unsigned char), 1024, file);

    // Allocation des données d'image (pixels)
    img->data = malloc(img->dataSize);
    if (!img->data) {
        perror("Erreur. Echec de l'allocation des donnees.\n");
        free(img);
        fclose(file);
        return NULL;
    }

    // Lecture des pixels
    fread(img->data, sizeof(unsigned char), img->dataSize, file);
    fclose(file);
    return img;
}

// Sauvegarde une image BMP 8 bits sur le disque
void bmp8_saveImage(const char *filename, t_bmp8 *img) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Erreur. Il est impossible d'ouvrir le fichier \n");
        return;
    }

    fwrite(img->header, sizeof(unsigned char), 54, file);
    fwrite(img->colorTable, sizeof(unsigned char), 1024, file);
    fwrite(img->data, sizeof(unsigned char), img->dataSize, file);

    fclose(file);
    printf("L'image a ete sauvegardee avec succes dans %s\n", filename);
}

// Libère la mémoire allouée à une image BMP 8 bits
void bmp8_free(t_bmp8 *img) {
    if (img) {
        if (img->data) free(img->data);
        free(img);
    }
}

// Affiche les informations d'une image (dimensions, profondeur, taille)
void bmp8_printInfo(t_bmp8 *img) {
    printf("Image Info:\n   ");
    printf("Width: %u\n   ", img->width);
    printf("Height: %u\n    ", img->height);
    printf("Color Depth: %u\n   ", img->colorDepth);
    printf("Data Size: %u\n   ", img->dataSize);
}

// Applique un filtre négatif à l'image (inversion des pixels)
void bmp8_negative(t_bmp8 *img) {
    for (unsigned int i = 0; i < img->dataSize; i++) {
        img->data[i] = 255 - img->data[i];
    }
}

// Modifie la luminosité de l'image (+/- value)
void bmp8_brightness(t_bmp8 *img, int value) {
    for (unsigned int i = 0; i < img->dataSize; i++) {
        int newValue = img->data[i] + value;
        if (newValue > 255) newValue = 255;
        if (newValue < 0) newValue = 0;
        img->data[i] = (unsigned char)newValue;
    }
}

// Applique un seuillage (pixels < seuil deviennent noirs, les autres blancs)
void bmp8_threshold(t_bmp8 *img, int threshold) {
    for (unsigned int i = 0; i < img->dataSize; i++) {
        img->data[i] = (img->data[i] < threshold) ? 0 : 255;
    }
}

//Calcul l'histogramme d'une image de 8 bits en niveaux de gris
unsigned int * bmp8_computeHistogram(t_bmp8 * img) {

  if (img == NULL || img->data == NULL){
    return NULL;
    }
//On alloue un tableau de 256 cases pour représenter l'historigramme
  unsigned int *hist = (unsigned int *)calloc(256, sizeof(unsigned int));

  if (hist == NULL){
    printf("Erreur. Allocation mémoire histogramme.\n");
    return NULL;
    }

  //Remplissage de l'histogramme : on compte les pixels par niveaux de gris
  for (unsigned int i = 0; i < img->dataSize; i++) {
    hist[img->data[i]]++;
    }

  return hist;
}

//Calculer l’histogramme cumulé au moyen d’une CDF (fonction de repartition cumulé) et normalisation
unsigned int * bmp8_computeCDF(unsigned int * hist)
{

    if(hist==NULL){return NULL;}
    unsigned int *cdf = (unsigned int *)calloc(256, sizeof(unsigned int));
    if (cdf == NULL){
       printf("Erreur d'allocation de cdf");
       return NULL;
    }
    //Construction de la  CDF
    cdf[0] = hist[0];
    for (unsigned int i = 1; i < 256; i++) {
        cdf[i] = cdf[i-1] + hist[i];
    }

    //Calcul de la premiere valeur non nulle de la CDF (cdfmin)
    unsigned int cdfmin = 0;
    for (int i = 0; i < 256; i++) {
        if (cdf[i] != 0) {
            cdfmin = cdf[i];
            break;
        }
    }

    // Calcul de l'histogramme égalisé (normalisation de la CDF)
    unsigned int *hist_eq = (unsigned int *)calloc(256, sizeof(unsigned int));
    if (hist_eq == NULL) {
        printf("Erreur d'allocation de l'histogramme égalisé.\n");
        free(cdf);
        return NULL;
    }

    unsigned int totalPixels = cdf[255];

    for (int i = 0; i < 256; i++) {
        hist_eq[i] = round(((double)(cdf[i] - cdfmin) / (totalPixels - cdfmin)) * 255);
    }

    free(cdf);
    return hist_eq;
}

// Applique l'égalisation à l'image : remplace chaque pixel par sa nouvelle valeur
void bmp8_equalize(t_bmp8 *img, unsigned int *hist_eq) {


    if (img == NULL || img->data == NULL || hist_eq == NULL){
      return;
      }

    for (unsigned int i = 0; i < img->dataSize; i++) {
        img->data[i] = (unsigned char)hist_eq[img->data[i]];
    }
}


