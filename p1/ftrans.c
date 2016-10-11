#include "ftrans.h"
#include "estado.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
static inline Transicion* FtransUltimaTransicionLibre(Ftrans* delta) {
	if(delta->n==delta->len) return NULL;
	return &(delta->delta[delta->n]);
}

Ftrans* FtransNuevo() {
	Ftrans* f = (Ftrans*) calloc(1,sizeof(Ftrans));
	if(f == NULL) {
		return NULL;
	}

	f->len = INIT_LEN;
	f->n = 0;
	f->delta = calloc(sizeof(Transicion), INIT_LEN);
	if(f->delta == NULL) {
		FtransElimina(f);
		return NULL;
	}
	return f;
}

void FtransElimina(Ftrans* delta) {
	int i;
	if(delta != NULL) {
		if(delta->delta != NULL) {
			for(i=0; i<delta->n; i++) {
				LiberaLetra(delta->delta[i].letra);
			}
			free(delta->delta);
		}
		free(delta);
	}
}

Ftrans* FtransInserta(Ftrans* delta, Estado* origen, Estado* destino, Letra l) {
	Transicion* t;
	if (FtransUltimaTransicionLibre(delta) == NULL) {
		delta->delta = realloc(delta->delta, delta->len+INCR_LEN);
		delta->len += INCR_LEN;
		bzero(delta->delta+delta->n+1, INCR_LEN);
	}
	t = FtransUltimaTransicionLibre(delta);
	t->origen = origen;
	t->destino = destino;
	t->letra = CopiaLetra(l);
	delta->n++;
	return delta;
}


Estado** FtransTransita(Ftrans* delta, Estado* q_0, Letra l, int* len) {
	int i, n=0;
	Estado** conj = NULL;//(Estado**) calloc(1, sizeof(Estado*));
	for(i=0; i<delta->n;i++) {
		if(EstadoCompara(delta->delta[i].origen, q_0)) {
			/*if(!EstadoPerteneceAConjunto(delta->delta[i].destino, conj, n)) {
				if(ComparaLetra(l, delta->delta[i].letra)) {
					conj[n] = delta->delta[i].destino;
					n++;
					conj = realloc(conj, n+1);
					if(conj == NULL) return NULL;
				}
			}*/
			if(ComparaLetra(l, delta->delta[i].letra)) {
				conj = EstadoInsertaConjunto(delta->delta[i].destino, conj, &n);
			}

		}
	}

	*len = n;
	return conj;
}

char * FtransToString(Ftrans* delta, char* tabs, Estado** q, Alfabeto* sigma, int len) {
	int i, j, l;
	char* string1, *string2, *aux, *string=strdup("");
	Estado** res;


	for (i=0; i<len; i++) {
		for (j=0; j<sigma->num_simbolos; j++) {
			res = FtransTransita(delta, q[i], sigma->letras[j], &l);

			string1 =  EstadoToStringConjunto(res, l);
			if(string1 == NULL) {
				free(res);
				if(string) free(string);
				return NULL;
			}
			EstadoEliminaConjunto(res);	

			string2 = EstadoToString(q[i]);
			if(string2 == NULL) {
				free(string1);
				if(string) free(string);
				return NULL;
			}

			asprintf(&aux, "%s%sf(%s,%s)={%s }\n",string, tabs, string2, sigma->letras[j], string1);

			if(aux==NULL) {
				free(string1);
				free(string2);
				if(string) free(string);
				return NULL;
			}

			free(string);
			string = aux;
			free(string1);
			free(string2);

		}
	}
	return string;
}