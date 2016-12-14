#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "afnd.h"
#include "estado.h"
#include "alfabeto.h"
#include "relacion.h"
#include "ftrans.h"
#include "tools.h"

struct _AFND {
    char *      nombre;
    Estado **   estados;
    int         num_estados;
    Alfabeto *  sigma;
    Relacion *  lambda;
    Ftrans *    delta;
    Estado **   actual;
    int         num_actual;
    char **     cadena;
    int         num_cadena;
};

AFND * AFNDNuevo(char * nombre, int num_estados, int num_simbolos) {
    AFND * p_afnd = NULL;
    if(nombre == NULL) {
        return NULL;
    }
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
    p_afnd->delta = FtransNuevo();
    if(p_afnd->delta == NULL) {
        AlfabetoElimina(p_afnd->sigma);
        free(p_afnd->estados);
        free(p_afnd->nombre);
        free(p_afnd);
    }

    p_afnd->lambda = RelacionNueva(num_estados);
    if(p_afnd->lambda == NULL) {
        FtransElimina(p_afnd->delta);
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

char * AFNDObtieneNombre(AFND * p_afnd) {
    if(p_afnd == NULL) {
        return NULL;
    }
    return p_afnd->nombre;
}

Alfabeto * AFNDObtieneAlfabeto(AFND * p_afnd) {
    if(p_afnd == NULL) {
        return NULL;
    }
    return p_afnd->sigma;
}

Relacion * AFNDObtieneRelacion(AFND * p_afnd) {
    if(p_afnd == NULL) {
        return NULL;
    }
    return p_afnd->lambda;
}

Ftrans * AFNDObtieneTransicion(AFND * p_afnd) {
    if(p_afnd == NULL) {
        return NULL;
    }
    return p_afnd->delta;
}

Estado ** AFNDObtieneEstados(AFND * p_afnd, int * num_estados) {
    if(p_afnd == NULL || num_estados == NULL) {
        return NULL;
    }
    *num_estados = p_afnd->num_estados;
    return p_afnd->estados;
}

void AFNDElimina(AFND * p_afnd) {
    int i;
    if(p_afnd == NULL) {
        return;
    }
    if(p_afnd->cadena != NULL) {
        free(p_afnd->cadena);
    }
    if(p_afnd->actual != NULL) {
        free(p_afnd->actual);
    }
    if(p_afnd->delta != NULL) {
        FtransElimina(p_afnd->delta);
    }
    if(p_afnd->lambda != NULL) {
        RelacionElimina(p_afnd->lambda);
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
    char * s1 = NULL, * s2 = NULL, * s3 = NULL, * s4 = NULL, * s5 = NULL;
    int i;
    if(fd == NULL || p_afnd == NULL) {
        return;
    }
    s1 = AlfabetoToString(p_afnd->sigma);
    if(s1 == NULL) {
        return;
    }
    s2 = EstadoToStringConjunto(p_afnd->estados, p_afnd->num_estados, 1);
    if(s2 == NULL) {
        free(s1);
        return;
    }
    s3 = RelacionToString(p_afnd->lambda, "\t\t");
    if(s3 == NULL) {
        free(s2);
        free(s1);
        return;
    }
    s4 = FtransToString(p_afnd->delta, "\t\t", p_afnd->estados, p_afnd->sigma, p_afnd->num_estados);
    if(s4 == NULL) {
        free(s3);
        free(s2);
        free(s1);
        return;
    }
    s5 = (char *) malloc((RelacionObtieneNumTrans(p_afnd->lambda)+RelacionObtieneReflex(p_afnd->lambda)+1)*sizeof(char));
    if(s5 == NULL) {
        free(s4);
        free(s3);
        free(s2);
        free(s1);
        return;
    }
    for(i=0; i<RelacionObtieneNumTrans(p_afnd->lambda); i++) {
        s5[i] = '+';
    }
    if(RelacionObtieneReflex(p_afnd->lambda) == 1) {
        s5[i++] = '*';
    }
    s5[i] = '\0';
    fprintf(fd, _AFND_format, p_afnd->nombre, AlfabetoObtieneNumSimbolos(p_afnd->sigma), s1, p_afnd->num_estados, s2, s5, s3, s4);
    free(s1);
    free(s2);
    free(s3);
    free(s4);
    free(s5);
}

AFND * AFNDInsertaSimbolo(AFND * p_afnd, char * simbolo) {
    if(p_afnd == NULL || simbolo == NULL) {
        return NULL;
    }
    if(AlfabetoInserta(p_afnd->sigma, simbolo) == NULL) {
        return NULL;
    }
    return p_afnd;
}

AFND * AFNDInsertaEstado(AFND * p_afnd, char * nombre, int tipo) {
    int i;
    if(p_afnd == NULL || nombre == NULL) {
        return NULL;
    }
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
    if(p_afnd == NULL || nombre_estado_i == NULL || nombre_simbolo_entrada == NULL || nombre_estado_f == NULL) {
        return NULL;
    }
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

AFND * AFNDInsertaLTransicion(AFND * p_afnd, char * nombre_estado_i, char * nombre_estado_f) {
    int i = 0, f = 0;
    Relacion * r = NULL;
    if(p_afnd == NULL) {
        return NULL;
    }
    i = IndiceObtieneConjunto(nombre_estado_i, p_afnd->estados, p_afnd->num_estados);
    f = IndiceObtieneConjunto(nombre_estado_f, p_afnd->estados, p_afnd->num_estados);
    if(i < 0 || f < 0) {
        return NULL;
    }
    r = RelacionInserta(p_afnd->lambda, i, f);
    if(r == NULL) {
        return NULL;
    }
    return p_afnd;
}

AFND * AFNDCierraLTransicion(AFND * p_afnd) {
    Relacion * r = NULL;
    if(p_afnd == NULL) {
        return NULL;
    }
    r = RelacionCierreReflexivo(p_afnd->lambda);
    if(r == NULL) {
        return NULL;
    }
    p_afnd->lambda = r;
    r = RelacionCierreTransitivo(p_afnd->lambda);
    if(r == NULL) {
        return NULL;
    }
    p_afnd->lambda = r;
    return p_afnd;
}

AFND * AFNDInsertaLetra(AFND * p_afnd, char * letra) {
    char * aux1 = NULL, ** aux2 = NULL;
    if(p_afnd == NULL || letra == NULL) {
        return NULL;
    }

    /*if(!LetraPerteneceAAlfabeto(p_afnd->sigma, letra)) {
        return NULL;
    }*/
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
    if(fd == NULL || p_afnd == NULL) {
        return;
    }
    aux = EstadoToStringConjunto(p_afnd->actual, p_afnd->num_actual, 1);
    if(aux == NULL) {
        return;
    }
    fprintf(fd, "{%s}\n", aux);
    free(aux);
}

AFND * AFNDInicializaCadenaActual(AFND * p_afnd) {
    int i;
    for(i=0; i<p_afnd->num_cadena; i++) {
        free(p_afnd->cadena[i]);
    }
    free(p_afnd->cadena);
    p_afnd->num_cadena = 0;
    return p_afnd;
}

void AFNDImprimeCadenaActual(FILE *fd, AFND * p_afnd) {
    int i;
    if(fd == NULL || p_afnd == NULL) {
        return;
    }
    fprintf(fd, "[(%d)", p_afnd->num_cadena);
    for(i=0; i<p_afnd->num_cadena; i++)
        fprintf(fd, " %s", p_afnd->cadena[i]);
    fprintf(fd, "]\n");
}

AFND * AFNDInicializaEstado (AFND * p_afnd) {
    int i;
    if(p_afnd == NULL) {
        return NULL;
    }
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

void AFNDCerrarConjuntoActual(AFND * p_afnd) {
    int i = 0, k = 0, j = 0, l = 0, n = 0;
    Estado** set = NULL;

    for(i=0; i<p_afnd->num_actual; i++) {
        k = IndiceObtieneConjunto(EstadoNombre(p_afnd->actual[i]), p_afnd->estados, p_afnd->num_estados);
        for(j=0; j<p_afnd->num_estados; j++) {
            l = RelacionObtieneEstado(p_afnd->lambda, k, j);
            if(l) {
                set = EstadoInsertaConjunto(p_afnd->estados[j], set, &n);
                if (set == NULL) {
                    return;
                }
            }
        }
    }

    EstadoEliminaConjunto(p_afnd->actual);
    p_afnd->actual = set;
    p_afnd->num_actual = n;
}

void AFNDProcesaEntrada(FILE * fd, AFND * p_afnd) {
    char * aux = NULL;
    if(fd == NULL || p_afnd == NULL) {
        return;
    }
    while(p_afnd->num_cadena>0 && p_afnd->num_actual>0) {
        AFNDCerrarConjuntoActual(p_afnd);
        aux = EstadoToStringConjunto(p_afnd->actual, p_afnd->num_actual, 1);
        if(aux == NULL) {
            return;
        }
        fprintf(fd, "\nACTUALMENTE EN {%s}\n", aux);
        free(aux);
        AFNDImprimeCadenaActual(fd, p_afnd);
        AFNDTransita(p_afnd);

    }
    AFNDCerrarConjuntoActual(p_afnd);
    aux = EstadoToStringConjunto(p_afnd->actual, p_afnd->num_actual, 1);
    if(aux == NULL) {
       return;
    }
    fprintf(fd, "\nACTUALMENTE EN {%s}\n", aux);
    AFNDImprimeCadenaActual(fd, p_afnd);
    while(p_afnd->num_cadena > 0) {
        AFNDTransita(p_afnd);
        AFNDCerrarConjuntoActual(p_afnd);
    }
    free(aux);
}

void AFNDTransita(AFND * p_afnd) {
    int i, j, num_nuevo=0, num_aux=0;
    Estado ** nuevo = NULL, ** aux1 = NULL, ** aux2 = NULL;
    char ** aux3 = NULL;
    if(p_afnd == NULL) {
        return;
    }
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

void AFNDADot(AFND * p_afnd) {

}
