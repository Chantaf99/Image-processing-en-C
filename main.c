//main.c
#include <stdio.h>
#include "bmp8.h"
#include "kernels.h"
#include "bmp24.h"
int main(void){

/* Partie 1 Appel de l'image BMP 8 bits
    t_bmp8 *img = bmp8_loadImage("barbara_gray (1).bmp");
    if (!img) {
        perror("Échec du chargement de l'image\n");
        return 1;
    }
        */
/* Partie 1 Traitement d'images BMP 8 bits
    t_bmp8 *img_negative = bmp8_loadImage("barbara_gray (1).bmp");
    bmp8_negative(img_negative);
    bmp8_saveImage("image_negative.bmp", img_negative);
    bmp8_free(img_negative);


    t_bmp8 *img_brightness = bmp8_loadImage("barbara_gray (1).bmp");
    bmp8_brightness(img_brightness, 100);
    bmp8_saveImage("image_brightness.bmp", img_brightness);
    bmp8_free(img_brightness);

    t_bmp8 *img_threshold = bmp8_loadImage("barbara_gray (1).bmp");
    bmp8_threshold(img_threshold, 50);
    bmp8_saveImage("image_threshold.bmp", img_threshold);
    bmp8_free(img_threshold);
    bmp8_free(img);
*/
/* Partie 1 Traitement de filtres
const char *src = "barbara_gray (1).bmp";
    apply_and_save(src, "image_sharpen.bmp", sharpen, 3);
    apply_and_save(src, "image_box_blur.bmp", box_blur, 3);
    apply_and_save(src, "image_gauss.bmp", gaussian_blur, 3);
    apply_and_save(src, "image_outline.bmp", outline, 3);
    apply_and_save(src, "image_emboss.bmp", emboss, 3);
  
    bmp8_free(img);

    printf("Programme termine avec succes.\n");
*/
    // Partie 2 Traitement d'images BMP 24 bits

    t_bmp24 *img24 = bmp24_loadImage("flowers_color.bmp");
    if (!img24) {
        perror("Échec du chargement de l'image BMP 24 bits");
        return 1;
    }

    // Traitement d'image BMP 24 bits
    t_bmp24 *img_negative = bmp24_loadImage("flowers_color.bmp");
    if (!img_negative) {
        perror("Échec du chargement de l'image BMP 24 bits pour le négatif");
        return 1;
    }
    bmp24_negative(img_negative);
    bmp24_saveImage(img_negative, "flowers_color_negative.bmp");
    bmp24_free(img_negative);


    t_bmp24 *img_grayscale = bmp24_loadImage("flowers_color.bmp");
    bmp24_grayscale(img_grayscale);
    bmp24_saveImage(img_grayscale, "flowers_color_grayscale.bmp"); 
    bmp24_free(img_grayscale);
   
    t_bmp24 *img_brightness = bmp24_loadImage("flowers_color.bmp");
    bmp24_brightness(img_brightness, 50);
    bmp24_saveImage(img_brightness, "flowers_color_brightness.bmp");
    bmp24_free(img_brightness);
    
    bmp24_free(img24);

    printf("Traitement d'images BMP 24 bits termine avec succes.\n");

    //Traitement de filtres sur BMP 24 bits
    






    return 0;
}
