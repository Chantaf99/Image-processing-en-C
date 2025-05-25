


#include "bmp24.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    //lire l'offset du fichier BMP
    uint32_t offset  = img->header.offset;
    // taille d'une ligne en octets (3 octets par pixel)
    uint32_t rowSize = img->width * 3;
    // inversion verticale (BMP stocke du bas vers le haut)
    uint32_t fileY = img->height - 1 - y;
    // position dans le fichier
    uint32_t pos = offset + fileY * rowSize + x * 3;

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
    uint32_t offset  = img->header.offset;
    uint32_t rowsize = img->width*3;
    uint32_t fileY = (uint32_t)img->height - 1 - (uint32_t) y; 
    uint32_t position = offset + fileY * rowsize + (uint32_t)x * 3;

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
    file_rawRead(BITMAP_OFFSET, &header.offset,    sizeof header.offset,    1, file);
    uint32_t hdrSize = header.offset;
    uint8_t *hdrBuf  = malloc(hdrSize);
    if (!hdrBuf) { perror("malloc rawHeader"); fclose(file); return NULL; }
    fseek(file, 0, SEEK_SET);
    fread(hdrBuf, 1, hdrSize, file);

    // 2) Lire BITMAPINFOHEADER champ par champ
    t_bmp_info info;
    memcpy(&info.width,  hdrBuf + BITMAP_WIDTH,  sizeof info.width);
    memcpy(&info.height, hdrBuf + BITMAP_HEIGHT, sizeof info.height);
    memcpy(&info.bits,   hdrBuf + BITMAP_DEPTH,  sizeof info.bits);

    // 3) Récupérer les dimensions
    int width      = info.width;
    int height     = info.height;
    int colorDepth = info.bits;


    // 4) Allouer structure + data
    t_bmp24 *img = bmp24_allocate(width, height, colorDepth);
    if (!img) {
        free(hdrBuf);
        fclose(file);
        return NULL;
    }

    // 5) Copier les en-têtes
    img->header        = header;
    img->header_info   = info;
    img->rawHeader     = hdrBuf;
    img->rawHeaderSize = hdrSize;
    img->width         = width;
    img->height        = height;
    img->colorDepth    = colorDepth;

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
    // 1) Réécrire à l’identique les rawHeaderSize octets
    fwrite(img->rawHeader, 1, img->rawHeaderSize, file);

    // 2) Écrire la matrice pixel à partir de header.offset
    bmp24_writePixelData(img, file);

    fclose(file);
}

void bmp24_negative(t_bmp24 *img) {
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            img->data[y][x].red   = 255 - img->data[y][x].red;
            img->data[y][x].green = 255 - img->data[y][x].green;
            img->data[y][x].blue  = 255 - img->data[y][x].blue;
        }
    }
}

void bmp24_grayscale(t_bmp24 *img) {
    for(int i = 0; i < img->height; i++) {
        for(int j = 0; j < img->width; j++) {
            unsigned int moy= (img->data[i][j].red + img->data[i][j].green + img->data[i][j].blue) / 3;
            img->data[i][j].red   = moy;
            img->data[i][j].green = moy;
            img->data[i][j].blue  = moy;
        }
    }
}

void bmp24_brightness(t_bmp24 *img, int value) {
    for(int y = 0; y < img->height; y++) {
        for(int x = 0; x < img->width; x++) {
            int nr = img->data[y][x].red + value;
            int ng = img->data[y][x].green + value;
            int nb = img->data[y][x].blue + value;
            // Ajustement des valeurs entre 0 and 255
            if (nr > 255) nr = 255;
            if (nr <   0) nr = 0;
            if (ng > 255) ng = 255;
            if (ng <   0) ng = 0;
            if (nb > 255) nb = 255;
            if (nb <   0) nb = 0;

            img->data[y][x].red   = (uint8_t) nr;
            img->data[y][x].green = (uint8_t) ng;
            img->data[y][x].blue  = (uint8_t) nb;
        }
    }
}

t_pixel bmp24_convolution(t_bmp24 *img, int x, int y, float **kernel, int kernelSize) {
    int half = kernelSize / 2;
    float r = 0.0f, g = 0.0f, b = 0.0f;

    for (int ky = -half; ky <= half; ky++) {
        for (int kx = -half; kx <= half; kx++) {
            int ix = x + kx;
            int iy = y + ky;

            // Ignore les pixels hors limites
            if (ix < 0 || ix >= img->width || iy < 0 || iy >= img->height)
                continue;

            float coef = kernel[ky + half][kx + half];
            t_pixel px = img->data[iy][ix];

            r += px.red   * coef;
            g += px.green * coef;
            b += px.blue  * coef;
        }
    }

    // Clamping + arrondi
    if (r < 0.0f) r = 0.0f; else if (r > 255.0f) r = 255.0f;
    if (g < 0.0f) g = 0.0f; else if (g > 255.0f) g = 255.0f;
    if (b < 0.0f) b = 0.0f; else if (b > 255.0f) b = 255.0f;

    t_pixel result = {
        .red   = (uint8_t)(r + 0.5f),
        .green = (uint8_t)(g + 0.5f),
        .blue  = (uint8_t)(b + 0.5f)
    };

    return result;
}

void bmp24_applyFilter(t_bmp24 *img, float **kernel, int kernelSize) {
    int w = img->width;
    int h = img->height;

    // Allouer un nouveau tableau de pixels
    t_pixel **newData = malloc(h * sizeof(t_pixel *));
    for (int y = 0; y < h; y++) {
        newData[y] = malloc(w * sizeof(t_pixel));
    }

    // Appliquer la convolution à chaque pixel
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            newData[y][x] = bmp24_convolution(img, x, y, kernel, kernelSize);
        }
    }

    // Libérer l'ancien tableau
    for (int y = 0; y < h; y++) {
        free(img->data[y]);
    }
    free(img->data);

    // Remplacer par le nouveau
    img->data = newData;
}

void apply_and_save_24(const char *srcFilename, const char *outFilename, float **kernel, int kernelSize) {
    t_bmp24 *img = bmp24_loadImage(srcFilename);

    if (!img) {
        printf("Erreur : impossible de charger l'image '%s'\n", srcFilename);
        return;
    }

    bmp24_applyFilter(img, kernel, kernelSize);
    bmp24_saveImage(img, outFilename);

    printf("Image traitee et sauvegardee dans '%s'\n", outFilename);
    bmp24_free(img);
    }



