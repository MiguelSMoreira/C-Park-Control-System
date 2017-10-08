/***********************************************************************
* File Name: Structs.c
*	     
* Author:    Miguel Moreira (ist181048) e Pedro Coimbra (ist181293)
* Data:      21 de Novembro de 2015   
* 
* NAME       PARQUE AUTOMATICO (Projecto de AED)     
*
* DESCRIPTION 
*
* ************************************************************************/
/* Include Header File */
#include "Structs.h"


/***********************************************************************
* InirStructBis ()
*
* Arguments: Parque *p  estrutura fundamental do parque
* Returns: void
* Description: Funcao responsavel por alocar espaco para a estrutura dos nos e
				por a inicializar		
*
**********************************************************************/
void InitStructNos( Parque* p ){
	
	int i;
	p->info=(No*) malloc( (p->columns*p->rows*p->floors ) * sizeof(No) );
	if (p->info ==  NULL){
		fprintf(stderr, "Erro na alocacao de memoria para o ponteiro para a Lista de Adjacencias.\n");
		exit(0);
	}

	for(i=0; i< p->columns * p->rows * p->floors; i++){
		p->info[i].rest = 0;
		p->info[i].adj = NULL;
	}

	return;
}


/***********************************************************************
* InitParque
*
* Arguments: void
* Returns: ponteiro para a estrutura criada
* Description: Funcao responsavel por alocar espaco para a estrutura do Parque e
				por a inicializar	
*
**********************************************************************/
Parque* InitParque (void){
	
	Parque* p;
	p = (Parque*) malloc( sizeof(Parque) );
	if( p == NULL){
		fprintf(stderr, "Erro na alocacao de memoria para a estrutura principal do Parque.\n");
		exit(0);
	}
   
	p->ltotal = 0;
	p->loccupied = 0;
	p->vwaiting = 0;      /* Contador de viaturas a espera */
	p->viats = NULL;   
	p->floorrest = NULL;  
	p->info = NULL; 
	p->rests = NULL;
	p->accesses = NULL;
	p->waiting = NULL;   /* Apontador para lista de espera de viaturas */

	return p;
}


/***********************************************************************
* createVecFloorRest(p)
*
* Arguments: Parque* p  estrutura fundamental do parque  
* Returns: void
* Description: Funcao responsavel por alocar memoria e por inicializar
					o vector responsavel por guardar restricoes de pisos
*
**********************************************************************/
void createVecFloorRest(Parque* p){
	
	int i;
	p->floorrest = (int*) malloc( p->floors * sizeof(int) );
	if( p == NULL){
		fprintf(stderr, "Erro na alocacao de memoria para o vector de restricoes dos pisos.\n");
		exit(0);
	}

	for(i=0; i<p->floors; i++)
		p->floorrest[i] = 0;

	return;
}


/***********************************************************************
* FreeStructs(p)
*
* Arguments: Parque* p  estrutura fundamental do parque
* Returns: void
* Description: Dealoca correctamente todas as estruturas e listas utilizadas
*
**********************************************************************/
void FreeStructs( Parque* p){

	freeListAcc(p->accesses);
	freeListRest(p->rests);
	freeListViat(p->viats);
	freeInfo(p);
	free(p->floorrest);
	free(p);

	return;
}


/***********************************************************************
* ActRests()
*
* Arguments: Parque* p
				 ListViat* viat
				 int ActBit
* Returns: valor que permite identificar se existe alguma restricao a
			  calhar em cima de uma estrada
* Description: Funcao responsavel por actualizar estruturas com a 
				 informacao das restricoes a cada entrada de nova viatura
*
**********************************************************************/
int ActRests( Parque* p, ListViat* viat, int ActBit){

	ListRest* aux;
	int pos;
	int counter = 0;	

	/* Corre todas as restricoes postas */
	for(aux=p->rests; aux!=NULL; aux=aux->next){
		/* Se o tempo de entrada estiver no intervalo da restricao */
		if( viat->time >= aux->t_inicial  && (viat->time < aux->t_final || aux->t_final == 0) ){
			/* Se a restricao for de piso, e actualizado o VectRestFloor */
			if(aux->type == 'P')
				p->floorrest[ aux->p ] = ActBit;
			else if(aux->type == 'L'){
				pos =  aux->x + (aux->y * p->columns) + (aux->z * p->columns * p->rows);
				p->info[ pos ].rest = ActBit;
				if(p->info[pos].type == ' ' || p->info[pos].type == 'u' || p->info[pos].type == 'd') counter ++;
			}
		}
	}

	return counter;
}


