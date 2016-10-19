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

void Phase(float**,float**,float**,int,int);
void PreFFT_Translation(float** Matrix, int length, int width);

int main(int argc, char **argv)
 {
  int i,j,k;
  int length=128,width=128;
  float** MatriceImgR;
  float** MatriceImgR_ref;
  float** MatriceImgI;
  float** MatriceImgM;
  float** MatriceImgPhase;

  /*Allocation memoire pour la matrice image*/
  MatriceImgR = fmatrix_allocate_2d(length,width);
  MatriceImgR_ref = fmatrix_allocate_2d(length,width);
  MatriceImgI = fmatrix_allocate_2d(length,width);
  MatriceImgM = fmatrix_allocate_2d(length,width);
  MatriceImgPhase = fmatrix_allocate_2d(length,width);


  int taille = 40;
  int decalage;
  for(k=0; k<2; k++) {

      if(k == 0) {
          decalage = 0;
      } else {
          printf("Entrez la translation en y du carré : ");
          scanf("%d", &decalage);
      }
  
      /*Initialisation a zero de toutes les matrices */
      for(i=0;i<length;i++)
          for(j=0;j<width;j++) {
          
              if(i > decalage && i < decalage + taille && j > width/2.0 - taille/2.0 && j < width/2.0 + taille/2.0) {
                  MatriceImgR[i][j] = 1.0;
                  MatriceImgR_ref[i][j] = 255.0;
              } else {
                  MatriceImgR[i][j] = 0.0;
                  MatriceImgR_ref[i][j] = 0.0;
              }

              MatriceImgI[i][j]=0.0;
              MatriceImgM[i][j]=0.0;
              MatriceImgPhase[i][j] = 0.0;
          }
      
      SaveImagePgm("image-TpIFT6150-1-Af-img",MatriceImgR_ref,length,width);
      
      PreFFT_Translation(MatriceImgR, length, width);
      
      /*FFT*/
      FFTDD(MatriceImgR,MatriceImgI,length,width);
      
      /*Module*/
      Mod(MatriceImgM,MatriceImgR,MatriceImgI,length,width);
      Phase(MatriceImgPhase,MatriceImgR,MatriceImgI,length,width);

      printf("Translation en y = %d\n", decalage);
      printf("Valeur du module au point (9, 9) : %lf\n", MatriceImgM[9][9]);
      printf("Valeur de la phase au point (9, 9) : %lf\n\n", MatriceImgPhase[9][9]);
  }
  
  for(i=0; i<length; i++)
      for(j=0; j<width; j++)
          MatriceImgM[i][j] = logf(1 + MatriceImgM[i][j]);

  Recal(MatriceImgM,length,width);
  
  /*Sauvegarde de MatriceImgM sous forme d'image pgm*/
  SaveImagePgm("image-TpIFT6150-1-Af-spc",MatriceImgM,length,width);

  /*Liberation memoire pour les matrices*/
  free_fmatrix_2d(MatriceImgR);
  free_fmatrix_2d(MatriceImgI);
  free_fmatrix_2d(MatriceImgM);

  /*Commande systeme: visualisation de Ingout.pgm*/
  // system("display image-TpIFT6150-1-Ac.pgm&");
  return 0;
}

void Phase(float** Matrix, float** Real, float** Img, int length, int width) {
    int x,y;
    for(x=0; x<length; x++) {
        for(y=0; y<width; y++) {
            Matrix[x][y] = atan2f(Img[x][y], Real[x][y]);
        }
    }
}

void PreFFT_Translation(float** Matrix, int length, int width) {
    int x,y;
    for(x=0; x<length; x++)
        for(y=0; y<width; y++)
            if((x + y) % 2 == 1) {
                Matrix[x][y] = -Matrix[x][y];
            }
}
