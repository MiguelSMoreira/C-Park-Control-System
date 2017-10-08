/***********************************************************************
* File Name: Heap.h
*	      
* Author:    Miguel Moreira (ist181048) e Pedro Coimbra (ist181293)
* Data:      21 de Novembro de 2015   
* 
* NAME       PARQUE AUTOMATICO (Projecto de AED 2015)     
*
* DESCRIPTION 
* 
* ************************************************************************/
#include "Heap.h"


/***********************************************************************
* MoreNum()
*
* Arguments: int a, b  argumentos a considerar
* Returns: 0 ou 1 caso comparacao seja verdadeira ou nao
* Description: Funcao de comparacao de inteiros
*
**********************************************************************/
int MoreNum(int a, int b){
	return (a > b);
}


/***********************************************************************
* NewHeap()
*
* Arguments: Parque* p  estrutura fundamental do parque
* Returns:   ponteiro para a estrutura heap criada
* Description: Funcao responsavel por alocar e inicializar o acervo
*
**********************************************************************/
Heap *NewHeap( Parque *p ){
	
	Heap *h;
	int size;
	
	size = p->floors * p->columns * p->rows;
	h = (Heap *) malloc( sizeof(Heap) );
	if ( h == ( (Heap *) NULL ) ){
		fprintf(stderr, "Erro na alocacao de memoria para a Heap.\n");
		exit(0);
	}

	h->n_elements = 0;
	h->more = MoreNum;
	h->HeapData = (Data *) malloc( size * sizeof( Data ) );
	h->HeapDataAux = (int *) malloc( size * sizeof( int ) );
	if (h->HeapData == NULL || h->HeapDataAux == NULL) {
		fprintf(stderr, "Erro na alocacao de memoria para a HeapData.\n");
		exit(0);
	}

	return h;
}


/***********************************************************************
* FixDown()
*
* Arguments: Heap * h  estrutura do acervo
				 int tw    vertice
* Returns: void
* Description: Executa um fix down ao acervo a partir do vertice indicado.
				Faz as respectivas modificacoes ao vector auxiliar do acervo
				(que guarda correlacoes entre o indice do vertice e a sua posicao no vector do acervo).
*
**********************************************************************/
void FixDown(Heap * h, int tw){
	
	int j, t, p, aux;
	Data c;
  
	while ((2 * tw + 1) < h->n_elements) {  
		j = 2 * tw + 1;
		if (( (j + 1) < h->n_elements) && (h->more) (h->HeapData[j].weight, h->HeapData[j + 1].weight) )
			j++;
		if (!(h->more) (h->HeapData[tw].weight, h->HeapData[j].weight))
			break;

		t = (h->HeapData)[tw].index;
		p = (h->HeapData)[j].index;

		aux = (h->HeapDataAux)[t];
		(h->HeapDataAux)[t] = (h->HeapDataAux)[p];
		(h->HeapDataAux)[p] = aux;
		
		c = (h->HeapData)[tw];
		(h->HeapData)[tw] = (h->HeapData)[j];
		(h->HeapData)[j] = c;

		tw = j;
  }
  return;
}


/***********************************************************************
* FixUp()
*
* Arguments: Heap * h   estrutura do acervo
				 int tw		vertice a fazer
* Returns: void
* Description: Executa um fix up ao acervo a partir do vertice indicado.
				Faz as respectivas modificacoes ao vector auxiliar do acervo
				(que guarda correlacoes entre o indice do vertice e a sua posicao no vector do acervo).
*
**********************************************************************/
void FixUp(Heap * h, int tw){
	
	int t, p, aux;
	Data c;
 
	while ((tw > 0) && (h->more) ( h->HeapData[(tw - 1) / 2].weight, h->HeapData[tw].weight )){
		
		t = (h->HeapData)[tw].index;
		p = (h->HeapData)[(tw - 1) / 2].index;

		aux = (h->HeapDataAux)[t];
		(h->HeapDataAux)[t] = (h->HeapDataAux)[p];
		(h->HeapDataAux)[p] = aux;

		c = (h->HeapData)[tw];
		h->HeapData[tw] = h->HeapData[(tw - 1) / 2];
		h->HeapData[(tw - 1) / 2] = c;

		tw = (tw - 1) / 2;
	}

	return;
}


/***********************************************************************
* FreeHeap()
*
* Arguments: Heap *p  estrutura do acervo
* Returns: void
* Description: Funcao responsavel por dealocar correctamente a estrutura acervo
*
**********************************************************************/
void FreeHeap(Heap *h){
	
	free(h->HeapData);
	free(h->HeapDataAux);
	free (h);

  return;
}


/***********************************************************************
* Insert()
*
* Arguments: Heap * h     estrutura do acervo
				 int index    indice do vertice a adicionar
				 int weight	  peso do vertice a adicionar
* Returns: void
* Description: Funcao responsavel por inserir no no acervo
*
**********************************************************************/
void Insert(Heap * h, int index, int weight){
	
	h->HeapData[h->n_elements].weight = weight;
	h->HeapData[h->n_elements].index = index;
	h->HeapDataAux[h->n_elements] = index;
	h->n_elements++;

	return ;
}


/***********************************************************************
* RemoveMin()
*
* Arguments: Heap* h  estrutura do acervo
* Returns:   indice do ponto removido
* Description: Funcao remove o vertice com menor prioridade
*
**********************************************************************/
int RemoveMin(Heap * h){

	int ret, aux;
	if (h->n_elements > 0) {
		ret = (h->HeapData)[0].index;
		h->HeapDataAux[ret] = h->n_elements ;
		aux = (h->HeapData)[h->n_elements - 1].index;
		h->HeapDataAux[aux] = 0;
		
		(h->HeapData)[0] = (h->HeapData)[h->n_elements - 1];	
		h->n_elements--;
		FixDown(h, 0);
		return ret;
	}
	else{
		fprintf(stderr, "Erro a remover elemento da HeapInfo.\n");
		exit(0);
	}
}


/***********************************************************************
* DecPrior()
*
* Arguments: Heap * h   estrutura do acervo
				 int w		vertice a alterar
				 int weight novo peso para o vertice
* Returns: void
* Description: Funcao responsavel por modificar a prioridade do vertice 
					w para o indicado na variacel weight
*
**********************************************************************/
void DecPrior(Heap * h, int w, int weight){
  
	int tw;
	/* Verdadeiro valor do peso do vertice w */
	tw = h->HeapDataAux[w];
	h->HeapData[tw].weight = weight;
		
	FixUp(h, tw);

	return;
}


