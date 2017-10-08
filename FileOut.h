/***********************************************************************
* File Name: escreve_saida.c
*	      
* Author:    Miguel Moreira (ist181048) e Pedro Coimbra (ist181293)
* Data:      30 de Abril de 2009   
* 
* NAME       PARQUE AUTOMATICO (Projecto de AED 2015)     
*
* DESCRIPTION Ficheiro Header com os prototipos de funcoes utilizadas
*				para a escrita do ficheiro de saida do programa
*
************************************************************************/
#ifndef FileOutHeader
#define FileOutHeader

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ListAdj.h"

#define MAX_ID_SIZE 128

FILE* OpenFileOut(char*);
int escreve_saida(FILE*, char*, int, int, int, int, char);
int VerifyDir( Parque*, int, int, int);
void WriteMov (FILE*, Parque*, ListViat*, ListAcc*, ListAcc*, int, int);
int WriteRecSt(FILE*, Parque*, ListAcc*, ListViat*, int, int, int, int);
int WriteSt(FILE*, Parque*, ListAcc*, ListViat*, int, int, int);

#endif
