//
// Created by alilo on 19/03/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include "bmp8.h"

int main() {

    const char *inputFilename = "barbara_gray (1).bmp";
    const char *outputFilename = "barbara_gray_sauvegarde.bmp";

    // Charger l’image
    t_bmp8 *img = bmp8_loadImage(inputFilename);
    if (!img) {
        printf("Erreur lors du chargement de l’image.\n");
        return 1;
    }


    bmp8_printInfo(img);


    bmp8_saveImage(outputFilename, img);
    printf("Image sauvegardée dans %s\n", outputFilename);


    bmp8_free(img);

    return 0;
}

