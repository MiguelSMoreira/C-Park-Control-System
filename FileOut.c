/***********************************************************************
* File Name: FileOut.c
*	      
* Author:    Miguel Moreira (ist181048) e Pedro Coimbra (ist181293)
* Data:      30 de Abril de 2009   
* 
* NAME       PARQUE AUTOMATICO (Projecto de AED 2015)     
*
* DESCRIPTION Funcoes responsaveis pela abertura do ficheiro de saida e
*				pela escrita dos movimentos da viatura no seu percurso pelo
*				parque
*
************************************************************************/
#include "FileOut.h"


/***********************************************************************
* OpenFileOut()
*
* Arguments: char* fileNameIn nome do ficheiro de configuracoes
* Returns: ponteiro para o ficheiro de saida
* Description: Funcao responsavel pela correcta abertura do ficheiro
*				  de saida, de acordo com as indicacoes dadas no guia
*
**********************************************************************/
FILE* OpenFileOut( char* fileNameIn ){

	FILE* fp;
	int size;
	char *fileNameOut, extOut[] = ".pts", *aux;
	
	/* Procura a existencia de uma extensao nao especificada */
	aux = strrchr( fileNameIn, '.' );

	/* Calcula espaco necessario para o nome do ficheiro de saida, para
	os dois casos possiveis, um ficheiro de configuracoes com e sem
	extensao */
	if(aux != NULL ) size = strlen(fileNameIn) - strlen(aux);
	else size = strlen(fileNameIn);
	
	fileNameOut = (char *) malloc( sizeof(char) * (size + strlen(extOut)+ 1 ) );
	if(fileNameOut == NULL){
		fprintf(stderr, "Erro na alocacao de memoria para ficheiro de saida.\n");
    	exit(0);
	}
	
	/* Caso encontre uma extensao escreve a nova sobre essa, caso 
	contrario adiciona a extencao pretendida */
	strncpy( fileNameOut, fileNameIn , size);
	fileNameOut[size] = '\0';
	strcat( fileNameOut, extOut );
	fileNameOut[ size+strlen(extOut) ] = '\0';
	
	/* Abertura do ficheiro de saida */
	fp  = fopen(fileNameOut, "w");
	if(fp == NULL){
		fprintf(stderr, "Erro na abertura do ficheiro de saida.\n");
    	exit(0);
	}

	free(fileNameOut);
	return fp;
}


