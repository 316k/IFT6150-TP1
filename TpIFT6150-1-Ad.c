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
#define NAME_IMG_IN1  "D11r"
#define NAME_IMG_IN2  "D46r"

#define NAME_IMG_OUT1 "image-TpIFT6150-1-Ad-d11r"
#define NAME_IMG_OUT2 "image-TpIFT6150-1-Ad-d46r"

/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/
/*------------------------------------------------*/

void PreFFT_Translation(float** Matrix, int length, int width);

int main(int argc,char **argv)
 {
  int i,j,k;
  int length,width;
  float** MatriceImgR1;
  float** MatriceImgI1;
  float** MatriceImgM1;

  float** MatriceImgR2;
  float** MatriceImgI2;
  float** MatriceImgM2;
  
  /*Allocation memoire pour la matrice image*/
  MatriceImgR1=LoadImagePgm(NAME_IMG_IN1,&length,&width);
  MatriceImgI1=fmatrix_allocate_2d(length,width);
  MatriceImgM1=fmatrix_allocate_2d(length,width);

  MatriceImgR2=LoadImagePgm(NAME_IMG_IN2,&length,&width);
  MatriceImgI2=fmatrix_allocate_2d(length,width);
  MatriceImgM2=fmatrix_allocate_2d(length,width);
  
  /*Initialisation a zero de toutes les matrices */
  for(i=0;i<length;i++)
      for(j=0;j<width;j++) {
          MatriceImgI1[i][j]=0.0;
          MatriceImgM1[i][j]=0.0;

          MatriceImgI2[i][j]=0.0;
          MatriceImgM2[i][j]=0.0;
      }

  PreFFT_Translation(MatriceImgR1, length, width);
  PreFFT_Translation(MatriceImgR2, length, width);
  
  /*FFT*/
  FFTDD(MatriceImgR1,MatriceImgI1,length,width);
  FFTDD(MatriceImgR2,MatriceImgI2,length,width);

  /*Module*/
  Mod(MatriceImgM1,MatriceImgR1,MatriceImgI1,length,width);
  Mod(MatriceImgM2,MatriceImgR2,MatriceImgI2,length,width);

  for(i=0; i<length; i++)
      for(j=0; j<width; j++) {
          MatriceImgM1[i][j] = logf(1 + MatriceImgM1[i][j]);
          MatriceImgM2[i][j] = logf(1 + MatriceImgM2[i][j]);
      }

  Recal(MatriceImgM1,length,width);
  Recal(MatriceImgM2,length,width);

  
  /*Sauvegarde de MatriceImgM sous forme d'image pgm*/
  SaveImagePgm(NAME_IMG_OUT1,MatriceImgM1,length,width);
  SaveImagePgm(NAME_IMG_OUT2,MatriceImgM2,length,width);

  /*Liberation memoire pour les matrices*/
  free_fmatrix_2d(MatriceImgR1);
  free_fmatrix_2d(MatriceImgI1);
  free_fmatrix_2d(MatriceImgM1);
  free_fmatrix_2d(MatriceImgR2);
  free_fmatrix_2d(MatriceImgI2);
  free_fmatrix_2d(MatriceImgM2);

  /*Commande systeme: visualisation de Ingout.pgm*/
  system("display image-TpIFT6150-1-Ad-d11r.pgm&");
  system("display image-TpIFT6150-1-Ad-d46r.pgm&");

  /*retour sans probleme*/
  printf("\n C'est fini ... \n\n\n");
  return 0;
}

void PreFFT_Translation(float** Matrix, int length, int width) {
    int x,y;
    for(x=0; x<length; x++)
        for(y=0; y<width; y++)
            if((x + y) % 2 == 1)
                Matrix[x][y] = -Matrix[x][y];
}
