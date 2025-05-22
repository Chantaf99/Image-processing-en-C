//
// Created by alilo on 19/03/2025.
//
#include <stdio.h>
#include "bmp8.h"

int main(void){
    const char *input_filename  = "barbara_gray (1).bmp";
    const char *file_de_sortie = "barbara_gray_sauvegarde.bmp";
    
    // Chargement de l'image
    t_bmp8 *img = bmp8_loadImage(input_filename);
    if (!img) {
        perror("Échec du chargement de l'image\n");
        return 1;
    }

    // Affichage des informations
    bmp8_printInfo(img);

    // Sauvegarde de l'image
    bmp8_saveImage(file_de_sortie, img);

    // Libération de la mémoire
    bmp8_free(img);

    printf("Programme terminé avec succès.\n");
    return 0;
}