/************************************************************************
* escreve_saida ()
*
* Arguments: fp - descriptor do ficheiro de saida
*            vid - identificador da viatura
*            tk - instante de tempo em que ocorre o movimento
*            pX, pY, pZ - coordenadas (X,Y,Z) da viatura em movimento
*            tm - tipo de movimento
* Returns: 0 - se nao houver qualquer erro, ou seja se as coordenadas
*              corresponderem a uma posicao valida
*         -1 - se houver algum erro
*
* Description: escreve no ficheiro de saida um tuplo de valores do tipo
*              Vid T X Y Z M
*              sendo Vid o identificador da viatura, T o instante de tempo,
*              X, Y e Z  indicam a posicao da viatura no parque de
*              estacionamento e M e' o tipo de movimento podendo *    	
*  		      apenas ter
*              os caracteres 'i', 'm', 'e', 'p', 'a', 'x' ou 's'.
*
* DIAGNOSTICS a funcao nao fecha o ficheiro
*
* ***********************************************************************/
int escreve_saida(FILE *fp, char *vid, int tk, int pX, int pY, int pZ, char tm){

  static char pvid[MAX_ID_SIZE];
  static int ptk = -1, ppx, ppy, ppz;

  int retval = 0;    /* valor retornado quando não há erro;
                      * se houver erro retval = -1
                      */
  char *bogus = "??";

  if(fp == NULL) {
    fprintf(stderr, "Ficheiro invalido.\n");
    exit(0);
  }

  /* check for valid range of values and valid type of move */
  if (vid == NULL) {
    vid = bogus;
    fprintf(stderr, "Argumentos invalidos: Identificador de viatura nulo!\n");
    fprintf(stderr, "Chamada erronea:\t\t\t%s %d %d %d %d %c\n",
            vid, tk, pX, pY, pZ, tm);

    retval = -1;
  } else if (tk < 0) {
    fprintf(stderr, "Argumentos invalidos: tempo negativo!\n");
    fprintf(stderr, "Chamada erronea:\t\t\t%s %d %d %d %d %c\n",
            vid, tk, pX, pY, pZ, tm);
    retval = -1;
  } else if (pX <0 || pY <0 || pZ <0) {
    fprintf(stderr, "Argumentos invalidos: coordenadas erradas!\n");
    fprintf(stderr, "Chamada erronea:\t\t\t%s %d %d %d %d %c\n",
            vid, tk, pX, pY, pZ, tm);
    retval = -1;
  } else if (tm != 'i' && tm != 'x' && tm != 'e' && tm != 'm' && tm != 'p'
             && tm != 's' && tm != 'a') {
    fprintf(stderr, "Argumentos invalidos: tipo de movimento!\n");
    fprintf(stderr, "Chamada erronea:\t\t\t%s %d %d %d %d %c\n",
            vid, tk, pX, pY, pZ, tm);
    retval = -1;
  } else {
    if (ptk != -1) { /* 2nd execution */
      /* Note: this is abusive; this assumes that identifiers for different
       * are in different memory locations.
       * If this is not the case, comment out this test!
       */
      if (!strcmp(vid, pvid)) {
        /* same vehicle as in last call; exclude summary and exit lines */
        if ((tm == 'i') || (tm == 'm') || (tm == 'e') ||
            (tm == 'p') || (tm == 'a')) {
          if (tk <= ptk) {
            /* invalid tk */
            fprintf(stderr, "tk deve ser maior que %d.\n", ptk);
            fprintf(stderr, "Chamada erronea:\t\t\t%s %d %d %d %d %c\n",
                    vid, tk, pX, pY, pZ, tm);
            retval = -1;
          } else  if (pZ == ppz && pX == ppx && pY == ppy) {
            /* it did not move at all */
            fprintf(stderr, "Viatura %s não se moveu.\n", vid);
            fprintf(stderr, "Chamada erronea:\t\t\t%s %d %d %d %d %c\n",
                    vid, tk, pX, pY, pZ, tm);
            retval = -1;
          } else if (pZ == ppz && pX != ppx && pY != ppy) {
              /* diagonal move on same floor*/
              fprintf(stderr, "Movimento invalido: ");
              fprintf(stderr,
                      "linha e coluna nao podem mudar em simultaneo.\n");
              fprintf(stderr, "Chamada erronea:\t\t\t%s %d %d %d %d %c\n",
                      vid, tk, pX, pY, pZ, tm);
              retval = -1;
            } else if (pZ != ppz && (pX != ppx || pY != ppy)) {
              /* level changes with some lateral move*/
              fprintf(stderr, "Movimento invalido: ");
              fprintf(stderr,
                      "linha e coluna devem ser iguais ao nivel anterior.\n");
              fprintf(stderr, "Chamada erronea:\t\t\t%s %d %d %d %d %c\n",
                      vid, tk, pX, pY, pZ, tm);
              retval = -1;
            }
        }
      }
    }
  }
  if (retval == 0) {
    /* keep info from this run through */
    strncpy(pvid, vid, MAX_ID_SIZE);
    ptk = tk;
    ppx = pX; ppy = pY; ppz = pZ;

    /* generate output */
    fprintf(fp, "%s %d %d %d %d %c\n", vid, tk, pX, pY, pZ, tm);
  }

  return retval;
}
/*end of function */


