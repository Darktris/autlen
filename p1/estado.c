#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "estado.h"
#include "tools.h"

Estado* EstadoNuevo(char* nombre, int tipo) {
    Estado *q;
    if(nombre == NULL) return NULL;
    q = calloc(1, sizeof(Estado));
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

char* EstadoToString(Estado* q, int mostrar_tipo) {
    char* string = NULL;
    if(q == NULL) return NULL;
    if(!mostrar_tipo || q->tipo == NORMAL) {
        string = (char *) malloc((strlen(q->nombre)+1)*sizeof(char));
        if(string == NULL) {
            return NULL;
        }
        sprintf(string, "%s", q->nombre);
    } else if (q->tipo == FINAL) {
        string = (char *) malloc((strlen(q->nombre)+2)*sizeof(char));
        if(string == NULL) {
            return NULL;
        }
        sprintf(string, "%s*", q->nombre);
    } else if (q->tipo == INICIAL) {
        string = (char *) malloc((strlen(q->nombre)+3)*sizeof(char));
        if(string == NULL) {
            return NULL;
        }
        sprintf(string, "->%s", q->nombre);
    }
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
    return (strcmp(i->nombre, j->nombre)==0);
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

Estado * EstadoObtieneConjunto(char* nombre, Estado** array, int len) {
    int i;
    if(nombre == NULL || array == NULL) return NULL;
    for(i=0; i<len; i++) {
        if(!strcmp(nombre, array[i]->nombre))
                return array[i];
    }
    return NULL;
}

void EstadoEliminaConjunto(Estado** array) {
    if(array != NULL)
        free(array);
}

Estado** EstadoInsertaConjunto(Estado* q, Estado** array, int* len) {
    Estado** aux;
    if(q == NULL || len == NULL) return array;
    if(array == NULL) {
        array = calloc(1, sizeof(Estado*));
        if(array == NULL) {
            return NULL;
        }
        *len = 1;
        array[0] = q;
        return array;
    }

    if(EstadoPerteneceAConjunto(q, array, *len)) return array;
    aux = realloc(array, (*len+1)*sizeof(Estado*));
    if(aux == NULL) {
        return array;
    }
    (*len)++;

    array = aux;
    array[(*len)-1] = q;
    return array;
}

char* EstadoToStringConjunto(Estado** q, int size, int mostrar_tipo) {
    char * s = NULL, * aux1 = NULL, * aux2 = NULL;
    int i, len, tlen;
    tlen = 0;
    for(i=0; i<size; i++) {
        aux1 = EstadoToString(q[i], mostrar_tipo);
        if(aux1 == NULL) {
            free(s);
            return NULL;
        }
        len = strlen(aux1);
        aux2 = (char *) realloc(s, (tlen+len+1)*sizeof(char));
        if(aux2 == NULL) {
            free(aux1);
            free(s);
            return NULL;
        }
        s = aux2;
        memcpy(s+tlen, aux1, len*sizeof(char));
        tlen += len;
        s[tlen++] = ' ';
        free(aux1);
    }
    aux2 = (char *) realloc(s, tlen+1);
    if(aux2 == NULL) {
        free(s);
        return NULL;
    }
    s = aux2;
    s[tlen] = '\0';
    return s;
}
