/***********************************************************************
* File Name: Dijkstra.c
*	      
* Author:    Miguel Moreira (ist181048) e Pedro Coimbra (ist181293)
* Data:      21 de Novembro de 2015   
* 
* NAME       PARQUE AUTOMATICO (Projecto de AED 2015)     
*
* DESCRIPTION Conjunto de funcoes responsaveis pelo calculo do melhor 
				 caminho, melhor acesso a escolher, melhor lugar para estacionar e
				 responsaveis pelo movimento e pela verificacao de restricoes
* 
* ************************************************************************/
#include "Dijkstra.h"


/***********************************************************************
* Dijkstra()
*
* Arguments: Parque * p  estrutura fundamental do parque
				 Heap* h     ponteiro para estrutura da Heap 
				 int s	       vertice raiz (origem do caminho)	
				 int st[], wt[] vectores dos pesos e posicoes a preencher
* Returns: void
* Description: Algoritmo Dijkstra utiizado para calcular a SPT com origem em s 
*
**********************************************************************/
void Dijkstra(Parque * p, Heap* h , int s, int st[], int wt[]){

	int v, w, pmax, weight;
	char type;
	ListAdj* t; 
	
	pmax = p->floors * p->columns * p->rows;
	for (v=0; v< pmax; v++){
		st[v] = -1;
		wt[v] = maxWT;
		Insert(h, v, maxWT);
	}
 	
	wt[s] = 0;
	DecPrior(h, s, 0);
	type = p->info[s].type;
	while ( h->n_elements > 0 ){
		/* O vertice V vai ser o proximo a ser adicionado*/
		if ( wt[ v = RemoveMin(h) ] != maxWT ){
			if(st[v] != -1) type = p->info[ st[v] ].type;
			for (t = p->info[v].adj; t != NULL; t = t->next){
				if( p->info[ v ].rest == 1 ) break;
				if( p->info[ t->index ].rest == 1 ) continue;
				if( (p->info[v].type=='u' || p->info[v].type=='d') && p->info[t->index].type!='u' && p->info[t->index].type!='d' && type != 'u' && type != 'd') continue;

				if( ( p->info[v].type == 'u' && type == 'd' ) || ( p->info[v].type == 'd' && type == 'u' ) ) weight = 2;
				else weight = 1;
				if ( wt[w = t->index] > wt[v] + weight ){
					/* Se o caminho conhecido ate ao w for superior ao 
					caminho minimo que estavamos a criar ate ao ponto v, 
					mais o peso de v a w, ou seja, t, entao adicionamos w 
					ao nosso caminho minimo */
					wt[w] = wt[v] + weight;
					DecPrior(h, w, wt[w]);
					st[w] = v;
				}
			}
		}
 	}
	return;
}


/***********************************************************************
* CalcDijkstraEntry()
*
* Arguments: Parque* p    estrutura fundamental do parque
				 Heap* h      ponteiro para a estrutura do acervo
				 ListAcc* ent ponteiro para estrutura da entrada
* Returns: void
* Description: Funcao responsavel por alocar memoria e calcular os vectores
				 de posicoes e pesos Dijkstra para uma determinada entrada
*
**********************************************************************/
void CalcDijkstraEntry( Parque* p, Heap* h, ListAcc* ent){
	
	int max;

	max = p->columns * p->rows * p->floors;
	
	ent->st = (int*) malloc( max * sizeof(int) );
	ent->wt = (int*) malloc( max * sizeof(int) );
	if(ent->st == NULL || ent->wt == NULL){
		fprintf(stderr, "Erro na alocacao de memoria para o vectores auxiliares ao Dijkstra.\n");
		exit(0);
	}
	
	Dijkstra(p, h, ent->index, ent->st, ent->wt);

	return;
}


/***********************************************************************
* CalcDijkstraAccesses()
*
* Arguments: Parque* p    estrutura fundamental do parque
				 Heap* h		  ponteiro para a estrutura do acervo
				 ListAcc* acc  ponteiro para a estrutura do acesso
* Returns: void
* Description: Funcao responsavel por alocar memoria e calcular os vectores
				 de posicoes e pesos Dijkstra para todos os acessos do mesmo tipo
				 do acesso recebido
*
**********************************************************************/
void CalcDijkstraAccesses( Parque* p, Heap* h, ListAcc* acc ){

	ListAcc* aux;
	int max;
	
	max = p->columns * p->rows * p->floors;
	for( aux = p->accesses; aux != NULL; aux = aux->next){
		if( aux->type == acc->type ){
			aux->st = (int*) malloc( max * sizeof(int) );
			aux->wt = (int*) malloc( max * sizeof(int) );
			if(aux->st == NULL || aux->wt == NULL){
				fprintf(stderr, "Erro na alocacao de memoria para o vectores auxiliares ao Dijkstra.\n");
				exit(0);
			}

			Dijkstra(p, h, aux->index, aux->st, aux->wt);
		}
	}
	
	return;
}

