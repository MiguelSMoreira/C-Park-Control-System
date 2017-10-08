/***********************************************************************
* File Name: filein.c
*	      
* Author:    Miguel Moreira (ist181048) e Pedro Coimbra (ist181293)
* Data:      21 de Novembro de 2015   
* 
* NAME       PARQUE AUTOMATICO (Projecto de AED 2015)     
*
* DESCRIPTION 
*
* DIAGNOSTICS 
* ************************************************************************/
/* Include Header File */
#include"FileIn.h"


/***********************************************************************
* CompleteListAdj ()
*
* Arguments: Parque* p    estrutura fundamental do parque
				 int x, y, z  coordenadas da posicao a considerar
* Returns: void
* Description: Funcao calcula as adjacencias para um ponto cujo 
				conjunto de coordenadas e dado no argumento, inserindo-as
				na lista de adjacencias do ponto considerado
*
**********************************************************************/
void CompleteListAdj(Parque* p, int x, int y, int z){
	
	int aux, posicao, cols, rows;
	char actual, adjac;
	cols = p->columns;
	rows = p->rows;
	posicao = x + (y * p->columns) + (z * p->columns * p->rows);
 
	actual = p->info[posicao].type;
	/* Faz adjacencia para o piso inferior (ie, uma rampa) */	
	if ( actual == 'd' ){
		aux = posicao - (cols * rows);
		p->info[posicao].adj = insertUnsortedListAdj(p->info[posicao].adj, aux );
	}
	
	/* Verifica se pode ter adjacencia na linha inferior */	
	aux = posicao - cols;
	adjac = p->info[aux].type;
	if( y > 0 && adjac != '@' && adjac != 'a' && adjac != 'e'){
		if( !( (actual == 'a'|| actual == 'e') && adjac == '.')  )
		p->info[posicao].adj = insertUnsortedListAdj(p->info[posicao].adj, aux );
	}

	/* Verifica se pode ter adjacencia na coluna esquerda */	
	aux = posicao - 1;
	adjac = p->info[aux].type;
	if( x > 0 && adjac != '@' && adjac != 'a' && adjac != 'e'){
		if( !( (actual == 'a'|| actual == 'e') && adjac == '.')  )
		p->info[posicao].adj = insertUnsortedListAdj(p->info[posicao].adj, aux );
	}

	/* Verifica se pode ter adjacencia na coluna direita */	
	aux = posicao + 1;
	adjac = p->info[aux].type;
	if( x < cols-1 && adjac != '@' && adjac != 'a' && adjac != 'e'){
		if( !( (actual == 'a'|| actual == 'e') && adjac == '.')  )
		p->info[posicao].adj = insertUnsortedListAdj(p->info[posicao].adj, aux );
	}

	aux = posicao + cols;
	adjac = p->info[aux].type;
	/* Verifica se pode ter adjacencia na linha superior */	
	if( y < rows-1 && adjac != '@' && adjac != 'a' && adjac != 'e'){
		if( !( (actual == 'a'|| actual == 'e') && adjac == '.')  )
		p->info[posicao].adj = insertUnsortedListAdj(p->info[posicao].adj, aux );
	}

	/* Faz adjacencia para o piso superior (ie, uma rampa) */	
	if ( actual == 'u' ){
		aux = posicao + (cols * rows);
		p->info[posicao].adj = insertUnsortedListAdj(p->info[posicao].adj, aux );
	}

   return;
}


