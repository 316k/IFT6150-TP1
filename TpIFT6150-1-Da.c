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

int main(int argc,char **argv)
 {
  const float pi = acos(-1.0);
  int i, j;
  int length = 128, width = 128;
  float** cosinus = fmatrix_allocate_2d(length,width);
  
  for(i=0; i<length; i++)
      for(j=0; j<width; j++) {
          cosinus[i][j] = 128 + 127.0 * cos(2 * pi * (25 * j + 31 * i) / 128.0);
      }
  
  SaveImagePgm("image-TpIFT6150-1-Da", cosinus, length, width);

  free_fmatrix_2d(cosinus);
  
  return 0;
}