/***********************************************************************
* WriteMov()
*
* Arguments:  FILE*fp         ponteiro para o ficheiro de saida
				  Parque* p       estrutura fundamental do parque
				  ListViat* viat  ponteiro para estrutura da viatura a tratar
				  ListAcc *ent    ponteiro para estrutura da entrada
				  ListAcc *acc    ponteiro para estrutura da saida
				  int extratime    tempo adicional de espera em caso de escrita
										 da viatura proveniente da lista de espera
				  int bitWaitList  bit permite identificar se vem da lista de espera
* Returns: void
* Description: Funcao principal responsavel pela escrita do ficheiro
*
**********************************************************************/
void WriteMov (FILE*fp, Parque* p, ListViat* viat, ListAcc *ent, ListAcc *acc, int extratime, int bitWaitList){

	int etime , atime, x, y, z, check;

	/* Se o bit WaitList estiver a 1 sabemos que o veiculo veio lista de espera, 
   pelo que nao temos de imprimir a linha de chegada, a terminada em 'i',
	visto ja o fazermos na funcao que adiciona veiculo a lista de espera */
	if( bitWaitList == 0){
		check = escreve_saida(fp, viat->id, viat->time, viat->x, viat->y, viat->z, 'i');
		if(check != 0){
			fprintf(stderr, "Erro na escrita do ficheiro de saida.\n");
			exit(0);
		}
	}
	
	/* Funcao recursiva que permite escrever as varias posicoes do carro desde
	que entra no parque ate chegar ao lugar (inclusive) */
	etime = WriteRecSt(fp, p, ent, viat, viat->spot, ent->index, -1, 1);
	
	/* Escreve que estacionou */
	x = viat->spot % (p->columns);
	y = (viat->spot / (p->columns)) % (p->rows);
	z = viat->spot / (p->columns * p->rows);
	
	check = escreve_saida(fp,viat->id,(etime + viat->time),x,y,z,'e');
	if(check != 0){
		fprintf(stderr, "Erro na escrita do ficheiro de saida.\n");
		exit(0);
	}

	/* Funcao responsavel pela escrita das varias posicoes do carro desde que
	sai do lugar a pe, ate que chega ao acesso (inclusive). Nao necessita de
	recursao ja que st indicara a posicao seguinte do percurso (ja que foi
	calculado para o percurso contrario (do acesso ao lugar) ) */
	atime = WriteSt(fp, p, acc, viat, viat->spot, acc->index, etime);
	
	/* Escreve que chegou ao acesso */
	x = acc->index % (p->columns);
	y = (acc->index / (p->columns)) % (p->rows);
	z = acc->index / (p->columns * p->rows);
	atime++;
	check = escreve_saida(fp,viat->id,(viat->time+etime+atime),x,y,z,'a');
	if(check != 0){
		fprintf(stderr, "Erro na escrita do ficheiro de saida.\n");
		exit(0);
	}
	
	/* Apos chegar ao acesso gerar linha de resumo com os pesos */
	/* Caso seja carro vindo da lista de espera devemos subtrair tempo de espera do tempo da viatura, ja que o seu tempo de chegada a entrada foi 
	actualizada para contabilizar o tempo de espera. Este e somado no peso final, ja que a espera conta para o custo do estacionamento */
	check = escreve_saida(fp, viat->id, viat->time - extratime, (etime + viat->time), (etime + atime + viat->time), (extratime+etime + 3 * atime), 'x');
	if(check != 0){
		fprintf(stderr, "Erro na escrita do ficheiro de saida.\n");
		exit(0);
	}

	return;
}


/***********************************************************************
* WriteRecSt()
*
* Arguments: FILE* fp         ponteiro para ficheiro de saida
*				 Parque* p			estrutura fundamental do parque
*				 ListAcc* ent		entrada utilizada pelo veiculo
*				 ListViat* viat	veiculo considerado
*				 int spot			lugar onde viatura vai estacionar
*				 int dest			condicao de paragem da recursao (indice da entrada)
*				 int time			argumento que permite alimentar o tempo resultado
										da iteracao anterior na seguinte, propagando-o 
*				 int aux          alimenta a iterada com a sua posicao atual,
										que sera a posicao seguinte
* Returns: tempo do movimento da entrada para o lugar de estacionamento
* Description: Funcao responsavel pela impressao dos movimentos do veiculo
*					desde a sua entrada no mapa ate ao estacionamento
*
**********************************************************************/
int WriteRecSt(FILE* fp, Parque* p, ListAcc* ent, ListViat* viat, int spot, int dest, int time, int aux){
	
	int x, y, z, check, type=0;
	/* Recursao da funcao de forma a percorrer corretamente o vetor de posicoes st[] desde o lugar de estacionamento
	ate a entrada para, posteriormente, imprimirmos o caminho pela ordem correcta */
	if( spot != dest ){
		aux = spot;
		spot = ent->st[spot];
		time = WriteRecSt(fp, p, ent, viat, spot, dest, time, aux);
	}
	
	/* Condisao if serve apenas para nao imprimir a posicao correspondente ao lugar de estacionamento, ja que
	inicializamos o tempo a zero e incrementamo-lo depois deste if */
	if (time > 0){
		x = spot % (p->columns);
		y = (spot / (p->columns)) % (p->rows);
		z = spot / (p->columns * p->rows);
		
		/* chama funcao que verifica se deveremos imprimir o movimento, ou seja, que verifica se houve uma mudanca 
		de piso ou de direcao. Caso seja de piso retorna 2 de forma a pudermos modificar correctamente o tempo */
		type = VerifyDir( p, aux, spot, ent->st[spot]);
		if ( type == 1 || type == 2 ){
			check = escreve_saida(fp,viat->id, time+viat->time,x,y,z,'m');
			if(check != 0){
				fprintf(stderr, "Erro na escrita do ficheiro de saida.\n");
				exit(0);			
			}
		}
	}
	
	/* Mudancas de tempo, somamos dois caso seja uma rampa, um em todos os outros casos */
	if( type == 2 ) time = time + 2;
	else time ++;

	return time;
}


