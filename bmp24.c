


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

void bmp24_printInfo(t_bmp24 *img) {
    if (!img) {
        printf("Aucune image chargée.\n");
        return;
    }
    printf("Image Info (24 bits):\n   ");
    printf("Width: %d\n   ", img->width);
    printf("Height: %d\n   ", img->height);
    printf("Color Depth: %d bits/pixel\n   ", img->colorDepth);
    // Calcul de la taille brute des pixels (sans header ni palettes)
    unsigned int dataSize = img->width * img->height * 3;
    printf("Data Size: %u bytes\n", dataSize);
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

// Convertit une couleur (R,G,B) en composantes YUV (luminance + chrominance)
void rgb_to_yuv(unsigned char R, unsigned char G, unsigned char B, double *Y, double *U, double *V) {
    *Y = 0.299 * R + 0.587 * G + 0.114 * B;
    *U = -0.14713 * R - 0.28886 * G + 0.436 * B;
    *V = 0.615 * R - 0.51499 * G - 0.10001 * B;
}

// Convertit une couleur (Y,U,V) en (R,G,B), avec correction de débordement [0,255]
void yuv_to_rgb(double Y, double U, double V, unsigned char *R, unsigned char *G, unsigned char *B) {
  // Calcul des canaux RGB à partir de YUV (formules inverses)
    int r = round(Y + 1.13983 * V);
    int g = round(Y - 0.39465 * U - 0.58060 * V);
    int b = round(Y + 2.03211 * U);

   //nécessité de rester dans les bornes 0 et 255
    if (r < 0) r = 0; if (r > 255) r = 255;
    if (g < 0) g = 0; if (g > 255) g = 255;
    if (b < 0) b = 0; if (b > 255) b = 255;

    *R = (unsigned char)r;
    *G = (unsigned char)g;
    *B = (unsigned char)b;
}


// Améliore le contraste d'une image couleur 24 bits par égalisation de l'histogramme de luminance (Y)
void bmp24_equalizeColor(t_bmp24 *img) {
    if (!img || !img->data) {
        printf("Image invalide\n");
        return;
    }

    int width = img->width;
    int height = img->height;
    int size = width * height;
    // Allocation des canaux YUV (luminance et chrominance)
    double *Y = malloc(size * sizeof(double));
    double *U = malloc(size * sizeof(double));
    double *V = malloc(size * sizeof(double));

    if (!Y || !U || !V) {
        printf("Erreur d’allocation mémoire YUV\n");
        free(Y);
        free(U);
        free(V);
        return;
    }

    // Étape 1 : conversion RGB → YUV
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int idx = i * width + j;
            t_pixel pixel = img->data[i][j];
            rgb_to_yuv(pixel.red, pixel.green, pixel.blue, &Y[idx], &U[idx], &V[idx]);
        }
    }

    // Étape 2 : histogramme de luminance Y
    unsigned int hist[256] = {0};
    for (int i = 0; i < size; i++) {
        int y = round(Y[i]);
        if (y < 0) {
          y = 0;
        }
        if (y > 255){
          y = 255;
        }
        hist[y]++;
    }

    // Étape 3 : Construction de la CDF (fonction de répartition cumulée)
    unsigned int cdf[256] = {0};
    cdf[0] = hist[0];
    for (int i = 1; i < 256; i++) {
        cdf[i] = cdf[i - 1] + hist[i];
    }

    //Recherche de la première valeur non nulle
    unsigned int total = cdf[255];
    unsigned int cdfmin = 0;
    for (int i = 0; i < 256; i++) {
        if (cdf[i] > 0) {
            cdfmin = cdf[i];
            break;
        }
    }

    // Génère une table de correspondance (valeurs égalisé)
    unsigned char map[256];
    for (int i = 0; i < 256; i++) {
        if (total == cdfmin) {
            map[i] = i;
        } else {
            map[i] = round(((double)(cdf[i] - cdfmin) / (total - cdfmin)) * 255);
        }
    }

    // Étape 4 : égalisation de Y : remplace chaque valeur Y par sa version égalisée
    for (int i = 0; i < size; i++) {
        int y = round(Y[i]);
        if (y < 0) y = 0;
        if (y > 255) y = 255;
        Y[i] = map[y];
    }

    // Étape 5 : reconversion YUV → RGB
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int idx = i * width + j;
            unsigned char r, g, b;
            yuv_to_rgb(Y[idx], U[idx], V[idx], &r, &g, &b);
            img->data[i][j].red = r;
            img->data[i][j].green = g;
            img->data[i][j].blue = b;
        }
    }

    free(Y); free(U); free(V);
}



