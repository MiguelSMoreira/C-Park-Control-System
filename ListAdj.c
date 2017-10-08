/***********************************************************************
* File Name: ListAdj.c
*	      
* Author:    Miguel Moreira (ist181048) e Pedro Coimbra (ist181293)
* Data:      21 de Novembro de 2015   
* 
* NAME       PARQUE AUTOMATICO (Projecto de AED 2015)     
*
* DESCRIPTION Conjunto de Funcoes responsaveis pela criacao, manipulacao e
*				 dealocacao das listas utilizadas ao longo do programa
*
* ************************************************************************/
/* Include Header File */
#include"ListAdj.h"


/***********************************************************************
* freeListRest()
*
* Arguments: ListRest *first 
* Returns: void
* Description: Funcao responsavel pela dealocacao da lista de restricoes
*
**********************************************************************/
void freeListRest(ListRest *first){

	ListRest * next, * aux;
  
	for(aux = first; aux != NULL; aux = next) {
		next = aux->next;
		free(aux);
  }

  return;
}


/***********************************************************************
* freeListViat()
*
* Arguments: ListViat *first
* Returns: void
* Description: Funcao responsavel pela dealocacao da lista de viaturas
*
**********************************************************************/
void freeListViat(ListViat *first){

	ListViat *next, *aux;
  
	for(aux = first; aux != NULL; aux = next) {
		next = aux->next;                                        
		free(aux->id);
		free(aux);
  }

  return;
}


/***********************************************************************
* freeInfo()
*
* Arguments: Parque* p
* Returns: void
* Description: Funcao responsavel pela dealocacao do vetor com a 
*             informacao dos vertices
*
**********************************************************************/
void freeInfo(Parque* p){

	int i;
	ListAdj *next, *aux;
	
	for(i = 0; i < p->columns * p->rows * p->floors ; i++){
		for(aux = p->info[i].adj; aux != NULL; aux = next) {
			next = aux->next;
			free(aux);
  		}
	}
	free(p->info);

	return;
}


/***********************************************************************
* freeListAcc()
*
* Arguments: ListAcc *first
* Returns: void
* Description: Funcao responsavel pela dealocacao da lista de restricoes
*
**********************************************************************/
void freeListAcc(ListAcc *first){

	ListAcc * next, * aux;
  
	for(aux = first; aux != NULL; aux = next) {
		next = aux->next;
		if (aux->st != NULL) free(aux->st);
		if (aux->wt != NULL) free(aux->wt);
		free(aux);
  }

  return;
}


/***********************************************************************
* init&fillListViat()
*
* Arguments: Parque* p	 estrutura fundamental do parque
*				 char** id	 identificador da viatura
*				 int time	 instante da ocurrencia
*				 char type	 'S' para saida ou o acesso pretendido pela viatura
*				 int x, y, z coordenadas de entrada da viatura ou do veiculo a remover
*				 int v       numero de argumentos lidos do ficheiro (permite
*								 indicar se e saida por coordenadas ou identificador)
* Returns: Ponteiro para a viatura adicionada
* Description: Funcao responsavel por adicionar viaturas a lista, 
*					preenchendo a estrutura desta com as informacoes recebidas 
*
**********************************************************************/
ListViat* initaddListViat(Parque* p, char** id, int time, char type, int x, int y, int z, int v){
	
	ListViat *new, *aux;
	int size;
	
	new = (ListViat*) malloc( sizeof( ListViat ) );
	if( new == NULL ){
		fprintf(stderr, "Erro na alocacao de memoria para uma nova estrutura da Lista de Viaturas.\n");
		exit(0);
	}

	size = strlen( *id);
	new->id = (char*) malloc ( (size+1) * sizeof(char) );
	if (new->id == NULL){
		fprintf(stderr, "Erro na alocacao de memoria para o ID da Lista de Viaturas.\n");
		exit(0);
	}	

	if(v == 6 && type == 'S') new->known = 0;
	else new->known = 1;

	strcpy( new->id, *id);
	new->x = x;
 	new->y = y;
 	new->z = z;
	new->spot = 0;
	new->time = time;
	new->type = type;
	new->next = NULL;
	
	if( p->viats == NULL ){
		p->viats = new;
		return new;
	}
	else{
		aux = p->viats;	
		while( aux->next != NULL)
			aux=aux->next;
		aux->next = new;
	}

	return new;
}


