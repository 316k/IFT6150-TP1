/*------------------------------------------------------*/
/* Prog    : TpIFT6150-1-Ac.c                           */
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

#define NAME_IMG_OUT "image-TpIFT6150-1-Ac"

/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/
/*------------------------------------------------*/

void PreFFT_Translation(float** Matrix, int length, int width);

int main(int argc,char **argv)
 {
  int i,j,k;
  int length,width;
  float** MatriceImgR;
  float** MatriceImgI;
  float** MatriceImgM;

  /*Allocation memoire pour la matrice image*/
  MatriceImgR=LoadImagePgm(NAME_IMG_IN,&length,&width);
  MatriceImgI=fmatrix_allocate_2d(length,width);
  MatriceImgM=fmatrix_allocate_2d(length,width);

  /*Initialisation a zero de toutes les matrices */
  for(i=0;i<length;i++)
      for(j=0;j<width;j++) {
          MatriceImgI[i][j]=0.0;
          MatriceImgM[i][j]=0.0;
      }

  PreFFT_Translation(MatriceImgR, length, width);
  
  /*FFT*/
  FFTDD(MatriceImgR,MatriceImgI,length,width);

  /*Module*/
  Mod(MatriceImgM,MatriceImgR,MatriceImgI,length,width);

  for(i=0; i<length; i++)
      for(j=0; j<width; j++)
          MatriceImgM[i][j] = logf(1 + MatriceImgM[i][j]);

  Recal(MatriceImgM,length,width);

  
  /*Sauvegarde de MatriceImgM sous forme d'image pgm*/
  SaveImagePgm(NAME_IMG_OUT,MatriceImgM,length,width);

  /*Liberation memoire pour les matrices*/
  free_fmatrix_2d(MatriceImgR);
  free_fmatrix_2d(MatriceImgI);
  free_fmatrix_2d(MatriceImgM);

  /*Commande systeme: visualisation de Ingout.pgm*/
  system("display image-TpIFT6150-1-Ac.pgm&");

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
