/***********************************************************************
* File Name: Dijkstra.h
*	      
* Author:    Miguel Moreira (ist181048) e Pedro Coimbra (ist181293)
* Data:      21 de Novembro de 2015   
* 
* NAME       PARQUE AUTOMATICO (Projecto de AED 2015)     
*
* DESCRIPTION Ficheiro Header com os prototipos de funcoes que 
*				 permitem implementar o algoritmo Dijkstra e que sao
*				 responsaveis pelo movimento e verificacao de restricoes
*
* ************************************************************************/
#ifndef DijkstraHeader
#define DijkstraHeader 

#include "ListAdj.h"
#include "Heap.h"
#include "Structs.h"
#include "FileOut.h"
#include "strings.h"
#include "FileIn.h"

#define maxWT 1000000 /* Definicao do Peso Maximo usado na inicializacao
								do vector de pesos do algoritmo Dijkstra */


void Dijkstra(Parque*, Heap*, int, int*, int*);
void CalcDijkstraEntry( Parque*, Heap*, ListAcc*);
void CalcDijkstraAccesses( Parque*, Heap*, ListAcc*);
ListAcc* CalcEntrance(Parque *, Heap*, ListViat *);
ListAcc* CalcAccess( Parque*, Heap*, ListViat *, ListAcc *);
int CalcBestSpot(Parque*, ListAcc*, ListAcc*);
void MovViat(FILE*, Parque*, Heap*, ListViat*, int, int);
void RemViatID(Parque*, ListViat*, FILE*);
void RemViatUnknown (Parque*, ListViat*, FILE*);
void ReloadDijkstra(Parque*, Heap*, ListAcc*, ListAcc*);
ListAcc* ActDijkstraRest(Parque*, Heap*, ListViat*, ListAcc*, ListAcc*, int*, int*);
int checkAffectedPathEntry(Parque*, int, int, ListAcc*);
int checkAffectedPathAcc(Parque*, int, ListAcc*);

#endif
