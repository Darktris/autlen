#ifndef _ESTADO_H_
#define _ESTADO_H_

struct _Estado;
typedef struct _Estado Estado;

enum {INICIAL, NORMAL, FINAL, INICIAL_Y_FINAL};

Estado* EstadoNuevo(char* nombre, int tipo);
char* EstadoToString(Estado* q, int mostrar_tipo);
void EstadoElimina(Estado* q);
int EstadoInicial(Estado* q);
int EstadoFinal(Estado* q);
int EstadoCompara(Estado* j, Estado* i);

int EstadoPerteneceAConjunto(Estado* q, Estado** array, int len);
Estado * EstadoObtieneConjunto(char* nombre, Estado** array, int len);
void EstadoEliminaConjunto(Estado** array);
Estado** EstadoInsertaConjunto(Estado* q, Estado** array, int* len);
char* EstadoToStringConjunto(Estado** array, int size, int mostrar_tipo);
#endif
