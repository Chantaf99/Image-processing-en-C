


#include "bmp24.h"
#include <stdio.h>
#include <stdlib.h>

t_pixel **bmp24_allocateDataPixels(int width, int height) {

    t_pixel **pixels = malloc(height * sizeof(t_pixel *));
    if (!pixels) {
        perror("Erreur d'allocation de mémoire pour les pixels");
        return NULL;
    }
    for (int i=0; i<height; i++){
        pixels[i]=malloc(width * sizeof(t_pixel));
        if (!pixels[i]) {
            perror("Erreur d'allocation de mémoire pour une ligne de pixels");
            // Libérer les lignes déjà allouées
            for (int j = 0; j < i; j++) {
                free(pixels[j]);
            }
            free(pixels);
            return NULL;
        }
    }
    return pixels;
}

void bmp24_freeDataPixels(t_pixel **pixels, int height) {
    if (pixels) {
        for (int i = 0; i < height; i++) {
            free(pixels[i]);
        }
        free(pixels);
    }
}

t_bmp24 *bmp24_allocate(int width, int height, int colorDepth) {
    t_bmp24 *img = malloc(sizeof(t_bmp24));
    if (!img) {
        perror("Erreur d'allocation de la structure t_bmp24");
        return NULL;
    }
    img->data = bmp24_allocateDataPixels(width, height);
    if (!img->data) {
        free(img);
        perror("Erreur d'allocation des données des pixels");
        return NULL;
    }
    img->width = width;
    img->height = height;
    img->colorDepth = colorDepth;

    return img;
}

void bmp24_free(t_bmp24 *img) {
    if (img) {
        bmp24_freeDataPixels(img->data, img->height);
        free(img);
    }
}
void file_rawRead (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) {
    fseek(file, position, SEEK_SET);
    fread(buffer, size, n, file);
 }

void file_rawWrite (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) {
    fseek(file, position, SEEK_SET);
        fwrite(buffer, size, n, file);
}

 /**
 * @brief Lit la valeur du pixel (x,y) depuis le fichier BMP et la stocke dans image->data[y][x].
 */
void bmp24_readPixelValue(t_bmp24 * img,  int x, int y, FILE * file){
  if (!(img->height%4 == 0) || !(img->width%4 == 0)) {
    printf("Error : invalid image size (%d,%d)\n", img->height, img->width);
    return;
  }
    // taille d'une ligne en octets (3 octets par pixel)
    uint32_t rowSize = img->width * 3;
    // inversion verticale (BMP stocke du bas vers le haut)
    uint32_t fileY = img->height - 1 - y;
    // position dans le fichier
    uint32_t pos = BMP24_DATA_OFFSET + fileY * rowSize + x * 3;

    unsigned char bgr[3];
    file_rawRead(pos, bgr, 1, 3, file);

    // stockage en RGB
    img->data[y][x].red   = bgr[2];
    img->data[y][x].green = bgr[1];
    img->data[y][x].blue  = bgr[0];;

}   

void bmp24_readPixelData(t_bmp24 *img, FILE *file) {
    for (int y = 0; y < img->height; y++)
      for (int x = 0; x < img->width; x++)
        bmp24_readPixelValue(img, x, y, file);
}
 //@brief Écrit la valeur du pixel (x,y) depuis image->data[y][x] dans le fichier BMP.

void bmp24_writePixelValue(t_bmp24 *img, int x, int y, FILE *file) { 

    uint32_t rowsize = img->width*3;
    uint32_t fileY = (uint32_t)img->height - 1 - (uint32_t) y; 
    uint32_t position = BMP24_DATA_OFFSET + fileY * rowsize + (uint32_t)x * 3;

    // Préparer les 3 octets en ordre B, G, R 
    unsigned char bgr[3] = { img->data[y][x].blue, img->data[y][x].green, img->data[y][x].red }; 
    // Écrire ces 3 octets 
    file_rawWrite(position, bgr, 1, 3, file); }

/**
 * @brief Écrit tous les pixels de image->data dans le fichier BMP.
 */
void bmp24_writePixelData(t_bmp24 *image, FILE *file) {
    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            bmp24_writePixelValue(image, x, y, file);
        }
    }
}

