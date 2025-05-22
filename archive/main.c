//
// Created by alilo on 19/03/2025.
//
#include <stdio.h>
#include "bmp8.h"

int main(void){


    t_bmp8 *img = bmp8_loadImage("barbara_gray (1).bmp");
    if (!img) {
        perror("Échec du chargement de l'image\n");
        return 1;
    }
    // Image négative
    t_bmp8 *img_negative = bmp8_loadImage("barbara_gray (1).bmp");
    bmp8_negative(img_negative);
    bmp8_saveImage("image_negative.bmp", img_negative);
    bmp8_free(img_negative);

    // Image avec plus de luminosité (+50)
    t_bmp8 *img_brightness = bmp8_loadImage("barbara_gray (1).bmp");
    bmp8_brightness(img_brightness, 100);
    bmp8_saveImage("image_brightness.bmp", img_brightness);
    bmp8_free(img_brightness);

    // Seuillage à 128
    t_bmp8 *img_threshold = bmp8_loadImage("barbara_gray (1).bmp");
    bmp8_threshold(img_threshold, 50);
    bmp8_saveImage("image_threshold.bmp", img_threshold);
    bmp8_free(img_threshold);


    // Libération de la mémoire
    bmp8_free(img);

    printf("Programme termine avec succes.\n");

    return 0;
}
