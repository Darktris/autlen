#ifndef _FTRANS_H
#define _FTRANS_H
#include "alfabeto.h"
#include "estado.h"

typedef struct {
	Estado* origen;
	Estado* destino;
	Letra letra;
} Transicion;


typedef struct {
	unsigned int n;
	unsigned int len;
	Transicion* delta;
} Ftrans;

#define INIT_LEN 1000
#define INCR_LEN 500
Ftrans* FtransNuevo();
Ftrans* FtransInserta(Ftrans* delta, Estado* origen, Estado* destino, Letra l);
void FtransElimina(Ftrans* delta);
char* FtransToString(Ftrans* delta, int tab, Estado** q, Alfabeto* sigma, int len);
Estado** FtransTransita(Ftrans* delta, Estado* q_0, Letra l, int* len); 
#endif
