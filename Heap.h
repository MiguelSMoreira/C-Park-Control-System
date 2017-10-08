/***********************************************************************
* File Name: Heap.h
*	      
* Author:    Miguel Moreira (ist181048) e Pedro Coimbra (ist181293)
* Data:      21 de Novembro de 2015   
* 
* NAME       PARQUE AUTOMATICO (Projecto de AED 2015)     
*
* DESCRIPTION Ficheiro Header com os tipos de dados e prototipos de funcoes
*				que manipulam a Heap (estrutura auxiliar ao algoritmo Dijkstra)
*				utilizado pelo programa
*
* ************************************************************************/   
#ifndef HeapHeader
#define HeapHeader 

#include "Structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

typedef struct InfoAcervo{		/* Estrutura do vector principal da heap */
	int index;						/* Indice do vertice guardado nesta posicao */
	int weight;						/* Peso do vertice guardado na posicao */ 
}Data;								/* Definicao do tipo Data */

typedef struct Acervo{			/* Estrutura base do acervo */
	int n_elements;				/* Numero de elementos no acervo */
	int (*more) (int, int);		/* Funcao para comparacao de pesos */
	Data *HeapData;				/* Ponteiro para a heap */
	int *HeapDataAux;				/* Ponteiro para o vector auxiliar da heap */
}Heap;								/* Definicao do tipo Heap */

void FixDown(Heap *, int);
void FixUp(Heap *, int);
void DecPrior(Heap *, int, int);
int MoreNum(int, int);
Heap *NewHeap(Parque * );
void FreeHeap(Heap *);
void Insert(Heap *, int, int);
int RemoveMin(Heap *);

#endif
