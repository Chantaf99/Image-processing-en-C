# 🖼️ Image Processing en C

Ce projet de traitement d'images en C a été réalisé par **Ali EL HAJJAM** et **Charaf EL MANSARI** dans le cadre du module **TI202 - Algorithmique et Structures de Données 1**.

## Objectif

Développer un programme en C capable de :

- Charger et afficher des images BMP en niveaux de gris (8 bits) et en couleur (24 bits)
- Appliquer des transformations simples (négatif, binarisation, luminosité, flou, etc.)
- Réaliser une **égalisation d’histogramme**, pour améliorer le contraste des images
- Gérer une **interface simple en ligne de commande**
- Crer un menu intéractif qui regroupe plusieurs fonctionalités 

---

## Compilation

-Avec GCC : gcc main.c bmp8.c bmp24.c kernels.c filtres_convolution.c -o main.exe

## Fonctionnalités

Chargement/Sauvegarde d’images BMP (8 bits et 24 bits)

- Menu intéractif en ligne de commande

-Opérations : négatif, luminosité, binarisation

-Le programme applique divers **filtres d’image** (flou, contours, netteté…) en utilisant la **convolution 2D** avec des **kernels** (matrices) sur chaque pixel.  
Chaque filtre est représenté par une matrice (ex: 3x3) appliquée sur l’image en niveaux de gris ou en couleur, pour modifier localement les pixels selon leur voisinage.  
Les filtres incluent : **flou moyen**, **flou gaussien**, **détection de contours**, **netteté**, etc.

-Calcul d’histogrammes et de CDF

-Égalisation d’histogramme en niveaux de gris

-Égalisation d’histogramme en couleur (conversion YUV)

## Auteurs

EL MANSARI CHARAF - EL HAJJAM ALI