/***********************************************************************
* ReadFileConfig ()
*
* Arguments: Parque* p      estrutura fundamental do parque
				 char* filein	 ponteiro para string com nome do ficheiro de configuracoes
* Returns: void
* Description: Funcao responsavel por ler ficheiro de Configuracoes e guardar
				as informacoes lidas nas estruturas de informacoes de pontos e na
				lista de acessos/entradas
*
**********************************************************************/
void ReadFileConfig(Parque* p, char* filein){
	
	FILE* fp;
	int  posicao;
	int i, j, k;
	int x, y, z;
	int size = 20;
	char *buf, *junk, aux, type, *ret;
	
	fp = fopen(filein, "r");
	if (fp == NULL){
		fprintf(stderr, "ERROR: Incorrect Configuration File Name.\n");
		exit(0);
	}

	k = fscanf(fp, "%d %d %d %d %d ", &(p->columns), &(p->rows),&(p->floors), &i, &j);

	if(size < p->columns+2) size = p->columns+2;

	junk = (char*) malloc( 6 * sizeof(char) );
	buf = (char*) malloc( size * sizeof(char) );
	if(buf==NULL || junk==NULL){
		fprintf(stderr, "Erro na alocacao de memoria para o Buffer do ReadFileConfig.\n");
		exit(0);
	}	

	/* Inicializa estrutura que guarda inforamacoes sobre os varios nos */
	InitStructNos(p);

	for (i = 0 ; i < p->floors ; i++){
		for (j = (p->rows - 1); j >= 0 ; j--){
			ret = fgets(buf, p->columns+2 , fp);
			for (k = 0 ; k < p->columns ; k++){
				sscanf(&buf[k], "%c", &aux);
				if (aux == '.' || aux == 'x') (p->ltotal)++;
				if (aux == 'x') (p->loccupied)++;
				posicao = k + (j * p->columns) + (i * p->columns * p->rows);
				p->info[posicao].type = aux;
			}
		}
	
		ret = fgets(buf, size, fp);	
		if(ret == NULL){
			fprintf(stderr, "Erro na alocacao de memoria para o Buffer do ReadFileConfig.\n");
		exit(0);
		}

		while( sscanf(buf,"%5s %d %d %d %c", junk, &x, &y, &z, &type) == 5){
			posicao =  x + (y * p->columns) + (z * p->columns * p->rows);
			if ( junk[0] == 'E' ){
				initaddListAccesses(p, posicao, type, 0);
			}
			else if ( junk[0] == 'A' ) {
				initaddListAccesses(p, posicao, type, 1);
			}
			ret = fgets(buf, size, fp);
		 }
	}
	
	for (i = 0 ; i < p->floors ; i++){
		for (j = (p->rows - 1); j >= 0 ; j--){
			for (k = 0 ; k < p->columns ; k++){
				posicao = k + (j * p->columns) + (i * p->columns * p->rows);
				if( p->info[posicao].type != '@' && p->info[posicao].type != '.' && p->info[posicao].type != 'x' )
					CompleteListAdj(p, k, j, i);
			}			
		}
	}
	/* Cria vector onde guardamos as restricoes dos pisos para facil acesso */
	createVecFloorRest(p);
			
	free(junk);	
	free(buf);
	fclose(fp);
	return;
}
 
 
/***********************************************************************
* ReadFileInput()
*
* Arguments: Parque* p      estrutura fundamental do parque
				 char* filein	 ponteiro para string com nome do ficheiro de input de viaturas
* Returns: void
* Description: Funcao responsavel por ler ficheiro de Input de viaturas e guardar
				as informacoes lidas na lista de viaturas
*
* **********************************************************************/ 
void ReadFileInput(Parque* p, char* filein){
	
	FILE* fp;
	int v, x, y, z, time;
	char *aux, *buf, type;
	
	fp = fopen(filein, "r");
	if (fp == NULL){
		fprintf(stderr, "ERROR: Incorrect Input File Name.\n");
		exit(0);
	}
	buf = (char*) malloc( 40 * sizeof(char) );
	aux = (char*) malloc( 6 * sizeof(char) );
	if( buf == NULL || aux == NULL ){
		fprintf(stderr, "Erro na alocacao de memoria para o Buffer do ReadFileInput.\n");
		exit(0);
	}

	while ( fgets(buf, 39, fp) != NULL ){		
		v = sscanf(buf, "%5s %d %c %d %d %d", aux, &time, &type, &x, &y, &z);
		if( v == 3 && type == 'S')
			initaddListViat(p, &aux, time, type, 0, 0, 0, v);
		else if( v == 6 )
			initaddListViat(p, &aux, time, type, x, y, z, v);	
	}

	free(buf);
	free(aux);
	fclose(fp);
	return;

}

	
/***********************************************************************
* ReadFileRes ()
*
* Arguments: Parque* p      estrutura fundamental do parque
				 char* filein	 ponteiro para string com nome do ficheiro de restricoes
* Returns: void
* Description: Funcao responsavel por ler ficheiro de restricoes e guardar
				as informacoes lidas na lista de restricoes
*
 **********************************************************************/
void ReadFileRes(Parque* p, char* filein){
	
	FILE* fp;
	char *buf, aux;
	int v, ti, tf, x, y, z;
	
	fp = fopen(filein, "r");
	if (fp == NULL){
		fprintf(stderr, "ERROR: Incorrect Restrictions File Name.\n");
		exit(0);
	}
	
	buf = (char*) malloc( 40 * sizeof(char) );
	if (buf == NULL){
		fprintf(stderr, "Erro na alocacao de memoria para o Buffer do ReadFileRes.\n");
		exit(0);
	}
	while ( fgets(buf, 39, fp) != NULL){
		v = sscanf(buf, "%c %d %d %d %d %d", &aux, &ti, &tf, &x, &y, &z );
		if( aux == 'R'){
			if( v == 4 )
				initaddListRest(p, ti, tf, x, y, z, 0);
			else if( v == 6 )
				initaddListRest(p, ti, tf, x, y, z, 1);
		}
	}

	free(buf);
	fclose(fp);
	return;
}
