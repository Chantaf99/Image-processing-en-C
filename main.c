#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "bmp8.h"
#include "bmp24.h"
#include "kernels.h"
#include "filtres_convolution.h"

// Chemin fixe où on stocke/tire les BMP
#define BASE_PATH "C:\\Users\\chara\\Programmes_C\\Image-processing-en-C\\"

// Type courant d'image chargée
enum ImageType { NONE, GRAY8, COLOR24 };

int main(void) {
    enum ImageType type = NONE;
    t_bmp8  *img8  = NULL;
    t_bmp24 *img24 = NULL;
    char filename[256];
    char fullpath[512];
    int choice;

    while (1) {
        // ——— Menu Principal ———————————————————————————————
        printf("\n==== Menu Principal ====\n");
        printf("1. Ouvrir une image BMP\n");
        printf("2. Sauvegarder l'image courante\n");
        printf("3. Appliquer un filtre\n");
        printf("4. Afficher les infos de l'image\n");
        printf("5. Quitter\n");
        printf(">>> Votre choix : ");
        if (scanf("%d", &choice)!=1) {
            // Si pas un entier, on nettoie et on recommence
            int c; while((c=getchar())!='\n' && c!=EOF);
            continue;
        }
        // Vider le '\n' restant
        int c; while((c=getchar())!='\n' && c!=EOF);

        switch (choice) {
          case 1:
            // — Ouvrir une image
            printf("Nom du fichier BMP (ex: foo.bmp) : ");
            if (!fgets(filename, sizeof(filename), stdin)) break;
            filename[strcspn(filename,"\n")] = '\0';

            // Construire chemin complet
            snprintf(fullpath, sizeof(fullpath), "%s%s", BASE_PATH, filename);

            // Libérer l’ancienne image
            if (img8)  { bmp8_free(img8);   img8  = NULL; }
            if (img24) { bmp24_free(img24); img24 = NULL; }

            // Tenter 8 bits
            img8 = bmp8_loadImage(fullpath);
            if (img8) {
                type = GRAY8;
                printf("-> Image 8-bits chargee : %ux%u\n",
                       img8->width, img8->height);
            }
            else {
                // Sinon 24 bits
                img24 = bmp24_loadImage(fullpath);
                if (img24) {
                    type = COLOR24;
                    printf("-> Image 24-bits chargee : %dx%d\n",
                           img24->width, img24->height);
                }
                else {
                    type = NONE;
                    printf("-> Echec du chargement de '%s'\n", fullpath);
                }
            }
            break;

          case 2:
            // — Sauvegarder l’image courante
            if (type==NONE) {
                printf("Aucune image chargee.\n");
                break;
            }
            printf("Nom de sortie (ex: out.bmp) : ");
            if (!fgets(filename, sizeof(filename), stdin)) break;
            filename[strcspn(filename,"\n")] = '\0';
            snprintf(fullpath, sizeof(fullpath), "%s%s", BASE_PATH, filename);

            if (type==GRAY8) {
                bmp8_saveImage(fullpath, img8);
            } else {
                bmp24_saveImage(img24, fullpath);
            }
            break;

          case 3:
            // — Appliquer un filtre
            if (type==NONE) {
                printf("Aucune image chargee.\n");
                break;
            }
            {
              int f;
              printf("Choisissez un filtre :\n");
              printf(" 1.Negatif  2.Luminosite(+50)  3.Seuillage(128)\n");
              printf(" 4.Flou    5.Gaussien    6.Contours\n");
              printf(" 7.Relief  8.Nettete    9.Annuler\n");
              printf(">>> "); scanf("%d",&f);
              while((c=getchar())!=EOF&&c!='\n');

              if (f==9) break;

              // Filtre 8-bits ou 24-bits
              if (type==GRAY8) {
                switch(f) {
                  case 1: bmp8_negative(img8);                      break;
                  case 2: bmp8_brightness(img8, 50);                break;
                  case 3: bmp8_threshold(img8, 128);                break;
                  case 4: bmp8_applyFilter(img8, box_blur, 3);      break;
                  case 5: bmp8_applyFilter(img8, gaussian_blur, 3); break;
                  case 6: bmp8_applyFilter(img8, outline, 3);       break;
                  case 7: bmp8_applyFilter(img8, emboss, 3);        break;
                  case 8: bmp8_applyFilter(img8, sharpen, 3);       break;
                  default: printf("Filtre invalide.\n"); continue;
                }
              } else {
                switch(f) {
                  case 1: bmp24_negative(img24);                     break;
                  case 2: bmp24_brightness(img24, 50);              break;
                  case 3: bmp24_grayscale(img24);                    break;
                  case 4: bmp24_applyFilter(img24, box_blur, 3);     break;
                  case 5: bmp24_applyFilter(img24, gaussian_blur,3); break;
                  case 6: bmp24_applyFilter(img24, outline, 3);      break;
                  case 7: bmp24_applyFilter(img24, emboss, 3);       break;
                  case 8: bmp24_applyFilter(img24, sharpen, 3);      break;
                  default: printf("Filtre invalide.\n"); continue;
                }
              }
              printf("-> Filtre applique.\n");
            }
            break;

          case 4:
            // — Afficher les infos
            if (type==GRAY8)  bmp8_printInfo(img8);
            else if (type==COLOR24) bmp24_printInfo(img24);
            else printf("Aucune image chargee.\n");
            break;

          case 5:
            // — Quitter
            if (img8 ) bmp8_free(img8);
            if (img24) bmp24_free(img24);
            printf("Au revoir !\n");
            return 0;

          default:
            printf("Choix invalide.\n");
        } // switch
    } // while

    //Egalisation d'histogramme en gris
    t_bmp8 *imgP38 = bmp8_loadImage("image_brightness.bmp");
    if (!imgP38 || imgP38->data == NULL) {
        printf("Erreur chargement image\n");
        return 1;
    }

    unsigned int *hist = bmp8_computeHistogram(imgP38);
    unsigned int *cdf = bmp8_computeCDF(hist);
    bmp8_equalize(imgP38, cdf);

    bmp8_saveImage("imagegris_equalized.bmp", imgP38);
    bmp8_free(imgP38);
    free(hist);
    free(cdf);
    //Egalisation d'histogramme en couleur
    t_bmp24 *imgP324 = bmp24_loadImage("flowers_color.bmp");
    if (!imgP324) {
        printf("Erreur chargement image\n");
        return 1;
    }

    bmp24_equalizeColor(imgP324);

    bmp24_saveImage(imgP324, "flowers_equalized.bmp");
    bmp24_free(imgP324);
    printf("Image Flowers equalized bien chargee");
    return 0;
}
