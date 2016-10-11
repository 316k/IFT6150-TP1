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
/* PROGRAMME PRINCIPAL   -------------------------*/
/*------------------------------------------------*/

void PreFFT_Translation(float** Matrix, int length, int width);

int main(int argc,char **argv)
 {
  int i,j,k;
  int length=128,width=128;
  float** MatriceImgR;
  float** MatriceImgR_ref;
  float** MatriceImgI;
  float** MatriceImgMod;

  /*Allocation memoire pour la matrice image*/
  MatriceImgR = fmatrix_allocate_2d(length,width);
  MatriceImgR_ref = fmatrix_allocate_2d(length,width);
  MatriceImgI = fmatrix_allocate_2d(length,width);
  MatriceImgMod = fmatrix_allocate_2d(length,width);

  /*Initialisation des matrices */
  for(i=0;i<length;i++)
      for(j=0;j<width;j++) {
          
          if(i > 2 * length/5.0 && i < 3 * length / 5.0 && j > 2 * width/5.0 && j < 3 * width / 5.0) {
              MatriceImgR[i][j] = 1/(length/5.0 * width/5.0);
              MatriceImgR_ref[i][j] = 255.0;
          } else {
              MatriceImgR[i][j] = 0.0;
              MatriceImgR_ref[i][j] = 0.0;
          }

          MatriceImgMod[i][j] = 0.0;
          MatriceImgI[i][j]=0.0;
      }

  float sum = 0;
  for(i=0; i<length; i++)
      for(j=0; j<width; j++)
          sum += MatriceImgR[i][j];

  printf("%lf\n", sum);

  SaveImagePgm("image-TpIFT6150-1-Ca_square", MatriceImgR_ref,length,width);
  
  /*FFT*/
  FFTDD(MatriceImgR,MatriceImgI,length,width);

  float reel, imaginaire;
  for(i=0; i<length; i++)
      for(j=0; j<width; j++) {
          
          reel = MatriceImgR[i][j];
          imaginaire = MatriceImgI[i][j];

          MatriceImgR[i][j] = reel * reel - imaginaire * imaginaire;
          MatriceImgI[i][j] = 2 * reel * imaginaire;
      }

  PreFFT_Translation(MatriceImgR, length, width);
  
  IFFTDD(MatriceImgR, MatriceImgI, length, width);
  
  Recal(MatriceImgR, length, width);
  
  SaveImagePgm("image-TpIFT6150-1-Ca_convolution", MatriceImgR, length, width);

  /*Liberation memoire pour les matrices*/
  free_fmatrix_2d(MatriceImgR);
  free_fmatrix_2d(MatriceImgI);
  
  return 0;
}

void PreFFT_Translation(float** Matrix, int length, int width) {
    int x,y;
    for(x=0; x<length; x++)
        for(y=0; y<width; y++)
            if((x + y) % 2 == 1)
                Matrix[x][y] = -Matrix[x][y];
}
