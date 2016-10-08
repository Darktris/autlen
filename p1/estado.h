#ifndef _ESTADO_H_
#define _ESTADO_H_

typedef struct {
	char* nombre;
	int tipo;
} Estado;

enum {INICIAL, NORMAL, FINAL};

Estado* EstadoNuevo(char* nombre, int tipo);
char* EstadoToString(Estado* q);
void EstadoElimina(Estado* q);
int EstadoInicial(Estado* q);
int EstadoFinal(Estado* q);
int EstadoCompara(Estado* j, Estado* i);

int EstadoPerteneceAConjunto(Estado* q, Estado** array, int len);
void EstadoEliminaConjunto(Estado** array);
Estado** EstadoInsertaConjunto(Estado* q, Estado** array, int* len);
char* EstadoToStringConjunto(Estado** array, int len);
#endif