/***********************************************************************
* CalcBestSpot()
*
* Arguments: Parque* p     estrutura fundamental do parque
				 ListAcc* ent	ponteiro para a estrutura da entrada
				 ListAcc* acc  ponteiro para a estrutura do acesso
* Returns: indice do melhor lugar para estacionar
* Description: Funcao responsavel por calcular o melhor lugar disponivel
				 para estacionar a viatura, de acordo com a entrada e o
				 acesso calculado por nos como o melhor
*
**********************************************************************/
int CalcBestSpot(Parque* p, ListAcc* ent, ListAcc* acc){
	
	int min, max, spot=0, dist, i;
	int z;
	/* Ent ponta em que entrada a viatura esta e Acc aponta para o 
	acesso do tipo pretendido mais proximo */
	
	max = p->floors * p->rows * p->columns;
	min = 4 * max;

	/* Calculo do melhor lugar */
	for( i=0; i< max; i++){
		z = i / (p->columns * p->rows);
		if( p->info[i].type == '.' && p->info[i].rest == 0 && p->floorrest[z] == 0){
			dist = (ent->wt[i]) + 3 * (acc->wt[i]);
			if (dist < min){
				min = dist;
				spot = i;
			}
		}
	}
	return spot;
}


/***********************************************************************
* CalcEntrance()
*
* Arguments: Parque *p     estrutura fundamental do parque
				 Heap* h       estrutura do acervo
				 ListViat *viat  ponteiro para a viatura a considerar
* Returns: ponteiro para a entrada encontrada
* Description: Funcao responsavel por encontrar o ponteiro para a entrada
					em que a viatura se apresenta
*
**********************************************************************/
ListAcc* CalcEntrance(Parque *p, Heap* h, ListViat *viat){

	int pos;
	ListAcc *ent;

	pos = viat->x + (viat->y * p->columns) + (viat->z *p->columns *p->rows);
	ent = p->accesses;
	while( ent != NULL ){
		if(ent->type == 'Z' && ent->index == pos) break;
		ent = ent->next;
	}
	
	/* Caso os vectores de posicao e peso Dijkstra nao estiverem preenchidos
	para esta entrada, a funcao e chamada para os preencher */
	if( ent->st == NULL || ent->wt == NULL ) CalcDijkstraEntry(p, h, ent);

	return ent;
}


/***********************************************************************
* CalcAccess()
*
* Arguments: Parque* p      estrutura fundamental do parque
				 Heap*h	       estrutura do acervo
				 ListViat *viat	ponteiro para a viatura a considerar
				 ListAcc *ent		ponteiro para a entrada a considerar
* Returns: ponteiro para o melhor acesso encontrado
* Description: Funcao responsavel por calcular para uma determinada
				  viatura o melhor acesso a escolher com base na entrada em 
				  que se encontra
*
**********************************************************************/
ListAcc* CalcAccess( Parque* p, Heap*h, ListViat *viat, ListAcc *ent){

	int min, spot, fspot=0, weight;
	ListAcc *acc=NULL, *aux; 
	
	min = 4 * p->floors * p->rows * p->columns;
	for( aux = p->accesses; aux != NULL; aux = aux->next){
		if( aux->type == viat->type ){
			/* Caso os vetores para este tipo de acessos nao ter sido calculado
			e chamada a funcao seguinte que os calcula para todos os acessos do
			mesmo tipo. Somos forcados a calculalos para todos os acessos do 
			mesmo tipo de forma a garantirmos sempre a escolha do melhor acesso */
			if( aux->st == NULL || aux->wt == NULL) CalcDijkstraAccesses(p, h, aux);
			/* A funcao da o melhor lugar de estacionamento para a entrada 
			dada e este determinado acesso. */
			spot = CalcBestSpot( p, ent, aux);
			weight = (3 * aux->wt[spot]) + ent->wt[spot];
			if (weight < min){
				min = weight;
				fspot = spot;
				acc = aux;
			}
		}
	}
	viat->spot = fspot;
	return acc;
}


