#ifndef _FTRANS_H
#define _FTRANS_H
#include "alfabeto.h"
typedef struct {
	char* origen;
	char* destino;
	Letra* l;
} Transicion;

typedef Transicion* Ftrans;

Ftrans* FtransNuevo();
Ftrans* FtransInserta(Ftrans* delta, char* origen, char* destino, Letra l);
void FtransElimina(Ftrans* delta);
char* FtransToString(Ftrans* delta, int tab);
#endif