/***********************************************************************
* WriteSt()
*  
* Arguments: FILE* fp       ponteiro para ficheiro de escrita
*				 Parque* p		 estrutura fundamental do parque
*				 ListAcc* acc	 acesso utilizado pelo veiculo
*				 ListViat* viat viatura considerada
*				 int spot		 indice do lugar onde a viatura vai 
*				 int dest       condicao de paragem da recursao (indice do acesso)
*				 int etime		 permite alimentar alimentar o tempo, resultado
*									 da iteracao anterior, na seguinte, propagando-o
* Returns: tempo do movimento, desde o lugar ate ao acesso
* Description: Funcao responsavel pela impressao dos movimentos do veiculo
*					desde o seu estacionamento ate ao acesso
*
**********************************************************************/
int WriteSt(FILE* fp, Parque* p, ListAcc* acc, ListViat* viat, int spot, int dest, int etime){

	int time = 0, counter = 0, check, x, y, z, type;
	int  aux, ant;

	aux = acc->st[spot];
	ant = spot;

	/* Ciclo permite-nos correr o vetor st[] desde o lugar de estacionamento ate ao acesso */
	while( aux != dest ){
		x = aux % (p->columns);
		y = (aux / (p->columns)) % (p->rows) ;
		z = aux / (p->columns * p->rows);
			
		/* chama funcao que verifica se deveremos imprimir o movimento, ou seja, que verifica se houve uma mudanca 
		de piso ou de direcao. Caso seja de piso retorna 2 de forma a pudermos modificar correctamente o tempo */	
		type = VerifyDir( p, ant, aux, acc->st[aux]);
		if( type == 2 ) time = time + 2;
		else time ++;

		/* Apenas imprimimos caso: haja uma modanca de direcao/piso ou, se quando chegarmos ao final do percurso pedestre, ainda nao 
		tivermos impresso nenhuma linha pedonal no ficheiro de saida. Desta forma mesmo que nao haja uma mudanca de piso ou direcao vai 	
		aparecer uma linha do tipo 'p' entre uma linha 'e' e uma 'a', tal como pedido pelos docentes */
		if( type == 2 || type == 1 || ( counter == 0 && acc->st[aux] == acc->index) ){
			check = escreve_saida(fp,viat->id, (etime + time + (viat->time)), x, y, z, 'p');
			if(check != 0){
				fprintf(stderr, "Erro na escrita do ficheiro de saida.\n");
				exit(0);
			}
			counter++;
		}

		ant = aux;
		aux = acc->st[aux];
	}

	return time;
}


/***********************************************************************
* VerifyDir()
*
* Arguments: Parque* p  estrutura fundamental do parque
				 int ant		indice da posicao anterior
				 int spot	indice da posicao atual
				 int seg		indice da posicao seguinte
* Returns: 0 caso nao haja mudanca de direcao ou piso
			  1 caso haja, e 2 caso haja mudanca de piso e tenhamos de 
			  incrementar o tempo de acordo com a rampa (somar 2 em vez de 1)
* Description: Funcao verifica se houve mudanca de direcao ou de piso.
				Serve como condicao para imprimir correctamente.
*
**********************************************************************/
int VerifyDir( Parque* p, int ant, int spot, int seg){
	
	int x, y, z, xa, ya, za, xs, ys, zs;

	x = spot % (p->columns);
	y = (spot / (p->columns)) % (p->rows);
	z = spot / (p->columns * p->rows);
	xa = ant % (p->columns);
	ya = (ant / (p->columns)) % (p->rows);
	za = ant / (p->columns * p->rows);
	xs = seg % (p->columns);
	ys = (seg / (p->columns)) % (p->rows);
	zs = seg / (p->columns * p->rows);

	/* Caso em que muda de piso */
	if( z == za + 1 || z + 1 == za ) return 2;
	if( z == zs + 1 || z + 1 == zs ) return 1;
	
	/* Caso em que muda de direcao vertical para direcao horizontal */
	if( (x == xa) && (xs != x) ) return 1;
	
	/* Caso em que muda de direcao horizontal para direcao vertical */	
	if( (y == ya) && (ys != y) ) return 1;

	return 0;
}


