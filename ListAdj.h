/***********************************************************************
* File Name: ListAdj.h
*	      
* Author:    Miguel Moreira (ist181048) e Pedro Coimbra (ist181293)
* Data:      21 de Novembro de 2015   
* 
* NAME       PARQUE AUTOMATICO (Projecto de AED 2015)     
*
* DESCRIPTION Ficheiro Header com os tipos de dados e prototipos de funcoes
*				que manipulam todas as listas usadas pelo programa
*
* ************************************************************************/   
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "Structs.h"
#include "FileOut.h"
                   
#ifndef ListAdjHeader
#define ListAdjHeader  

typedef struct InformacaoNo No;					/* Definicao do tipo No */

typedef struct ParqueEstacionamento Parque;  /* Definicao do tipo Parque */

typedef struct ListaRestricoes ListRest;		/* Definicao do tipo ListRest */
struct ListaRestricoes{								/* Estrutura que guarda informacao relativa a cada uma 
															das restricoes lidas */
	int x, y, z, p;									/* Inteiro p usado caso seja restricao de piso, coordenadas
															utilizadas caso seja uma restricao de posicao */
	int t_inicial, t_final;							/* Intervalo de tempo em que restricao esta activa */
	char type;											/* Tipo de Restricao ('P' e 'L' para piso ou lugar resp.) */
	ListRest* next;									/* Ponteiro para proximo elemento na lista de restricoes */
};

typedef struct ListaViaturasStruct ListViat; /* Definicao do tipo ListViat */
struct ListaViaturasStruct{						/* Estrutura que guarda informacao relativa a cada entrada
															ou saida do parque de estacionamento */
	char* id;											/* Identificador da Viatura */
	int time;											/* Tempo em que se deu a entrada ou saida */
	char type;											/* Tipo ('Z' caso seja saida, ou caracter que indica o tipo
															de acesso pretendido, em caso de ser entrada */
	int x, y, z;										/* Coordenadas da entrada ou da posicao de onde retirar viat */
	int known;											/* Bit que indica se e saida por coordenadas ou por identificador */
	int spot;											/* Lugar onde viatura vai estacionar, calculada pelo gestor */
	ListViat* next;									/* Ponteiro para proximo elemento na lista de viaturas */
};

typedef struct ListaAdjacenciasStruct ListAdj;/* Definicao do tipo ListAdj */
struct ListaAdjacenciasStruct{					 /* Estrutura que guarda a informacao das adjacencias de um vertice */
	int index;											 /* Indice do ponto adjacente */
	ListAdj* next;										 /* Ponteiro para proximo elemento na lista de adjacencias */
};

typedef struct ListaAccessesStruct ListAcc;   /* Definicao do tipo ListAcc */
struct ListaAccessesStruct{						 /* Estrutura que guarda a informacao dos acessos e das entradas */
	char type;											 /* Caso */
	int index;											 /* Indice do vertice considerado */
	int* wt;												 /* Ponteiro para vector Dijkstra dos pesos */
	int* st;												 /* Ponteiro para vector Dijkstra das posicoes */
	ListAcc* next;										 /* Ponteiro para proximo elemento na lista de acessos */
};

typedef struct ListaWaitingStruct ListWait;   /* Definicao do tipo ListWait */
struct ListaWaitingStruct{							 /* Estrutura que guarda os ponteiros para as estruturas dos 
															 veiculos na lista de espera */
	ListViat* viatwaiting;							 /* Ponteiro para estrutura da viatura na lista de espera */
	ListWait* next;									 /* Ponteiro para proximo elemento na lista de espera */
};

void freeListViat(ListViat *);
void freeListRest(ListRest *);
void freeInfo(Parque* );
void freeListAcc(ListAcc *);
void freeListWaiting(ListWait *);
void initaddListRest(Parque*, int, int, int, int, int, int);
ListViat* initaddListViat(Parque*, char**, int, char, int, int, int, int);
ListAdj* insertUnsortedListAdj(ListAdj*, int);
void initaddListAccesses(Parque*, int, char, int);
void AddWaitingList(FILE*, Parque*, ListViat*);

#endif