/***********************************************************************
* MovViat()
*
* Arguments: FILE* fp
				 Parque *p	
				 Heap *h
				 ListViat *viat
				 int extratime
				 int bitWaitList
* Returns: void 
* Description: Funcao responsavel pelo movimento da viatura. Verifica
				restricoes, calcula melhor entrada e melhor acesso, altera
				vectores Dijkstra caso restricao calhe na estrada e chama funcao para 
				escrever movimento, recalculando os vectores Dijkstra caso tenham sido
				alterados por uma restricao
*
**********************************************************************/
void MovViat(FILE* fp, Parque *p, Heap *h, ListViat *viat, int extratime, int bitWaitList){
	
	ListAcc *ent, *acc;
	int e=0, a=0, makerest;

	/* Actualiza informacao de restricoes para esta movimento */
	/* Verifica se alguma restricao afecta caminho. Em caso afirmativo mete makerest a 1 */
	makerest = ActRests(p, viat, 1);

	/* Calcula melhor acesso e posicao para os vectores Dijsktra existentes */
	ent = CalcEntrance(p, h, viat);
	acc = CalcAccess(p, h, viat, ent);

	/* Caso exista uma restricao que calhe numa estrada (makerest a 1) chamamos funcao 
	para verificar se alguma restricao afecta o caminho e recalcular
	melhor acesso e lugar em caso afirmativo. Mudamos e e a por referencia para saber se 
	temos de retornar os vectores Dijkstra aos seus valores antes das restricoes */
	if( makerest > 0 )
		acc = ActDijkstraRest(p, h, viat, ent, acc, &e, &a);
	
	WriteMov(fp, p, viat, ent, acc, extratime, bitWaitList);
	
	/* Poe a viatura no lugar final onde vai ficar estacionada */
	p->info[ viat->spot ].type = 'x';
	(p->loccupied)++;

	/* Retorna as restricoes aos seus valores iniciais */
	ActRests(p, viat, 0);
	/* Caso tenha havido alteracao aos vectores Dijkstra iniciais recalculamo-los para
	condicoes sem restricoes */
	if( makerest != 0){
		if ( e == 0 && a == 0) return;
		if ( e != 0 || a != 0) ReloadDijkstra(p, h, ent, acc);
	}

	return;
}


/***********************************************************************
* RemViatID()
*
* Arguments: Parque* p      estrutura fundamental do parque
				 ListViat* viat ponteiro para estrutura com info da saida
				 FILE* fp       ponteiro para ficheiro de saida
* Returns: void 
* Description: Funcao responsavel por remover viatura com base no seu
				identificador
*
**********************************************************************/
void RemViatID( Parque* p, ListViat* viat, FILE* fp){

	ListViat *aux;
	int check, x, y, z;

	/* Correr do inicio a lista de carros ate encontrar um com o mesmo ID. 
	Depois tirar as coordenadas deste */
	for(aux = p->viats; aux != NULL; aux = aux->next)
		if( !strcasecmp(aux->id, viat->id) ) break;
	
	if( p->info[ aux->spot ].type == 'x'){
		p->info[ aux->spot ].type = '.';
		(p->loccupied)--;

		x = aux->spot % (p->columns);
		y = (aux->spot / (p->columns)) % (p->rows);
		z = aux->spot / (p->columns * p->rows);

		check = escreve_saida(fp, viat->id, viat->time, x, y, z, 's');
		if(check != 0){
			fprintf(stderr, "Erro na escrita do ficheiro de saida.\n");
			exit(0);
		}
	}

	return;
}


/***********************************************************************
* RemViatUnknown()
*
* Arguments: Parque* p      estrutura fundamental do parque
				 ListViat* viat ponteiro para estrutura com info da saida
				 FILE* fp       ponteiro para ficheiro de saida
* Returns: void
* Description: Funcao responsavel por remover viatura com base na sua
				posicao (caso em que gestor nao conhece viatura)
*
**********************************************************************/
void RemViatUnknown (Parque *p, ListViat *viat, FILE *fp){
	
	int pos; 
	int check;	

	pos = viat->x + (viat->y * p->columns) + (viat->z * p->columns * p->rows);
	if( p->info[pos].type == 'x'){
		p->info[pos].type = '.';
		(p->loccupied)--;

		check = escreve_saida(fp, viat->id, viat->time, viat->x, viat->y, viat->z, 's');
		if(check != 0){
			fprintf(stderr, "Erro na escrita do ficheiro de saida.\n");
			exit(0);
		}
	}

	return;
}


