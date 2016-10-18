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
  int i, j, k, l, x, y;
  int length, width;
  float** MatriceImgBriques;
  float** MatriceImgConvolution;

  int convolution_size = 15;
  
  MatriceImgBriques = LoadImagePgm("D1r", &length, &width);
  MatriceImgConvolution = fmatrix_allocate_2d(length, width);
  
  for(i=0; i<length; i++)
      for(j=0; j<width; j++) {
          // 
          MatriceImgConvolution[i][j] = 0;

          for(k = -convolution_size/2; k < convolution_size/2; k++)
              for(l = -convolution_size/2; l < convolution_size/2; l++) {
                  x = wrapped_coord(i + k, length);
                  y = wrapped_coord(j + l, width);
                  
                  MatriceImgConvolution[i][j] += MatriceImgBriques[x][y];
              }
      }

  Mult(MatriceImgConvolution, 1/(convolution_size * convolution_size + 0.0), length, width);
  SaveImagePgm("image-TpIFT6150-1-Cb_filtered", MatriceImgConvolution, length, width);

  /*Liberation memoire pour les matrices*/
  free_fmatrix_2d(MatriceImgConvolution);
  free_fmatrix_2d(MatriceImgBriques);
  
  return 0;
}

int wrapped_coord(int i, int len) {
    /* // zero */
    /* if(i < 0 || i >= len) */
    /*     return 0; */

    /* return i; */
    
    // wrap
    return (i + len) % len;
}
