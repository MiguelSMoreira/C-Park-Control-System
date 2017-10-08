/***********************************************************************
* File Name: Gestor.c
*	      
* Author:    Miguel Moreira (ist181048) e Pedro Coimbra (ist181293)
* Data:      21 de Novembro de 2015   
* 
* NAME       PARQUE AUTOMATICO (Projecto de AED 2015)     
*
* DESCRIPTION Ficheiro da funcao main do Parque Automatico. Corresponde
*				 ao modulo principal do programa sendo responsavel pela 
* 				 chamada de todas as funcoes que permitem o correto 
*			    funcionamento do programa
*
* ************************************************************************/
/* Inclusoes de ficheiro Header com tipos de data e prototipos de funcoes */
#include"ListAdj.h"
#include"FileIn.h"
#include"FileOut.h"
#include"Structs.h"
#include"Heap.h"
#include"Dijkstra.h"


/* Funcao Main */
int main(int argc, char *argv[]){

	Parque* p;
	Heap *h;
	ListViat *viat;
	ListWait *aux;
	FILE *fp;
	int dtime=0;

	/* Verificacao de correcta chamada do programa */
	if(argc < 3 || argc > 4){
		fprintf(stderr, "Usage: ./gestor [parque.cfg] [parque.inp] [parque.res]\n");
		exit(0);
 	}
   
	/* Chamada de InitParque responsavel por inicializar a estrutura fundamental 
	do parque. Funcoes seguintes sao responsaveis pela correcta leitura dos ficheiros de 
	configuracao, viaturas e restricoes do parque, e pela alocacao das 
	estruturas de dados respectivas */
	p = InitParque();
	ReadFileConfig(p, argv[1]);
   ReadFileInput(p, argv[2]);
	if (argc == 4) ReadFileRes(p, argv [3]);

	/* Inicializacao da heap usada como auxiliar ao funcionamento 
	do algoritmo Dijkstra. Seguidamente chamamos funcao OpenFileOut, responsavel pela 
	abertura do ficheiro de saida de acordo com as instrucoes dadas no enunciado*/
	h = NewHeap(p);
	fp = OpenFileOut( argv[1] );

	/* Ciclo responsavel pelo movimento/ operacao efectiva do parque */
	/* Trata-se de um ciclo que corre lista de Viats onde temos organizada 
	cronologicamente informacao sobre todas as entradas e saidas de viaturas */
	for(viat = p->viats; viat != NULL ; viat = viat->next){
		/* Caso em que temos de movimentar viatura */
		if( p->loccupied < p->ltotal && viat->type != 'S')
			MovViat(fp, p, h, viat, 0, 0);
					
		/* Caso em que parque esta cheio, adicionamos a viatura a 
		lista de espera para estacionar */
		else if( p->loccupied >= p->ltotal && viat->type != 'S')
			AddWaitingList(fp, p, viat);

		/* Caso em que temos de retirar um veiculo */ 
		else if (viat->type == 'S'){
			dtime = viat->time; /* Utilizado mais tarde para calcular tempo de espera da viatura */ 
			/* Caso em que gestor conhece o identificador do veiculo */
			if( viat->known == 1 ) RemViatID(p, viat, fp);

			/* Caso em que  o gestor tira o carro baseado na sua posicao */
			else if( viat->known == 0 ) RemViatUnknown(p, viat, fp);
		}
		
		/* Verificamos se ha possibilidade de entrada de um veiculo no parque vindo da lista 
		de espera. Na pratica so entramos neste caso se o movimento anterior for de saida*/
		if( p->vwaiting > 0 && p->loccupied < p->ltotal){
			/* Calculo do tempo que viatura teve de esperar na lista com base no tempo de
			saida da ultima viatura */
			dtime -= (p->waiting->viatwaiting->time);
			(p->waiting)->viatwaiting->time += dtime;
			/* Movimento da viatura. Ultimo bit indica a funcao um movimento vindo da lista
			de espera. Dtime e inserido para impressao correcta dos tempos */
			MovViat(fp, p, h, (p->waiting)->viatwaiting, dtime, 1);
			/* Actualizacao da Lista de Espera depois do movimento */
			if( p->vwaiting == 1){
				free(p->waiting);
				p->vwaiting --;
			}
			else{
				aux = p->waiting;
				p->waiting = p->waiting->next;
				free(aux);
				p->vwaiting --;
			}
		}
	}

	/* Fecho do ficheiro de saida e chamada das funcoes responsaveis por 
	dealocar correctamente todas as estruturas utilizadas no funcionamento 
	do programa */
	fclose(fp);
	FreeHeap(h);
	FreeStructs(p);
 
	exit(0);  
}