t_bmp24 *bmp24_loadImage(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Erreur ouverture BMP 24-bits");
        return NULL;
    }

    // 1) Lire BITMAPFILEHEADER champ par champ
    t_bmp_header header;
    file_rawRead(BITMAP_MAGIC,  &header.type,      sizeof header.type,      1, file);
    file_rawRead(BITMAP_SIZE,   &header.size,      sizeof header.size,      1, file);
    file_rawRead(6,             &header.reserved1, sizeof header.reserved1, 1, file);
    file_rawRead(8,             &header.reserved2, sizeof header.reserved2, 1, file);
    file_rawRead(BITMAP_OFFSET, &header.offset,    sizeof header.offset,    1, file);

    // 2) Lire BITMAPINFOHEADER champ par champ
    t_bmp_info info;
    file_rawRead(14,              &info.size,        sizeof info.size,        1, file);
    file_rawRead(BITMAP_WIDTH,    &info.width,       sizeof info.width,       1, file);
    file_rawRead(BITMAP_HEIGHT,   &info.height,      sizeof info.height,      1, file);
    file_rawRead(26,              &info.planes,      sizeof info.planes,      1, file);
    file_rawRead(BITMAP_DEPTH,    &info.bits,        sizeof info.bits,        1, file);
    file_rawRead(30,              &info.compression,  sizeof info.compression,  1, file);
    file_rawRead(34,              &info.imagesize,   sizeof info.imagesize,   1, file);
    file_rawRead(38,              &info.xresolution, sizeof info.xresolution, 1, file);
    file_rawRead(42,              &info.yresolution, sizeof info.yresolution, 1, file);
    file_rawRead(46,              &info.ncolors,     sizeof info.ncolors,     1, file);
    file_rawRead(50,              &info.importantcolors, sizeof info.importantcolors, 1, file);

    // 3) Récupérer les dimensions
    int width      = info.width;
    int height     = info.height;
    int colorDepth = info.bits;
    printf("DEBUG: width=%d, height=%d, bits=%d, pixelOffset=%u\n",
           width, height, colorDepth, header.offset);

    // 4) Allouer structure + data
    t_bmp24 *img = bmp24_allocate(width, height, colorDepth);
    if (!img) {
        fclose(file);
        return NULL;
    }

    // 5) Copier les en-têtes
    img->header      = header;
    img->header_info = info;

    // 6) Lire la matrice pixel (bottom-up, BGR→RGB)
    bmp24_readPixelData(img, file);

    fclose(file);
    return img;
}




void bmp24_saveImage(t_bmp24 *img, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Erreur création BMP 24-bits");
        return;
    }

    // 1) Ecrire BITMAPFILEHEADER (14 octets)
    //    type, size, reserved1, reserved2, offsetToPixels
    file_rawWrite(0, &img->header.type,      sizeof(img->header.type),      1, file);
    file_rawWrite(2, &img->header.size,      sizeof(img->header.size),      1, file);
    file_rawWrite(6, &img->header.reserved1, sizeof(img->header.reserved1), 1, file);
    file_rawWrite(8, &img->header.reserved2, sizeof(img->header.reserved2), 1, file);
    file_rawWrite(10, &img->header.offset,   sizeof(img->header.offset),   1, file);

    // 2) Ecrire BITMAPINFOHEADER (40 octets)
    size_t infoOff = HEADER_SIZE; // 14
    file_rawWrite(infoOff + 0,  &img->header_info.size,         sizeof(img->header_info.size),         1, file);
    file_rawWrite(infoOff + 4,  &img->header_info.width,        sizeof(img->header_info.width),        1, file);
    file_rawWrite(infoOff + 8,  &img->header_info.height,       sizeof(img->header_info.height),       1, file);
    file_rawWrite(infoOff + 12, &img->header_info.planes,       sizeof(img->header_info.planes),       1, file);
    file_rawWrite(infoOff + 14, &img->header_info.bits,         sizeof(img->header_info.bits),         1, file);
    file_rawWrite(infoOff + 16, &img->header_info.compression,  sizeof(img->header_info.compression),  1, file);
    file_rawWrite(infoOff + 20, &img->header_info.imagesize,    sizeof(img->header_info.imagesize),    1, file);
    file_rawWrite(infoOff + 24, &img->header_info.xresolution,  sizeof(img->header_info.xresolution),  1, file);
    file_rawWrite(infoOff + 28, &img->header_info.yresolution,  sizeof(img->header_info.yresolution),  1, file);
    file_rawWrite(infoOff + 32, &img->header_info.ncolors,      sizeof(img->header_info.ncolors),      1, file);
    file_rawWrite(infoOff + 36, &img->header_info.importantcolors, sizeof(img->header_info.importantcolors), 1, file);

    // 3) Positionner à BMP24_DATA_OFFSET (54)
    fseek(file, BMP24_DATA_OFFSET, SEEK_SET);

    // 4) Écrire les pixels ligne par ligne, avec padding si nécessaire
    int w = img->width, h = img->height;
    int rowSize = w * 3;
    int pad = (4 - (rowSize % 4)) % 4;
    unsigned char zero = 0;

    for (int y = h - 1; y >= 0; y--) {
        // pour chaque pixel de la ligne y
        for (int x = 0; x < w; x++) {
            unsigned char bgr[3] = {
                img->data[y][x].blue,
                img->data[y][x].green,
                img->data[y][x].red
            };
            fwrite(bgr, 1, 3, file);
        }
        // padding
        for (int p = 0; p < pad; p++) {
            fwrite(&zero, 1, 1, file);
        }
    }

    fclose(file);
}

void bmp24_negative(t_bmp24 *img) {
}

void bmp24_grayscale(t_bmp24 *img) {
}

void bmp24_brightness(t_bmp24 *img, int value) {
}

t_pixel bmp24_convolution(t_bmp24 *img, int x, int y, float **kernel, int kernelSize) {
    t_pixel p = {0, 0, 0};
    return p;
}

void bmp24_boxBlur(t_bmp24 *img) {
}

void bmp24_gaussianBlur(t_bmp24 *img) {
}

void bmp24_outline(t_bmp24 *img) {
}

void bmp24_emboss(t_bmp24 *img) {
}

void bmp24_sharpen(t_bmp24 *img) {
}
