//main.c
#include <stdio.h>
#include "bmp8.h"

// 1. Déclaration des noyaux 3×3 en tableau contigu
static const float box_blur_arr[3][3] = {
    {1.0f/9, 1.0f/9, 1.0f/9},
    {1.0f/9, 1.0f/9, 1.0f/9},
    {1.0f/9, 1.0f/9, 1.0f/9}
};

static const float gaussian_blur_arr[3][3] = {
    {1.0f/16, 2.0f/16, 1.0f/16},
    {2.0f/16, 4.0f/16, 2.0f/16},
    {1.0f/16, 2.0f/16, 1.0f/16}
};

static const float outline_arr[3][3] = {
    { -1, -1,  -1},
    { -1,  8,  -1},
    { -1, -1,  -1}
};
static const float emboss_arr[3][3] = {
    {-2, -1,  0},
    {-1,  1,  1},
    { 0,  1,  2}
};

static const float sharpen_arr[3][3] = {
    { 0, -1,  0},
    {-1,  5, -1},
    { 0, -1,  0}
};

// 2. Construction des tableaux de pointeurs
//    qui seront de type float *[3] et décayeront en float**
float *box_blur[3] = {
    (float*)box_blur_arr[0],
    (float*)box_blur_arr[1],
    (float*)box_blur_arr[2]
};

float *gaussian_blur[3] = {
    (float*)gaussian_blur_arr[0],
    (float*)gaussian_blur_arr[1],
    (float*)gaussian_blur_arr[2]
};

float *outline[3] = {
    (float*)outline_arr[0],
    (float*)outline_arr[1],
    (float*)outline_arr[2]
};
float *emboss[3] = {
    (float*)emboss_arr[0],
    (float*)emboss_arr[1],
    (float*)emboss_arr[2]};
float *sharpen[3] = {
    (float*)sharpen_arr[0],
    (float*)sharpen_arr[1],      
    (float*)sharpen_arr[2]};


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
