#ifndef _FTRANS_H
#define _FTRANS_H
#include "alfabeto.h"
#include "estado.h"

#define INIT_LEN 1
#define INCR_LEN 1

struct _Transicion;
struct _Ftrans;
typedef struct _Transicion Transicion;
typedef struct _Ftrans Ftrans;

Ftrans* FtransNuevo();
Ftrans* FtransInserta(Ftrans* delta, Estado* origen, Estado* destino, Letra l);
Ftrans* FtransUne(Ftrans* delta1, Ftrans*delta2);
void FtransElimina(Ftrans* delta);
char* FtransToString(Ftrans* delta, char* extra, Estado** q, Alfabeto* sigma, int len);
Estado** FtransTransita(Ftrans* delta, Estado* q_0, Letra l, int* len); 
#endif
