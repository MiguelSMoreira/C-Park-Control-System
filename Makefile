#***********************************************************************
# File Name: makefile
#	      
# Author:    Miguel Moreira (ist181048) e Pedro Coimbra (ist181293)
# Data:      21 de Novembro de 2015   
# 
# NAME       PARQUE AUTOMATICO (Projecto de AED 2015)     
# 
#************************************************************************
# Compiler
CC=gcc

# Name
TARGET=autopark

# Libraries
LIBS= -lm

# Compiler Flags
CFLAGS= -Wall -ansi -O3

# Sources
SRCFILES= Gestor.c Dijkstra.c FileIn.c FileOut.c Heap.c ListAdj.c Structs.c 
INCFILES= Dijkstra.h FileIn.h FileOut.h Heap.h ListAdj.h Structs.h
OBJFILES= Gestor.o Dijkstra.o FileIn.o FileOut.o Heap.o ListAdj.o Structs.o

$(TARGET): $(OBJFILES)
		$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LIBS)

Gestor.o: Dijkstra.c FileIn.c FileOut.c Heap.c ListAdj.c Structs.c

Dijkstra.o: Dijkstra.c

FileIn.o: FileIn.c

FileOut.o: FileOut.c

Heap.o: Heap.c

ListAdj.o: ListAdj.c

Structs.o: Structs.c

clean::
	rm -f $(TARGET) $(OBJFILES) core.* #*

depend::
	makedepend $(SOURCES)

 
