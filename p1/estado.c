#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "estado.h"

Estado* EstadoNuevo(char* nombre, int tipo) {
	Estado *q = calloc(1, sizeof(Estado));
	if(q == NULL) {
		return NULL;
	}

	q->nombre = strdup(nombre);
	if(q->nombre == NULL) {
		free(q);
		return NULL;
	}
	q->tipo = tipo;
	return q;
}

char* EstadoToString(Estado* q) {
	char* string = calloc(1, strlen(q->nombre)+4);
	if(string == NULL) return NULL;
	if(q->tipo == INICIAL) {
		sprintf(string, "->%s", q->nombre);
	} else if (q->tipo == FINAL) {
		sprintf(string, "%s*", q->nombre);
	} else sprintf(string, "%s", q->nombre);

	return string;
}
