#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "afnd1O.h"
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
    AFND * p_afnd1O = NULL;
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
    sprintf(nombre, "%s1O", nombre);

    sigma = AFNDObtieneAlfabeto(p_afnd);
    if(sigma == NULL) {
        free(nombre);
        return NULL;
    }
    num_simbolos = AlfabetoObtieneNumSimbolos(sigma);

    delta = AFNDObtieneTransicion(p_afnd);
    if(delta == NULL) {
        free(nombre);
        return NULL;
    }

    estados = AFNDObtieneEstados(p_afnd, &num_estados);
    if(estados == NULL) {
        free(nombre);
        return NULL;
    }

    p_afnd1O = AFNDNuevo(nombre, num_simbolos, num_estados+2);
    free(nombre);
    if(p_afnd1O == NULL) {
        return NULL;
    }

    for(i=0; i<num_simbolos; i++) {
        AFNDInsertaSimbolo(p_afnd1O, AlfabetoObtieneLetra(sigma, i));
    }

    l = !num_estados? 1:0;
    for(n=num_estados; n>0; n/=10) l++;
    nombre = (char *) malloc((l+1)*sizeof(char));
    if(nombre == NULL) {
        AFNDElimina(p_afnd1O);
        return NULL;
    }
    for(i=0; i<num_estados; i++) {
        sprintf(nombre, "q%d", i+1);
        AFNDInsertaEstado(p_afnd1O, nombre, NORMAL);
    }
    AFNDInsertaEstado(p_afnd1O, "q0", INICIAL);
    AFNDInsertaEstado(p_afnd1O, "qf", FINAL);

    naux = (char *) malloc((l+1)*sizeof(char));
    if(naux == NULL) {
        AFNDElimina(p_afnd1O);
        return NULL;
    }
    for(i=0; i<num_estados; i++) {
        sprintf(nombre, "q%d", i+1);
        if(EstadoInicial(estados[i])) {
            AFNDInsertaLTransicion(p_afnd1O, "q0", nombre);
        }
        if(EstadoFinal(estados[i])) {
            AFNDInsertaLTransicion(p_afnd1O, nombre, "qf");
        }
        for(j=0; j<num_simbolos; j++) {
            aux = AlfabetoObtieneLetra(sigma, j);
            eaux = FtransTransita(delta, estados[i], aux, &n);
            for(l=0; l<n; l++) {
                sprintf(naux, "q%d", IndiceObtieneConjunto(EstadoNombre(eaux[l]), estados, num_estados+2));
                AFNDInsertaTransicion(p_afnd1O, nombre, aux, naux);
            }
            EstadoEliminaConjunto(eaux);
        }
    }
    free(nombre);
    free(naux);
    return p_afnd1O;
}

