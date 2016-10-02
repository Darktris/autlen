#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "afnd.h"
#include "estado.h"
#include "alfabeto.h"
#include "ftrans.h"

struct AFND {
    char *      nombre;
    Estado **   estados;
    int         num_estados;
    Alfabeto *  sigma;
    int         num_simbolos;
    Ftrans *    delta;
    Estado **   actual;
    int         num_actual;
    char *      cadena;
    int         num_cadena;
    
};

AFND * AFNDNuevo(char * nombre, int num_estados, int num_simbolos) {
    AFND * p_afnd = NULL;
    p_afnd = (AFND*) malloc(sizeof(AFND));
    if(p_afnd == NULL)
        return NULL;
    p_afnd->nombre = strdup(nombre);
    if(p_afnd->nombre == NULL) {
        free(p_afnd);
        return NULL;
    }
    p_afnd->estados = (Estado **) calloc(num_estados, sizeof(estado*));
    if(p_afnd->estados == NULL) {
        free(p_afnd->nombre);
        free(p_afnd);
        return NULL;
    }
    p_afnd->num_estados = num_estados;
    p_afnd->sigma = AlfabetoNuevo();
    if(p_afnd->sigma == NULL) {
        free(p_afnd->estados);
        free(p_afnd->nombre);
        free(p_afnd);
        return NULL;
    }
    p_afnd->num_simbolos = num_simbolos;
    p_afnd->delta = FtransNuevo();
    if(p_afnd->delta == NULL) {
        AlfabetoElimina(p_afnd->sigma);
        free(p_afnd->estados);
        free(p_afnd->nombre);
        free(p_afnd);
    }
    p_afnd->actual = NULL;
    p_afnd->num_actual = 0;
    p_afnd->cadena = NULL;
    p_afnd->num_cadena = 0;
    return p_afnd;
}

void AFNDElimina(AFND * p_afnd) {
    int i;
    if(p_afnd->delta != NULL)
        FTransElimina(p_afnd->delta);
    if(p_afnd->sigma != NULL)
        AlfabetoElimina(p_afnd->sigma);
    for(i=0; i<p_afnd->num_estados; i++) {
        if(p_afnd->estados[i] != NULL)
            EstadoElimina(p_afnd->estados[i]);
    }
    free(p_afnd->estados);
    if(p_afnd->nombre != NULL)
        free(p_afnd->nombre);
    free(p_afnd);
}

void AFNDImprime(FILE * fd, AFND* p_afnd) {
    int i, len, tlen;
    char * q = NULL, * aux1 = NULL, * aux2 = NULL;
    q = (char *) malloc(sizeof(char));
    if(q == NULL)
        return;
    q[0] = ' ';
    tlen = 1;
    for(i=0; i<p_afnd->num_estados; i++) {
        aux1 = EstadoToString(p_afnd->estados[i]);
        if(aux1 == NULL) {
            free(q);
            return;
        }
        len = strlen(aux1);
        aux2 = (char *) realloc(q, (tlen+len+1)*sizeof(char));
        if(aux2 == NULL) {
            free(q);
            return;
        }
        q = aux2;
        memcpy(q+tlen, aux1, len*sizeof(char));
        tlen += len;
        q[tlen++] = ' ';
        free(aux1);
    }
    aux2 = (char *) realloc(q, tlen+2);
    if(aux2 == NULL) {
        free(q);
        return;
    }
    q = aux2;
    q[tlen] = ' ';
    q[tlen+1] = '\0';
    aux1 = AlfabetoToString(p_afnd->sigma);
    if(aux1 == NULL) {
        free(q);
        return;
    }
    aux2 = FtransToString(p_afnd->delta, 2);
    if(aux2 == NULL) {
        free(aux1);
        free(q);
        return;
    }
    fprintf(fd, _AFND_format, p_afnd->nombre, p_afnd->num_simbolos, aux1, p_afnd->num_estados, q, aux2);
}

AFND * AFNDInsertaSimbolo(AFND * p_afnd, char * simbolo) {
    if(AlfabetoInserta(p_afnd->sigma, simbolo) == NULL)
        return NULL;
    return p_afnd;
}

AFND * AFNDInsertaEstado(AFND * p_afnd, char * nombre, int tipo) {
    int i;
    for(i=0; i<p_afnd->num_estados; i++) {
        if(p_afnd->estados[i] == 0) {
            p_afnd->estados[i] = EstadoNuevo(nombre, tipo);
            if(p_afnd->estados[i] == NULL)
                return NULL;
            return p_afnd;
        }
    }
    return NULL;
}

AFND * AFNDInsertaTransicion(AFND * p_afnd, char * nombre_estado_i, char * nombre_simbolo_entrada, char * nombre_estado_f ) {
    if(FTransInserta(p_afnd->delta, nombre_estado_i, nombre_simbolo_entrada, nombre_estado_i) == NULL)
        return NULL;
    return p_afnd;
}

AFND * AFNDInsertaLetra(AFND * p_afnd, char * letra) {
    char * aux = NULL;
    aux = (char *) realloc(p_afnd->cadena, (p_afnd->num_cadena+1));
    if(aux == NULL)
        return NULL;
    p_afnd->cadena = aux;
    p_afnd->cadena[p_afnd->num_cadena++] = *letra;
    return p_afnd;
}

void AFNDImprimeConjuntoEstadosActual(FILE * fd, AFND * p_afnd) {
    int i;
    char * aux = NULL;
    fprintf(fd, "{ ");
    for(i=0; i<p_afnd->num_actual; i++) {
        aux = EstadoToString(p_afnd->actual[i]);
        if(aux != NULL)
            fprintf(fd, "%s ", aux);
    }
    fprintf(fd, "}\n");
}

void AFNDImprimeCadenaActual(FILE *fd, AFND * p_afnd) {
    int i;
    fprintf(fd, "[(%d)", p_afnd->num_cadena);
    for(i=0; i<p_afnd->num_cadena; i++)
        fprintf(fd, " %c", p_afnd->cadena[i]);
    fprintf(fd, "]\n");
}

AFND * AFNDInicializaEstado (AFND * p_afnd) {
    int i;
    for(i=0; i<p_afnd->num_estados; i++) {
        if(EstadoInicial(p_afnd->estados[i])) {
            if(p_afnd->actual != NULL)
                free(p_afnd->actual);
            p_afnd->actual = (Estado **) malloc(sizeof(Estado*));
            if(p_afnd->actual == NULL)
                return NULL;
            p_afnd->actual[0] = p_afnd->estados[i];
            p_afnd->num_actual = 1;
            return p_afnd;
        }
    }
    return NULL;
}

void AFNDProcesaEntrada(FILE * fd, AFND * p_afnd) {

}

void AFNDTransita(AFND * p_afnd) {

}
