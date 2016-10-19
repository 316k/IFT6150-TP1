/*------------------------------------------------------*/
/* Prog    : TpIFT6150-1-Aa.c                           */
/* Auteurs : Nicolas Hurtubise, Mercedes Gauthier       */
/* Date    : 18/10/2016                                 */
/* version : 1                                          */
/* langage : C                                          */
/* labo    : DIRO                                       */
/*------------------------------------------------------*/

/*------------------------------------------------*/
/* FICHIERS INCLUS -------------------------------*/
/*------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "FonctionDemo1.h"

/*------------------------------------------------*/
/* DEFINITIONS -----------------------------------*/
/*------------------------------------------------*/
#define NAME_IMG_IN  "D1r"

#define NAME_IMG_OUT "image-TpIFT6150-1-Ba"

/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/
/*------------------------------------------------*/

void PreFFT_Translation(float** Matrix, int length, int width);

int main(int argc,char **argv)
 {
  int i,j,k, min, max;
  int length,width;
  float** MatriceImgR;
  float** MatriceImgI;
  float** CumulR;
  float** CumulI;
  float** CumulRecaleeR;
  float** CumulRecaleeI;
  
  char name[31];

  int mins[] = {0, 2, 6, 16};
  int maxs[] = {1, 5, 15, 63};
  
  for(k=0; k<4; k++) {
      /*Allocation memoire pour la matrice image*/
      
      min = mins[k];
      max = maxs[k];
      
      MatriceImgR=LoadImagePgm(NAME_IMG_IN,&length,&width);
      MatriceImgI=fmatrix_allocate_2d(length,width);

      // Première itération : initialiser CumulR et CumulI
      if(k == 0) {
          CumulR=fmatrix_allocate_2d(length,width);
          CumulI=fmatrix_allocate_2d(length,width);
          CumulRecaleeR=fmatrix_allocate_2d(length,width);
          CumulRecaleeI=fmatrix_allocate_2d(length,width);
      
          for(i=0;i<length;i++)
              for(j=0;j<width;j++) {
                  CumulR[i][j] = 0.0;
                  CumulI[i][j] = 0.0;
                  CumulRecaleeR[i][j] = 0.0;
                  CumulRecaleeI[i][j] = 0.0;
              }
      }
      
      /*Initialisation a zero de toutes les matrices */
      for(i=0;i<length;i++)
          for(j=0;j<width;j++) {
              MatriceImgI[i][j] = 0.0;
          }
      
      PreFFT_Translation(MatriceImgR, length, width);
      
      /*FFT*/
      FFTDD(MatriceImgR,MatriceImgI,length,width);
      int nb_zeros = 0;
      
      // Filtre passe-bande
      for(i=0; i<length; i++) {
          for(j=0; j<width; j++) {
              if((i > width/2 - min && i < width/2 + min && j > width/2 - min && j < width/2 + min) ||
                 (i < width/2 - max || i > width/2 + max || j < width/2 - max || j > width/2 + max)) {

                  MatriceImgR[i][j] = 0.0;
                  MatriceImgI[i][j] = 0.0;
              }
          }
      }

      // On accumule graduellement les fréquences de plus en plus hautes
      for(i=0; i<length; i++) {
          for(j=0; j<width; j++) {
              CumulR[i][j] += MatriceImgR[i][j];
              CumulI[i][j] += MatriceImgI[i][j];
              CumulRecaleeR[i][j] = CumulR[i][j];
              CumulRecaleeI[i][j] = CumulI[i][j];
          }
      }

      IFFTDD(CumulRecaleeR, CumulRecaleeI, length, width);
  
      PreFFT_Translation(CumulRecaleeR, length, width);

      Recal(CumulRecaleeR, length, width);
      
      sprintf(name, "image-TpIFT6150-1-Bb_%d_%d", min, max);

      /*Sauvegarde de MatriceImgM sous forme d'image pgm*/
      SaveImagePgm(name,CumulRecaleeR,length,width);
      
      /*Liberation memoire pour les matrices*/
      free_fmatrix_2d(MatriceImgR);
      free_fmatrix_2d(MatriceImgI);

  }
  
  free_fmatrix_2d(CumulR);
  free_fmatrix_2d(CumulI);
  free_fmatrix_2d(CumulRecaleeR);
  free_fmatrix_2d(CumulRecaleeI);  

  return 0;
}

void PreFFT_Translation(float** Matrix, int length, int width) {
    int x,y;
    for(x=0; x<length; x++)
        for(y=0; y<width; y++)
            if((x + y) % 2 == 1)
                Matrix[x][y] = -Matrix[x][y];
}