/***********************************************************************
* initaddListResp()
*
* Arguments: Parque* p    estrutura fundamental do parque
				 int ti, tf   intervalo em que restricao esta ativa
				 int x, y, z  coordenada de afectacao da restricao
								(caso seja piso, info aparece em x)
				 int bit  distingue restricoes de piso das de lugar
* Returns: void
* Description:  Funcao cria lista de restricoes se esta nao existir ou
*					 adiciona uma restricao caso esta ja tenha sido criada
*
**********************************************************************/
void initaddListRest(Parque* p, int ti, int tf, int x, int y, int z, int bit){
		
	ListRest *new, *aux;

	new = (ListRest*) malloc ( sizeof(ListRest) );
	if( new == NULL){
		fprintf(stderr, "Erro na alocacao de memoria para uma nova estrutura da Lista de Restricoes.\n");
		exit(0);
	}

	if( bit == 0 ){
		new->p = x;
		new->t_inicial = ti;
		new->t_final = tf;
		new->type = 'P';
	}
	else if ( bit == 1 ){
		new->x = x;
		new->y = y;
		new->z = z;
		new->t_inicial = ti;
		new->t_final = tf;
		new->type = 'L';
	}
	new->next = NULL;

	if ( p->rests == NULL ){
		p->rests = new;
		return;
	}
	else{
		aux = p->rests;
		while( aux->next != NULL)
			aux = aux->next;
		aux->next = new;
		return;
	}
}


/***********************************************************************
* insertUnsortedListAdj()
*
* Arguments: ListAdj* list  ponteiro para inicio da lista de adjacencias
				 int index
* Returns: ponteiro para o inicio da lista
* Description:  Funcao cria lista de Adjacencias se esta nao existir e 
					adiciona vertices a lista caso ja exista
*
**********************************************************************/
ListAdj* insertUnsortedListAdj(ListAdj* list, int index){

	ListAdj * new;
	ListAdj * aux;
	
	new = (ListAdj *) malloc( sizeof(ListAdj) );
	if(new == NULL){
		fprintf(stderr, "Erro na alocacao de memoria para uma nova estrutura da Lista de Adjacencias.\n");
		exit(0);
	}
		
	new->next=NULL;
	new->index=index;	
	
	if(list == NULL)	return new;

	aux = list;
	while(aux->next != NULL)
		aux=aux->next;

	aux->next = new;

	return list;
}


/***********************************************************************
* initaddListAccesses()
*
* Arguments: Parque* p  estrutura fundamental do parque
				 int index  indice da entrada e ou acesso
				 char type  'Z' caso seja entrada, ou tipo de acesso caso contrario
				 int bit    permite identificar se se trata de uma entrada ou acesso
* Returns: void
* Description:  Funcao cria lista de acessos caso esta nao exista e 
*					adiciona novos acessos a lista caso ja exista
*
**********************************************************************/
void initaddListAccesses(Parque* p, int index, char type, int bit){

	ListAcc *new, *aux;

	new = (ListAcc*) malloc ( sizeof(ListAcc) );
	if( new == NULL){
		fprintf(stderr, "Erro na alocacao de memoria para uma nova estrutura da Lista de Restricoes.\n");
		exit(0);
	}

	new->index = index;
	new->next = NULL;
	new->wt = NULL;
	new->st = NULL;
	
	if (bit == 1) new->type = type;
	else new->type = 'Z';
	
	if ( p->accesses == NULL ){
		p->accesses = new;
		return;
	}
	else{
		aux = p->accesses;
		while( aux->next != NULL)
			aux = aux->next;
		aux->next = new;
		return;
	}
}


/***********************************************************************
* AddWaitingList()
*
* Arguments: FILE *fp        apontador para o ficheiro de saida
				 Parque *p		  estrutura fundamental do parque
				 ListViat *viat  apontador para estrutura da viatura considerada
* Returns: void
* Description:  Funcao responsavel por adicionar viatura a lista de espera
*
**********************************************************************/
void AddWaitingList(FILE *fp, Parque *p, ListViat *viat){
	
	ListWait *aux, *new;
	int check;

	new = (ListWait*) malloc ( sizeof(ListWait) );
	if ( new == NULL ){	
		fprintf(stderr, "Erro na alocacao de memoria para uma nova estrutura de viaturas.\n");
		exit(0);
	}
	
	new->viatwaiting = viat;
	new->next = NULL;

	/* Funcao escreve linha de entrada, deta forma identificamos chegada da viatura
	mesmo que esta seja redirecionada para a lista de espera*/
	check = escreve_saida(fp, viat->id, viat->time, viat->x, viat->y, viat->z, 'i');
	if(check != 0){
		fprintf(stderr, "Erro na escrita do ficheiro de saida.\n");
		exit(0);
	}

	if( p->waiting == NULL ){
		p->waiting = new; 
		p->vwaiting = 1;
		return;
	}

	/* Adiciona a viatura considerada no final da lista de forma a manter ordem de 
	chegada e incrementa vwaiting para actualizarmos numero de viaturas na lista de espera*/
	if( p->waiting != NULL ){
		for(aux = p->waiting; aux->next != NULL; aux = aux->next);
		aux->next = new;
		p->vwaiting ++;
	}

	return;
}


