#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "afnd10.h"
#include "tools.h"

AFND * AFND1ODeSimbolo(char * simbolo) {
    AFND * p_afnd = NULL;
    p_afnd = AFNDNuevo(simbolo, 2, 1);
    AFNDInsertaSimbolo(p_afnd, simbolo);
    AFNDInsertaEstado(p_afnd, "q0", INICIAL);
    AFNDInsertaEstado(p_afnd, "qf", FINAL);
    AFNDInsertaTransicion(p_afnd, "q0", simbolo, "qf");
    return p_afnd;
}

AFND * AFND1ODeLambda() {
    AFND * p_afnd = NULL;
    p_afnd = AFNDNuevo("lambda", 2, 0);
    AFNDInsertaEstado(p_afnd, "q0", INICIAL);
    AFNDInsertaEstado(p_afnd, "qf", FINAL);
    AFNDInsertaLTransicion(p_afnd, "q0", "qf");
    return p_afnd;
}

AFND * AFND1ODeVacio() {
    AFND * p_afnd = NULL;
    p_afnd = AFNDNuevo("vacio", 2, 0);
    AFNDInsertaEstado(p_afnd, "q0", INICIAL);
    AFNDInsertaEstado(p_afnd, "qf", FINAL);
    return p_afnd;
}

AFND * AFNDAAFND1O(AFND * p_afnd) {
    AFND * p_afnd10 = NULL;
    char * nombre = NULL, * aux = NULL, * naux = NULL;
    Alfabeto * sigma = NULL;
    Ftrans * delta = NULL;
    Estado ** estados = NULL, ** eaux = NULL;
    int num_simbolos, num_estados, i, j, l, n;

    nombre = strdup(AFNDObtieneNombre(p_afnd));
    if(nombre == NULL) {
        return NULL;
    }
    aux = (char *) realloc(nombre, (strlen(nombre)+3)*sizeof(char));
    if(aux == NULL) {
        free(nombre);
        return NULL;
    }
    nombre = aux;
    sprintf(nombre, "%s10", nombre);

    sigma = AFNDObtieneAlfabeto(p_afnd);
    if(sigma == NULL) {
        free(nombre);
        return NULL;
    }
    num_simbolos = AlfabetoObtieneNumSimbolos(sigma);

    delta = AFNDObtieneTransicion(p_afnd);
    if(delta == NULL) {
        free(nombre);
    }

    estados = AFNDObtieneEstados(p_afnd, &num_estados);

    p_afnd10 = AFNDNuevo(nombre, num_simbolos, num_estados+2);
    free(nombre);
    if(p_afnd10 == NULL) {
        return NULL;
    }

    for(i=0; i<num_simbolos; i++) {
        AFNDInsertaSimbolo(p_afnd10, AlfabetoObtieneLetra(sigma, i));
    }

    l = !num_estados? 1:0;
    for(n=num_estados; n>0; n/=10) l++;
    nombre = (char *) malloc((l+1)*sizeof(char));
    for(i=0; i<num_estados; i++) {
        sprintf(nombre, "q%d", i+1);
        AFNDInsertaEstado(p_afnd10, nombre, NORMAL);
    }
    AFNDInsertaEstado(p_afnd, "q0", INICIAL);
    AFNDInsertaEstado(p_afnd, "qf", FINAL);

    naux = (char *) malloc((l+1)*sizeof(char));
    for(i=0; i<num_estados; i++) {
        sprintf(nombre, "q%d", i+1);
        if(EstadoInicial(estados[i])) {
            AFNDInsertaLTransicion(p_afnd10, "q0", nombre);
        }
        if(EstadoFinal(estados[i])) {
            AFNDInsertaLTransicion(p_afnd10, nombre, "qf");
        }
        for(j=0; j<num_simbolos; j++) {
            aux = AlfabetoObtieneLetra(sigma, j);
            eaux = FtransTransita(delta, estados[i], aux, &n);
            for(l=0; l<n; l++) {
                sprintf(naux, "q%d", IndiceObtieneConjunto(EstadoNombre(eaux[l]), estados, num_estados));
                AFNDInsertaTransicion(p_afnd10, nombre, aux, naux);
            }
            EstadoEliminaConjunto(eaux);
        }
    }
    free(nombre);
    free(naux);
    return p_afnd;
}

AFND * AFND1OUne(AFND * p_afnd1O_1, AFND * p_afnd1O_2) {

    return NULL;
}

AFND * AFND1OConcatena(AFND * p_afnd_origen1, AFND * p_afnd_origen2) {

    return NULL;
}

AFND * AFND1OEstrella(AFND * p_afnd_origen) {

    return NULL;
}
