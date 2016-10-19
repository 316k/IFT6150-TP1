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

int wrapped_coord(int i, int len);

int main(int argc,char **argv)
 {
  int i, j, x, y;
  int length, width;
  float** MatriceImgBriquesR;
  float** MatriceImgBriquesI;
  
  float** MatriceImgConvolutionR;
  float** MatriceImgConvolutionI;

  
  MatriceImgBriquesR     = LoadImagePgm("D1r", &length, &width);
  MatriceImgBriquesI     = fmatrix_allocate_2d(length, width);
  MatriceImgConvolutionR = fmatrix_allocate_2d(length, width);
  MatriceImgConvolutionI = fmatrix_allocate_2d(length, width);
  

  int taille;
  printf("Entrez la taille du carré utilisé pour la convolution : ");
  scanf("%d", &taille);
  
  /*Initialisation a zero de toutes les matrices */
  for(i=0;i<length;i++)
      for(j=0;j<width;j++) {
          
          if((i < taille/2.0 || i >= length - taille/2.0)  &&
             (j < taille/2.0 || j >= width - taille/2.0)) {
              MatriceImgConvolutionR[i][j] = 255.0;
          } else {
              MatriceImgConvolutionR[i][j] = 0.0;
          }

          MatriceImgConvolutionI[i][j] = 0.0;
          MatriceImgBriquesI[i][j]=0.0;
      }

  SaveImagePgm("image-TpIFT6150-1-Cb_square", MatriceImgConvolutionR, length, width);
  Mult(MatriceImgConvolutionR, 1/255.0, length, width);
  
  FFTDD(MatriceImgConvolutionR, MatriceImgConvolutionI, length, width);
  FFTDD(MatriceImgBriquesR, MatriceImgBriquesI, length, width);

  float r1, r2, i1, i2;
  for(i=0; i<length; i++)
      for(j=0; j<width; j++) {
          r1 = MatriceImgConvolutionR[i][j];
          i1 = MatriceImgConvolutionI[i][j];
          r2 = MatriceImgBriquesR[i][j];
          i2 = MatriceImgBriquesI[i][j];
          
          MatriceImgConvolutionR[i][j] = (r1 * r2 - i1 * i2);
          MatriceImgConvolutionI[i][j] = (r1 * i2 + r2 * i1);
      }

  IFFTDD(MatriceImgConvolutionR, MatriceImgConvolutionI, length, width);
  IFFTDD(MatriceImgBriquesR, MatriceImgBriquesI, length, width);

  Recal(MatriceImgConvolutionR, length, width);
  SaveImagePgm("image-TpIFT6150-1-Cb_filtered", MatriceImgConvolutionR, length, width);

  /*Liberation memoire pour les matrices*/
  free_fmatrix_2d(MatriceImgConvolutionR);
  free_fmatrix_2d(MatriceImgConvolutionI);
  free_fmatrix_2d(MatriceImgBriquesR);
  free_fmatrix_2d(MatriceImgBriquesI);
  
  return 0;
}
