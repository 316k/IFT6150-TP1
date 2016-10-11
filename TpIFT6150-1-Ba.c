/*------------------------------------------------------*/
/* Prog    : TpIFT6150-1-Aa.c                           */
/* Auteur  : Nicolas Hurtubise                          */
/* Date    : 01/09/2016                                 */
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

  int mins[] = {0, 2, 6, 16};
  int maxs[] = {1, 5, 15, 63};
  
  for(k=0; k<4; k++) {
      /*Allocation memoire pour la matrice image*/
      MatriceImgR=LoadImagePgm(NAME_IMG_IN,&length,&width);
      MatriceImgI=fmatrix_allocate_2d(length,width);
      
      /*Initialisation a zero de toutes les matrices */
      for(i=0;i<length;i++)
          for(j=0;j<width;j++) {
              MatriceImgI[i][j]=0.0;
          }
      
      PreFFT_Translation(MatriceImgR, length, width);
      
      /*FFT*/
      FFTDD(MatriceImgR,MatriceImgI,length,width);
      
      min = mins[k];
      max = maxs[k];
      
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

      IFFTDD(MatriceImgR, MatriceImgI, length, width);
  
      PreFFT_Translation(MatriceImgR, length, width);

      Recal(MatriceImgR, length, width);
      
      char name[31];
      sprintf(name, "image-TpIFT6150-1-Ba_%d_%d", min, max);
      
      /*Sauvegarde de MatriceImgM sous forme d'image pgm*/
      SaveImagePgm(name,MatriceImgR,length,width);

      /*Liberation memoire pour les matrices*/
      free_fmatrix_2d(MatriceImgR);
      free_fmatrix_2d(MatriceImgI);
  }
  
  return 0;
}

void PreFFT_Translation(float** Matrix, int length, int width) {
    int x,y;
    for(x=0; x<length; x++)
        for(y=0; y<width; y++)
            if((x + y) % 2 == 1)
                Matrix[x][y] = -Matrix[x][y];
}
