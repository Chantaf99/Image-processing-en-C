//main.c
#include <stdio.h>
#include <stdlib.h>
#include "bmp8.h"
#include "kernels.h"
#include "bmp24.h"
//int main(void){
/*
//  Partie 1 Appel de l'image BMP 8 bits
    t_bmp8 *img = bmp8_loadImage("barbara_gray (1).bmp");
    if (!img) {
        perror("Échec du chargement de l'image\n");
        return 1;
    }



  //Partie 1 Traitement d'images BMP 8 bits
    t_bmp8 *img_negative8 = bmp8_loadImage("barbara_gray (1).bmp");
    bmp8_negative(img_negative8);
    bmp8_saveImage("image_negative8.bmp", img_negative8);
    bmp8_free(img_negative8);


    t_bmp8 *img_brightness8 = bmp8_loadImage("barbara_gray (1).bmp");
    bmp8_brightness(img_brightness8, 100);
    bmp8_saveImage("image_brightness8.bmp", img_brightness8);
    bmp8_free(img_brightness8);

    t_bmp8 *img_threshold8 = bmp8_loadImage("barbara_gray (1).bmp");
    bmp8_threshold(img_threshold8, 50);
    bmp8_saveImage("image_threshold8.bmp", img_threshold8);
    bmp8_free(img_threshold8);
    bmp8_free(img);

    
// Partie 1 Traitement de filtres
    const char *src = "barbara_gray (1).bmp";
    apply_and_save(src, "image_sharpen8.bmp", sharpen, 3);
    apply_and_save(src, "image_box_blur8.bmp", box_blur, 3);
    apply_and_save(src, "image_gauss8.bmp", gaussian_blur, 3);
    apply_and_save(src, "image_outline8.bmp", outline, 3);
    apply_and_save(src, "image_emboss8.bmp", emboss, 3);

    bmp8_free(img);

    printf("Programme termine avec succes.\n");


    //Partie 2 Traitement d'images BMP 24 bits

    t_bmp24 *img24 = bmp24_loadImage("flowers_color.bmp");
    if (!img24) {
        perror("Échec du chargement de l'image BMP 24 bits");
        return 1;
    }

    // Traitement d'image BMP 24 bits
    t_bmp24 *img_negative24 = bmp24_loadImage("flowers_color.bmp");
    if (!img_negative24) {
        perror("Échec du chargement de l'image BMP 24 bits pour le négatif");
        return 1;
    }
    bmp24_negative(img_negative24);
    bmp24_saveImage(img_negative24, "flowers_color_negative24.bmp");
    bmp24_free(img_negative24);


    t_bmp24 *img_grayscale24 = bmp24_loadImage("flowers_color.bmp");
    bmp24_grayscale(img_grayscale24);
    bmp24_saveImage(img_grayscale24, "flowers_color_grayscale24.bmp"); 
    bmp24_free(img_grayscale24);
   
    t_bmp24 *img_brightness24 = bmp24_loadImage("flowers_color.bmp");
    bmp24_brightness(img_brightness24, 50);
    bmp24_saveImage(img_brightness24, "flowers_color_brightness24.bmp");
    bmp24_free(img_brightness24);
    
    bmp24_free(img24);

    printf("Traitement d'images BMP 24 bits termine avec succes.\n");

    //Traitement de filtres sur BMP 24 bits

    const char *src24 = "flowers_color.bmp";
    apply_and_save_24(src24, "flowers_color_sharpen24.bmp", sharpen, 3);
    apply_and_save_24(src24, "flowers_color_box_blur24.bmp", box_blur, 3);
    apply_and_save_24(src24, "flowers_color_gaussian24.bmp", gaussian_blur, 3);
    apply_and_save_24(src24, "flowers_color_outline24.bmp", outline, 3);
    apply_and_save_24(src24, "flowers_color_emboss24.bmp", emboss, 3);

    

    //Egalisation d'histogramme en gris
    t_bmp8 *img = bmp8_loadImage("image_brightness.bmp");
    if (!img || img->data == NULL) {
        printf("Erreur chargement image\n");
        return 1;
    }

    unsigned int *hist = bmp8_computeHistogram(img);
    unsigned int *cdf = bmp8_computeCDF(hist);
    bmp8_equalize(img, cdf);

    bmp8_saveImage("imagegris_equalized.bmp", img);
    bmp8_free(img);
    free(hist);
    free(cdf);
*/
    //Egalisation d'histogramme en couleur
int main(void){
    t_bmp24 *img = bmp24_loadImage("flowers_color.bmp");
    if (!img) {
        printf("Erreur chargement image\n");
        return 1;
    }

    bmp24_equalizeColor(img);

    bmp24_saveImage(img, "flowers_equalized.bmp");
    bmp24_free(img);
    printf("Image Flowers equalized bien chargee");

    return 0;
}