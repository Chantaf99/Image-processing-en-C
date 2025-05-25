#ifndef BMP24_H
#define BMP24_H

#include <stdint.h>
#include <stdio.h>  // pour FILE
#include <stddef.h> // pour size_t
#include <string.h>
// Définition du pixel (doit venir avant t_bmp24)
typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} t_pixel;

// En-tête BMP file header
typedef struct {
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;    // BITMAP_OFFSET
} t_bmp_header;

// En-tête BMP info header
typedef struct {
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bits;      // colorDepth
    uint32_t compression;
    uint32_t imagesize;
    int32_t xresolution;
    int32_t yresolution;
    uint32_t ncolors;
    uint32_t importantcolors;
} t_bmp_info;

// Structure principale BMP24
typedef struct {
    t_bmp_header header;
    t_bmp_info   header_info;
    int          width;       // largeur
    int          height;      // hauteur
    int          colorDepth;  // en bits
    t_pixel    **data;        // matrice [height][width]
    uint8_t  *rawHeader;    // pointeur vers les header.offset octets
    uint32_t  rawHeaderSize;// vaut header.offset
} t_bmp24;

// Offsets dans l'en-tête
#define BITMAP_MAGIC   0x00
#define BITMAP_SIZE    0x02
#define BITMAP_OFFSET  0x0A
#define BITMAP_WIDTH   0x12
#define BITMAP_HEIGHT  0x16
#define BITMAP_DEPTH   0x1C
#define BITMAP_SIZE_RAW 0x22 // offset 34
// Constante pour le type de fichier BMP
#define BMP_TYPE 0x4D42 // 'BM' en hexadécimal
// Constantes pour les tailles des champs de l'en-tête BMP
#define HEADER_SIZE 0x0E // 14 octets
#define INFO_SIZE  0x28 // 40 octets
#define BMP24_DATA_OFFSET  (HEADER_SIZE + INFO_SIZE)
// Constantes pour les valeurs de profondeur de couleur

#define DEFAULT_DEPTH 0x18 // 2

// Allocation / libération
t_pixel ** bmp24_allocateDataPixels(int width, int height);
void       bmp24_freeDataPixels    (t_pixel **pixels, int height);
t_bmp24  * bmp24_allocate          (int width, int height, int colorDepth);
void       bmp24_free              (t_bmp24 *img);

// I/O brut
void file_rawRead  (uint32_t position, void *buffer, uint32_t size, size_t n, FILE *file);
void file_rawWrite (uint32_t position, void *buffer, uint32_t size, size_t n, FILE *file);

// Lecture/écriture pixel
void bmp24_readPixelValue(t_bmp24 * img,  int x, int y, FILE * file);
void bmp24_readPixelData(t_bmp24 *img, FILE *file) ;
void bmp24_writePixelValue(t_bmp24 *image, int x, int y, FILE *file);
void bmp24_writePixelData (t_bmp24 *image, FILE *file);

// Chargement / sauvegarde
t_bmp24 *bmp24_loadImage(const char *filename);
void bmp24_saveImage(t_bmp24 *img, const char *filename);

// Autres traitements (stubs)
void     bmp24_negative    (t_bmp24 *img);
void     bmp24_grayscale   (t_bmp24 *img);
void     bmp24_brightness  (t_bmp24 *img, int value);
t_pixel  bmp24_convolution (t_bmp24 *img, int x, int y, float **kernel, int kernelSize);
void bmp24_applyFilter(t_bmp24 *img, float **kernel, int kernelSize);
void apply_and_save_24(const char *srcFilename, const char *outFilename, float **kernel, int kernelSize);

//Egalisation d'histogramme en couleur
void rgb_to_yuv(unsigned char R, unsigned char G, unsigned char B, double *Y, double *U, double *V);
void yuv_to_rgb(double Y, double U, double V, unsigned char *R, unsigned char *G, unsigned char *B);
void bmp24_equalizeColor(t_bmp24 *img);

#endif // BMP24_H