AFND * AFND1OUne(AFND * p_afnd1O_1, AFND * p_afnd1O_2) {
    AFND * p_afnd1O = NULL;
    char * nombre = NULL, * aux1 = NULL, * aux2 = NULL;
    Alfabeto * sigma = NULL, * sigma1 = NULL, * sigma2 = NULL;
    Ftrans * delta1 = NULL, * delta2 = NULL;
    Estado ** estados1 = NULL, ** estados2 = NULL, ** eaux = NULL;
    int num_simbolos, num_estados, num_estados1, num_estados2, i, j, l, n;

    aux1 = strdup(AFNDObtieneNombre(p_afnd1O_1));
    if(aux1 == NULL) {
        return NULL;
    }
    aux2 = strdup(AFNDObtieneNombre(p_afnd1O_2));
    if(aux2 == NULL) {
        free(aux1);
        return NULL;
    }
    nombre = (char *) malloc((strlen(aux1)+strlen(aux2)+4)*sizeof(char));
    if(nombre == NULL) {
        free(aux1);
        free(aux2);
        return NULL;
    }
    sprintf(nombre, "%s_U_%s", aux2, aux2);
    free(aux1);
    free(aux2);

    sigma1 = AFNDObtieneAlfabeto(p_afnd1O_1);
    if(sigma1 == NULL) {
        free(nombre);
        return NULL;
    }
    sigma2 = AFNDObtieneAlfabeto(p_afnd1O_2);
    if(sigma2 == NULL) {
        free(nombre);
        return NULL;
    }
    sigma = AlfabetoUne(sigma1, sigma2, &num_simbolos);
    if(sigma == NULL) {
        free(nombre);
        return NULL;
    }

    delta1 = AFNDObtieneTransicion(p_afnd1O_1);
    if(delta1 == NULL) {
        free(nombre);
        AlfabetoElimina(sigma);
        return NULL;
    }
    delta2 = AFNDObtieneTransicion(p_afnd1O_2);
    if(delta2 == NULL) {
        free(nombre);
        AlfabetoElimina(sigma);
        return NULL;
    }

    estados1 = AFNDObtieneEstados(p_afnd1O_1, &num_estados1);
    if(estados1 == NULL) {
        free(nombre);
        AlfabetoElimina(sigma);
        return NULL;
    }
    estados2 = AFNDObtieneEstados(p_afnd1O_2, &num_estados2);
    if(estados2 == NULL) {
        free(nombre);
        AlfabetoElimina(sigma);
        return NULL;
    }
    num_estados = num_estados1 + num_estados2;

    p_afnd1O = AFNDNuevo(nombre, num_simbolos, num_estados+2);
    free(nombre);
    if(p_afnd1O == NULL) {
        AlfabetoElimina(sigma);
        return NULL;
    }

    for(i=0; i<num_simbolos; i++) {
        AFNDInsertaSimbolo(p_afnd1O, AlfabetoObtieneLetra(sigma, i));
    }
    AlfabetoElimina(sigma);

    l = !num_estados? 1:0;
    for(n=num_estados; n>0; n/=10) l++;
    nombre = (char *) malloc((l+1)*sizeof(char));
    if(nombre == NULL) {
        AFNDElimina(p_afnd1O);
        return NULL;
    }
    AFNDInsertaEstado(p_afnd1O, "q0", INICIAL);
    AFNDInsertaEstado(p_afnd1O, "qf", FINAL);

    for(i=0; i<num_estados1; i++) {
        sprintf(nombre, "q%d", i+1);
        AFNDInsertaEstado(p_afnd1O, nombre, NORMAL);
    }
    aux2 = (char *) malloc((l+1)*sizeof(char));
    if(aux2 == NULL) {
        AFNDElimina(p_afnd1O);
        return NULL;
    }
    for(i=0; i<num_estados1; i++) {
        sprintf(nombre, "q%d", i+1);
        if(EstadoInicial(estados1[i])) {
            AFNDInsertaLTransicion(p_afnd1O, "q0", nombre);
        }
        if(EstadoFinal(estados1[i])) {
            AFNDInsertaLTransicion(p_afnd1O, nombre, "qf");
        }
        for(j=0; j<AlfabetoObtieneNumSimbolos(sigma1); j++) {
            aux1 = AlfabetoObtieneLetra(sigma1, j);
            eaux = FtransTransita(delta1, estados1[i], aux1, &n);
            for(l=0; l<n; l++) {
                sprintf(aux2, "q%d", IndiceObtieneConjunto(EstadoNombre(eaux[l]), estados1, num_estados1) + 1);
                AFNDInsertaTransicion(p_afnd1O, nombre, aux1, aux2);
            }
            EstadoEliminaConjunto(eaux);
        }
    }

    for(i=0; i<num_estados2; i++) {
        sprintf(nombre, "q%d", num_estados1+i+1);
        AFNDInsertaEstado(p_afnd1O, nombre, NORMAL);
    }
    aux2 = (char *) malloc((l+1)*sizeof(char));
    if(aux2 == NULL) {
        AFNDElimina(p_afnd1O);
        return NULL;
    }
    for(i=0; i<num_estados2; i++) {
        sprintf(nombre, "q%d", num_estados1+i+1);
        if(EstadoInicial(estados2[i])) {
            AFNDInsertaLTransicion(p_afnd1O, "q0", nombre);
        }
        if(EstadoFinal(estados2[i])) {
            AFNDInsertaLTransicion(p_afnd1O, nombre, "qf");
        }
        for(j=0; j<AlfabetoObtieneNumSimbolos(sigma2); j++) {
            aux1 = AlfabetoObtieneLetra(sigma2, j);
            eaux = FtransTransita(delta2, estados2[i], aux1, &n);
            for(l=0; l<n; l++) {
                sprintf(aux2, "q%d", num_estados1 + IndiceObtieneConjunto(EstadoNombre(eaux[l]), estados2, num_estados2) + 1);
                AFNDInsertaTransicion(p_afnd1O, nombre, aux1, aux2);
            }
            EstadoEliminaConjunto(eaux);
        }
    }
    free(nombre);
    free(aux2);
    return p_afnd1O;
}

