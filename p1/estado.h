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

#endif
