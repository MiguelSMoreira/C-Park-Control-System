/***********************************************************************
* File Name: FileIn.h
*	      
* Author:    Miguel Moreira (ist181048) e Pedro Coimbra (ist181293)
* Data:      21 de Novembro de 2015   
* 
* NAME       PARQUE AUTOMATICO (Projecto de AED 2015)     
*
* DESCRIPTION Ficheiro Header com os prototipos de funcoes utilizadas
*				para a leitura dos ficheiros de configuracao do parque, 
* 				entrada de daos (viaturas) e de restricoes
*
* ************************************************************************/
#ifndef FileInHeader
#define FileInHeader

#include"ListAdj.h"
#include"Structs.h"

void CompleteListAdj( Parque*, int, int, int );
void ReadFileConfig( Parque*, char* );
void ReadFileInput( Parque*, char* );
void ReadFileRes( Parque*, char* );

#endif
