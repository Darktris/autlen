#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "alfabeto.h"

Alfabeto* AlfabetoNuevo(int num_simbolos) {
    Alfabeto* sigma = calloc(1, sizeof(Alfabeto));
    sigma->letras = calloc(num_simbolos, sizeof(Letra));
    sigma->num_simbolos = num_simbolos;
    return sigma;
}

Alfabeto* AlfabetoInserta(Alfabeto* sigma, Letra l) {
    Letra* it;
    it = sigma->letras;
    while((*it)!=NULL) it++;
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

void LiberaLetra(Letra l) {
    if(l != NULL) free(l);
}

char* AlfabetoToString(Alfabeto* sigma) {
    int i, len, tlen;
    char *string, *aux;

    string = (char *) malloc(sizeof(char));
    if(string == NULL) {
        return NULL;
    }
    string[0] = ' ';
    tlen = 1;
    for(i=0; i<sigma->num_simbolos; i++) {
        len = strlen(sigma->letras[i]);
        aux = (char *) realloc(string, (tlen+len+1)*sizeof(char));
        if(aux == NULL) {
            free(string);
            return NULL;
        }
        string = aux;
        memcpy(string+tlen, sigma->letras[i], len+sizeof(char));
        tlen += len;
        string[tlen++] = ' ';
    }
    aux = (char *) realloc (string, tlen+1);
    if(aux == NULL) {
        free(string);
        return NULL;
    }
    string = aux;
    string[tlen] = '\0';
    return string;
}

int ComparaLetra(Letra a, Letra b) {
    return !strcmp(a, b);
}
