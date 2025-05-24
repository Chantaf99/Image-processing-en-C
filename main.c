//main.c
#include <stdio.h>
#include "bmp8.h"
#include "kernels.h"
int main(void){


    t_bmp8 *img = bmp8_loadImage("barbara_gray (1).bmp");
    if (!img) {
        perror("Échec du chargement de l'image\n");
        return 1;
    }
/*
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

const char *src = "barbara_gray (1).bmp";
    apply_and_save(src, "image_sharpen.bmp", sharpen, 3);
    apply_and_save(src, "image_box_blur.bmp", box_blur, 3);
    apply_and_save(src, "image_gauss.bmp", gaussian_blur, 3);
    apply_and_save(src, "image_outline.bmp", outline, 3);
    apply_and_save(src, "image_emboss.bmp", emboss, 3);
  
    bmp8_free(img);

    printf("Programme termine avec succes.\n");

    return 0;
}