AFND * AFND1OConcatena(AFND * p_afnd_1, AFND * p_afnd_2) {
    AFND * p_afnd1O = NULL;
    char * nombre = NULL, * aux1 = NULL, * aux2 = NULL;
    Alfabeto * sigma = NULL, * sigma1 = NULL, * sigma2 = NULL;
    Ftrans * delta1 = NULL, * delta2 = NULL;
    Estado ** estados1 = NULL, ** estados2 = NULL, ** eaux = NULL;
    int num_simbolos, num_estados, num_estados1, num_estados2, i, j, l, n;

    aux1 = strdup(AFNDObtieneNombre(p_afnd_1));
    if(aux1 == NULL) {
        return NULL;
    }
    aux2 = strdup(AFNDObtieneNombre(p_afnd_2));
    if(aux2 == NULL) {
        free(aux1);
        return NULL;
    }
    nombre = (char *) malloc((strlen(aux1)+strlen(aux2)+4)*sizeof(char));
    if(nombre == NULL) {
        free(aux1);
        free(aux2);
        return NULL;
    }
    sprintf(nombre, "%s_C_%s", aux2, aux2);
    free(aux1);
    free(aux2);

    sigma1 = AFNDObtieneAlfabeto(p_afnd_1);
    if(sigma1 == NULL) {
        free(nombre);
        return NULL;
    }
    sigma2 = AFNDObtieneAlfabeto(p_afnd_2);
    if(sigma2 == NULL) {
        free(nombre);
        return NULL;
    }
    sigma = AlfabetoUne(sigma1, sigma2, &num_simbolos);
    if(sigma == NULL) {
        free(nombre);
        return NULL;
    }

    delta1 = AFNDObtieneTransicion(p_afnd_1);
    if(delta1 == NULL) {
        free(nombre);
        AlfabetoElimina(sigma);
        return NULL;
    }
    delta2 = AFNDObtieneTransicion(p_afnd_2);
    if(delta2 == NULL) {
        free(nombre);
        AlfabetoElimina(sigma);
        return NULL;
    }

    estados1 = AFNDObtieneEstados(p_afnd_1, &num_estados1);
    if(estados1 == NULL) {
        free(nombre);
        AlfabetoElimina(sigma);
        return NULL;
    }
    estados2 = AFNDObtieneEstados(p_afnd_2, &num_estados2);
    if(estados2 == NULL) {
        free(nombre);
        AlfabetoElimina(sigma);
        return NULL;
    }
    num_estados = num_estados1 + num_estados2;

    p_afnd1O = AFNDNuevo(nombre, num_simbolos, num_estados+2);
    free(nombre);
    if(p_afnd1O == NULL) {
        AlfabetoElimina(sigma);
        return NULL;
    }

    for(i=0; i<num_simbolos; i++) {
        AFNDInsertaSimbolo(p_afnd1O, AlfabetoObtieneLetra(sigma, i));
    }
    AlfabetoElimina(sigma);

    l = !num_estados? 1:0;
    for(n=num_estados; n>0; n/=10) l++;
    nombre = (char *) malloc((l+1)*sizeof(char));
    if(nombre == NULL) {
        AFNDElimina(p_afnd1O);
        return NULL;
    }
    AFNDInsertaEstado(p_afnd1O, "q0", INICIAL);
    AFNDInsertaEstado(p_afnd1O, "qf", FINAL);

    for(i=0; i<num_estados1; i++) {
        sprintf(nombre, "q%d", i+1);
        AFNDInsertaEstado(p_afnd1O, nombre, NORMAL);
    }
    aux2 = (char *) malloc((l+1)*sizeof(char));
    if(aux2 == NULL) {
        AFNDElimina(p_afnd1O);
        return NULL;
    }
    for(i=0; i<num_estados1; i++) {
        sprintf(nombre, "q%d", i+1);
        if(EstadoInicial(estados1[i])) {
            AFNDInsertaLTransicion(p_afnd1O, "q0", nombre);
        }
        for(j=0; j<AlfabetoObtieneNumSimbolos(sigma1); j++) {
            aux1 = AlfabetoObtieneLetra(sigma1, j);
            eaux = FtransTransita(delta1, estados1[i], aux1, &n);
            for(l=0; l<n; l++) {
                sprintf(aux2, "q%d", IndiceObtieneConjunto(EstadoNombre(eaux[l]), estados1, num_estados1) + 1);
                AFNDInsertaTransicion(p_afnd1O, nombre, aux1, aux2);
            }
            EstadoEliminaConjunto(eaux);
        }
    }

    for(i=0; i<num_estados2; i++) {
        sprintf(nombre, "q%d", num_estados1+i+1);
        AFNDInsertaEstado(p_afnd1O, nombre, NORMAL);
    }
    aux2 = (char *) malloc((l+1)*sizeof(char));
    if(aux2 == NULL) {
        AFNDElimina(p_afnd1O);
        return NULL;
    }
    for(i=0; i<num_estados2; i++) {
        sprintf(nombre, "q%d", num_estados1+i+1);
        if(EstadoFinal(estados2[i])) {
            AFNDInsertaLTransicion(p_afnd1O, nombre, "qf");
        }
        for(j=0; j<AlfabetoObtieneNumSimbolos(sigma2); j++) {
            aux1 = AlfabetoObtieneLetra(sigma2, j);
            eaux = FtransTransita(delta2, estados2[i], aux1, &n);
            for(l=0; l<n; l++) {
                sprintf(aux2, "q%d", num_estados1 + IndiceObtieneConjunto(EstadoNombre(eaux[l]), estados2, num_estados2) + 1);
                AFNDInsertaTransicion(p_afnd1O, nombre, aux1, aux2);
            }
            EstadoEliminaConjunto(eaux);
        }
    }
    for(i=0; i<num_estados1; i++) {
        for(j=0; j<num_estados2; j++) {
            if(EstadoFinal(estados1[i]) && EstadoInicial(estados2[j])) {
                sprintf(nombre, "q%d", i+1);
                sprintf(aux2, "q%d", num_estados1 + IndiceObtieneConjunto(EstadoNombre(eaux[l]), estados2, num_estados2) + 1);
                AFNDInsertaLTransicion(p_afnd1O, nombre, aux2);
            }
        }
    }
    free(nombre);
    free(aux2);
    return p_afnd1O;
}

AFND * AFND1OEstrella(AFND * p_afnd_origen) {

    return NULL;
}
