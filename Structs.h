/***********************************************************************
* File Name: Structs.h
*	      
* Author:    Miguel Moreira (ist181048) e Pedro Coimbra (ist181293)
* Data:      21 de Novembro de 2015   
* 
* NAME       PARQUE AUTOMATICO (Projecto de AED 2015)     
*
* DESCRIPTION  Ficheiro Header com os tipos de dados e prototipos de funcoes
*				que manipulam as estruturas fundamentais do programa
* 
* ************************************************************************/
#ifndef StructsHeader
#define StructsHeader

#include "ListAdj.h"

struct InformacaoNo{     /* Estruta para informacao dos vertices do mapa */
	char type;				 /* Tipo do vertice (parede, estacionamento,etc) */
	int rest;				 /* Funciona com bit para indicar restricao activa no vertice */
	ListAdj* adj;			 /* Ponteiro para Lista de Adjacencias do vertice */
};

struct ParqueEstacionamento{	/* Estrutura Fundamental do Parque */	
	int columns;			 /* Numero de colunas do mapa */
	int rows;				 /* Numero de linhas do mapa */
	int floors;				 /* Numero de pisos do mapa */
	int vwaiting;			 /* Numero de veiculos na Lista de Espera */
	int* floorrest;		 /* Ponteiro para vetor utilizado para condensar info de restricoes de piso */
	int ltotal;				 /* Numero total de posicoes para estacionamento */
	int loccupied;			 /* Numero de posicoes de estacionamento ocupadas */
	ListViat* viats;      /* Ponteiro para lista com informacao de entradas e saidas das viaturas */
	ListWait* waiting;    /* Ponteiro para lista de espera de viaturas (para entrada) */
	ListRest* rests;		 /* Ponteiro para lista com informacao das restricoes */
	ListAcc* accesses;	 /* Ponteiro para lista com informacao necessaria sobre entradas e acessos
								 (incluindo ponteiros para respectivos vectores Dijkstra de posicoes e pesos) */
	No* info;             /* Info no, incluindo respectiva lista de adj */
};


Parque* InitParque (void);
void InitStructNos( Parque* );
void InitFloorRest( Parque* );
void FreeStructs( Parque* );
void createVecFloorRest(Parque*);
int ActRests( Parque*, ListViat*, int);

#endif