/***********************************************************************
* ActDijstraRest
*
* Arguments: Parque* p     estrutra fundamental do parque
				 Heap* h			estrutura do acervo
				 ListViat* viat  viatura a considerar
				 ListAcc* ent    entrada do veiculo
				 ListAcc* acc	  melhor acesso antes de considerar restricoes	
				 int* e, * a	  valores a alterar por referencia de modo a saber
									  se o caminho e afectado fora da funcao
* Returns: ponteiro para novo melhor acesso
* Description: Funcao verifica se uma restricao afecta o caminho "normal"
				da viatura chamando funcoes auxiliares. Caso afecte recalcula
				os vectores Dijkstra chamando funcao Reload e calcula o novo
				melhor acesso e lugar, chamando CalcAccess
*
**********************************************************************/
ListAcc* ActDijkstraRest(Parque* p, Heap* h, ListViat* viat, ListAcc* ent, ListAcc* acc, int* e, int* a){

	ListAcc* aux;
	int spot;

	spot = viat->spot;	
	*e = checkAffectedPathEntry(p, spot, ent->index, ent);
	*a = checkAffectedPathAcc(p, spot, acc);	
	
	/* Se o retorno for 0 sabemos que as restricoes nao afectam o 
	caminho */
	if (*e == 0 && *a == 0) return acc;
	if (*e != 0 || *a != 0) ReloadDijkstra(p, h, ent, acc);

	/* Agora que temos os vectores Dijkstra correctos ja calculamos bem 
	o melhor accesso e sabemos que o sitio de estacionamento ja foi 
	correctamente actualizado para a nova entrada */
	aux = CalcAccess( p, h, viat, ent );
	return aux;
}


/***********************************************************************
* ReloadDijkstra
*
* Arguments: Parque* p     estrutura fundamental do parque
				 Heap* h			estrutura do acervo
				 ListAcc* ent	ponteiro para estrutura da entrada
				 ListAcc* acc ponteiro para estrutura do acesso
* Returns: void
* Description: Funcao recalcula os vectores Dijkstra para a entrada
				considerada e para todos os acessos do mesmo tipo que o
				considerado
*
**********************************************************************/
void ReloadDijkstra(Parque* p, Heap* h, ListAcc* ent, ListAcc* acc){
	
	ListAcc* aux=NULL;
	/* Se lhe for enviado um acesso que e uma entrada temos so de 
	recalcular Dijkstra para a entrada */ 
	Dijkstra(p, h, ent->index, ent->st, ent->wt);

	/* Se o acesso enviado nao for uma entrada, temos de correr o 
	Dijkstra para todos os acessos */
	for(aux=p->accesses; aux!=NULL; aux=aux->next){
		if( aux->type == acc->type )
			Dijkstra(p, h, aux->index, aux->st, aux->wt);
	}

	return;
}


/***********************************************************************
* checkAffectedPathEntry()
*
* Arguments: 	Parque* p   estrutura fundamental do parque
				   int pos     lugar de estacionamento da viatura
				   int dest    indice da entrada
				   ListAcc* ent  ponteiro para estrutura da entrada
* Returns: 0 caso a restricao nao afecte o caminho, 1 caso afecte
* Description: Funcao que verifica se o caminho desde a entrada ate
				 ao lugar foi afectado pela activacao de uma restricao
*
**********************************************************************/
int checkAffectedPathEntry(Parque* p, int pos, int dest, ListAcc* ent){

	int check=0;
	if( ent->st[pos] != dest ){
		pos = ent->st[pos];
		check = checkAffectedPathEntry(p, pos, dest, ent);
	}
	
	if( p->info[pos].rest == 0 && check == 0) return 0;
	else return 1;

}


/***********************************************************************
* checkAffectedPathAcc
*
* Arguments: Parque* p     estrutura fundamental do parque
				 int posicao	lugar de estacionamento da viatura
				 ListAcc* acc  ponteiro para estrutura do acesso
* Returns: 0 caso a restricao nao afecte o caminho, 1 caso afecte
* Description: Funcao que verifica se o caminho desde o lugar de 
				estacionamento ate ao acesso foi afectado pela activacao de uma restricao
*
**********************************************************************/
int checkAffectedPathAcc(Parque* p, int posicao, ListAcc* acc){

	while( posicao != acc->index ){
		if( p->info[posicao].rest == 1 ) return 1;                       
		posicao = acc->st[posicao];
	}
	
	return 0;
}


