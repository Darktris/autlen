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
    char **     cadena;
    int         num_cadena;
};

AFND * AFNDNuevo(char * nombre, int num_estados, int num_simbolos) {
    AFND * p_afnd = NULL;
    p_afnd = (AFND*) malloc(sizeof(AFND));
    if(p_afnd == NULL) {
        return NULL;
    }
    p_afnd->nombre = strdup(nombre);
    if(p_afnd->nombre == NULL) {
        free(p_afnd);
        return NULL;
    }
    p_afnd->estados = (Estado **) calloc(num_estados, sizeof(Estado*));
    if(p_afnd->estados == NULL) {
        free(p_afnd->nombre);
        free(p_afnd);
        return NULL;
    }
    p_afnd->num_estados = num_estados;
    p_afnd->sigma = AlfabetoNuevo(num_simbolos);
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
    if(p_afnd->cadena != NULL) {
        free(p_afnd->cadena);
    }
    if(p_afnd->actual != NULL) {
        free(p_afnd->actual);
    }
    if(p_afnd->delta != NULL) {
        FtransElimina(p_afnd->delta);
    }
    if(p_afnd->sigma != NULL) {
        AlfabetoElimina(p_afnd->sigma);
    }
    for(i=0; i<p_afnd->num_estados; i++) {
        if(p_afnd->estados[i] != NULL) {
            EstadoElimina(p_afnd->estados[i]);
        }
    }
    if(p_afnd->estados != NULL) {
        free(p_afnd->estados);
    }
    if(p_afnd->nombre != NULL) {
        free(p_afnd->nombre);
    }
    free(p_afnd);
}

void AFNDImprime(FILE * fd, AFND* p_afnd) {
    int i, len, tlen;
    char * q = NULL, * aux1 = NULL, * aux2 = NULL;
    q = (char *) malloc(sizeof(char));
    if(q == NULL) {
        return;
    }
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
            free(aux1);
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
    aux2 = FtransToString(p_afnd->delta, 2, p_afnd->estados, p_afnd->sigma, p_afnd->num_estados);
    if(aux2 == NULL) {
        free(aux1);
        free(q);
        return;
    }
    fprintf(fd, _AFND_format, p_afnd->nombre, p_afnd->num_simbolos, aux1, p_afnd->num_estados, q, aux2);
    free(aux2);
    free(aux1);
    free(q);
}

AFND * AFNDInsertaSimbolo(AFND * p_afnd, char * simbolo) {
    if(AlfabetoInserta(p_afnd->sigma, simbolo) == NULL) {
        return NULL;
    }
    return p_afnd;
}

AFND * AFNDInsertaEstado(AFND * p_afnd, char * nombre, int tipo) {
    int i;
    for(i=0; i<p_afnd->num_estados; i++) {
        if(!p_afnd->estados[i]) {
            p_afnd->estados[i] = EstadoNuevo(nombre, tipo);
            if(p_afnd->estados[i] == NULL) {
                return NULL;
            }
            return p_afnd;
        }
    }
    return NULL;
}

AFND * AFNDInsertaTransicion(AFND * p_afnd, char * nombre_estado_i, char * nombre_simbolo_entrada, char * nombre_estado_f ) {
    Estado * estado_i = NULL, * estado_f = NULL;
    estado_i = EstadoObtieneConjunto(nombre_estado_i, p_afnd->estados, p_afnd->num_estados);
    if(estado_i == NULL) {
        return NULL;
    }
    estado_f = EstadoObtieneConjunto(nombre_estado_f, p_afnd->estados, p_afnd->num_estados);
    if(estado_f == NULL) {
        return NULL;
    }
    if(FtransInserta(p_afnd->delta, estado_i, estado_f, nombre_simbolo_entrada) == NULL) {
        return NULL;
    }
    return p_afnd;
}

AFND * AFNDInsertaLetra(AFND * p_afnd, char * letra) {
    char * aux1 = NULL, ** aux2 = NULL;
    aux1 = strdup(letra);
    if(aux1 == NULL) {
        return NULL;
    }
    aux2 = (char **) realloc(p_afnd->cadena, (p_afnd->num_cadena+1)*sizeof(char *));
    if(aux2 == NULL) {
        free(aux1);
        return NULL;
    }
    p_afnd->cadena = aux2;
    p_afnd->cadena[p_afnd->num_cadena++] = aux1;
    return p_afnd;
}

