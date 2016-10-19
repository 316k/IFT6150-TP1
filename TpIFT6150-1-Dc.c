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
/* PROGRAMME PRINCIPAL   -------------------------*/
/*------------------------------------------------*/

int main(int argc,char **argv)
 {
  const float pi = acos(-1.0);
  int i, j;
  int length = 32, width = 32;
  float** cosinus = fmatrix_allocate_2d(length, width);
  
  for(i=0; i<length*4; i += 4)
      for(j=0; j<width*4; j += 4) {
          cosinus[i / 4][j / 4] = 128 + 127.0 * cos(2 * pi * (25 * j + 32 * i) / 128.0);
      }

  SaveImagePgm("image-TpIFT6150-1-Dc", cosinus, length, width);

  free_fmatrix_2d(cosinus);
  
  return 0;
}

