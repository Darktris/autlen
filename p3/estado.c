#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "estado.h"
#include "tools.h"

struct _Estado {
    char* nombre;
    int tipo;
};

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
    } else if (q->tipo == INICIAL || q->tipo == INICIAL_Y_FINAL) {
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
     return q->tipo == INICIAL || q->tipo == INICIAL_Y_FINAL;
}
int EstadoFinal(Estado* q) {
    if(q == NULL) return 0;
    return q->tipo == FINAL || q->tipo == INICIAL_Y_FINAL;

}

int EstadoCompara(Estado *i, Estado* j) {
    if(i == NULL || j == NULL) return 0;
    return (strcmp(i->nombre, j->nombre)==0);
}

char * EstadoNombre(Estado * q) {
    if(q == NULL) {
        return NULL;
    }
    return q->nombre;
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

int IndiceObtieneConjunto(char* nombre, Estado** array, int len) {
    int i;
    if(nombre == NULL || array == NULL) return -1;
    for(i=0; i<len; i++) {
        if(!strcmp(nombre, array[i]->nombre))
                return i;
    }
    return -1;
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

Estado** EstadoUneConjunto1(Estado** array1, int len1, Estado** array2, int len2, int* num_estados) {
    Estado** res;
    int i;
    Estado * estado;
    if(array1 == NULL || array2 == NULL) {
        return NULL;
    }

    res = calloc(sizeof(Estado*), len1+len2);

    for(i=0; i<len1; i++) {
        estado = calloc(sizeof(Estado), 1);
        estado->nombre = calloc(sizeof(char), 4 + strlen(array1[i]->nombre) + 1);
        if(estado->nombre == NULL) {
            /** TODO ***/
            return NULL;
        }

        sprintf(estado->nombre, "_U1_%s", array1[i]->nombre);
        estado->tipo = array1[i]->tipo;

        res[i] = estado;
    }

    for(i=0; i<len2; i++) {
        estado = calloc(sizeof(Estado), 1);
        estado->nombre = calloc(sizeof(char), 4 + strlen(array2[i]->nombre) + 1);
        if(estado->nombre == NULL) {
            /** TODO ***/
            return NULL;
        }

        sprintf(estado->nombre, "_U2_%s", array2[i]->nombre);
        estado->tipo = array2[i]->tipo;

        res[len1+i] = estado;
    }

    return res;


}

Estado** EstadoUneConjunto(Estado** array1, int len1, Estado** array2, int len2, int* num_estados) {
    Estado** res;
    int i;
    if(array1 == NULL || array2 == NULL) {
        return NULL;
    }

    res = calloc(sizeof(Estado*), len1+len2);

    for(i=0; i<len1; i++) {
        res[i] = array1[i];
    }

    for(i=0; i<len2; i++) {
        res[i+len1] = array2[i];
    }

    return res;
    

}