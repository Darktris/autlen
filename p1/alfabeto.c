#include <stdlib.h>
#include <string.h>

#include "alfabeto.h"

Alfabeto* AlfabetoNuevo(int num_simbolos) {
	Alfabeto* sigma = calloc(1, sizeof(Alfabeto));
	sigma->letras = calloc(num_simbolos, sizeof(Letra));
	return sigma;
}

Alfabeto* AlfabetoInserta(Alfabeto* sigma, Letra l) {
	Letra* it;
	it = sigma->letras;
	while((it)!=NULL) it++;
	*it = CopiaLetra(l);
	return sigma;
}

void AlfabetoElimina(Alfabeto* sigma) {
	int i;
	if(sigma->letras!=NULL) {
		for(i=0;i<sigma->num_simbolos;i++) {
			if(sigma->letras[i] != NULL) free(sigma->letras[i]);
		}
		free(sigma->letras);
	}
	free(sigma);
}

Letra CopiaLetra(Letra l) {
	return strdup(l);
}