void AFNDImprimeConjuntoEstadosActual(FILE * fd, AFND * p_afnd) {
    char * aux = NULL;
    aux = EstadoToStringConjunto(p_afnd->actual, p_afnd->num_actual);
    if(aux == NULL) {
        return;
    }
    fprintf(fd, "{%s}\n", aux);
    free(aux);
}

void AFNDImprimeCadenaActual(FILE *fd, AFND * p_afnd) {
    int i;
    fprintf(fd, "[(%d)", p_afnd->num_cadena);
    for(i=0; i<p_afnd->num_cadena; i++)
        fprintf(fd, " %s", p_afnd->cadena[i]);
    fprintf(fd, "]\n");
}

AFND * AFNDInicializaEstado (AFND * p_afnd) {
    int i;
    for(i=0; i<p_afnd->num_estados; i++) {
        if(EstadoInicial(p_afnd->estados[i])) {
            if(p_afnd->actual != NULL) {
                free(p_afnd->actual);
            }
            p_afnd->actual = (Estado **) calloc(1, sizeof(Estado*));
            if(p_afnd->actual == NULL) {
                return NULL;
            }
            p_afnd->actual[0] = p_afnd->estados[i];
            p_afnd->num_actual = 1;
            return p_afnd;
        }
    }
    return NULL;
}

void AFNDProcesaEntrada(FILE * fd, AFND * p_afnd) {
    char * aux = NULL;
    while(p_afnd->num_cadena>0) {
        aux = EstadoToStringConjunto(p_afnd->actual, p_afnd->num_actual);
        if(aux == NULL) {
            return;
        }
        fprintf(fd, "ACTUALMENTE EN {%s}\n", aux);
        free(aux);
        AFNDImprimeCadenaActual(fd, p_afnd);
        AFNDTransita(p_afnd);
        fprintf(fd, "\n");
    }
    aux = EstadoToStringConjunto(p_afnd->actual, p_afnd->num_actual);
    if(aux == NULL) {
       return;
    }
    fprintf(fd, "ACTUALMENTE EN {%s}\n", aux);
    AFNDImprimeCadenaActual(fd, p_afnd);
    free(aux);
}

void AFNDTransita(AFND * p_afnd) {
    int i, j, num_nuevo=0, num_aux=0;
    Estado ** nuevo = NULL, ** aux1 = NULL, ** aux2 = NULL;
    char ** aux3 = NULL;
    for(i=0, num_nuevo=0; i<p_afnd->num_actual; i++) {
        aux1 = FtransTransita(p_afnd->delta, p_afnd->actual[i], p_afnd->cadena[0], &num_aux);
        if(aux1 == NULL && num_aux != 0) {
            if(nuevo != NULL) {
                free(nuevo);
            }
            return;
        }
        for(j=0; j<num_aux; j++) {
            aux2 = EstadoInsertaConjunto(aux1[j], nuevo, &num_nuevo);
            if(aux2 == NULL) {
                EstadoEliminaConjunto(aux1);
                if(nuevo != NULL) {
                    free(nuevo);
                }
                return;
            }
            nuevo = aux2;
        }
        EstadoEliminaConjunto(aux1);
    }
    EstadoEliminaConjunto(p_afnd->actual);
    p_afnd->actual = nuevo;
    p_afnd->num_actual = num_nuevo;
    free(p_afnd->cadena[0]);
    if(!--p_afnd->num_cadena) {
        free(p_afnd->cadena);
        p_afnd->cadena = NULL;
    } else {
        aux3 = (char **) malloc(p_afnd->num_cadena*sizeof(char *));
        if(aux3 == NULL) {
            return;
        }
        memcpy(aux3, p_afnd->cadena+1, p_afnd->num_cadena*sizeof(char *));
        free(p_afnd->cadena);
        p_afnd->cadena = aux3;
    }
}
