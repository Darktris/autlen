#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "estado.h"

Estado* EstadoNuevo(char* nombre, int tipo) {
	if(nombre == NULL) return NULL;
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
	if(q == NULL) return NULL;
	char* string = calloc(1, strlen(q->nombre)+4);
	if(string == NULL) return NULL;
	if(q->tipo == INICIAL) {
		sprintf(string, "->%s", q->nombre);
	} else if (q->tipo == FINAL) {
		sprintf(string, "%s*", q->nombre);
	} else sprintf(string, "%s", q->nombre);

	return string;
}

void EstadoElimina(Estado* q) {
	if(q != NULL) {
		if(q->nombre != NULL) {
			free(q->nombre);
		}
		free(q);
	}
}


int EstadoInicial(Estado* q) {
	if(q == NULL) return 0;
	return q->tipo == INICIAL;
}
int EstadoFinal(Estado* q) {
	if(q == NULL) return 0;
	return q->tipo == FINAL;
}

int EstadoCompara(Estado *i, Estado* j) {
	if(i == NULL || j == NULL) return 0;
	return !(strcmp(i->nombre, j->nombre)==0);
}

int EstadoPerteneceAConjunto(Estado* q, Estado** array, int len) {
	int i;
	if(q == NULL || array == NULL) return 0;
	for(i=0; i<len; i++) {
		if(EstadoCompara(q, array[i]))
			return 1;
	}
	return 0;
}

void EstadoEliminaConjunto(Estado** array) {
	if(array != NULL) 
		free(array);
}

Estado** EstadoInsertaConjunto(Estado* q, Estado** array, int* len) {
	if(q == NULL || len == NULL) return array;
	Estado** aux;
	if(array == NULL) {
		array = malloc(sizeof(Estado*));
		if(array == NULL) {
			return NULL;
		}
		*len = 1;
		array[0] = q;
		return array;
	}
	if(EstadoPerteneceAConjunto(q, array, *len)) return array;
	aux = realloc(array, (*len)++);
	if(aux == NULL) {
		*len = *len-1;
		return array;
	}
	array[*len-1] = q;
	return array;
}