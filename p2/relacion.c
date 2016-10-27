#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "relacion.h"

#define IJ(r,i,j) (r)->matriz[(i)*(r)->num_estados+(j)]

struct _Relacion {
    char * matriz;
    int num_estados;
};

Relacion * RelacionNueva(int num_estados) {
    Relacion * p_r = NULL;
    p_r = (Relacion *) calloc(1, sizeof(Relacion));
    if(p_r == NULL) {
        return NULL;
    }
    p_r->matriz = (char *) calloc(num_estados*num_estados, sizeof(char));
    if(p_r->matriz == NULL) {
        free(p_r);
        return NULL;
    }
    p_r->num_estados = num_estados;
    return p_r;
}

char * RelacionToString(Relacion * p_r) {
    return NULL;
}

void RelacionElimina(Relacion * p_r) {
    if(p_r == NULL) {
        return;
    }
    if(p_r->matriz != NULL) {
        free(p_r->matriz);
    }
    free(p_r);
}

Relacion * RelacionCopia(Relacion * p_r1) {
    Relacion * p_r = NULL;
    if(p_r1 == NULL) {
        return NULL;
    }
    p_r = RelacionNueva(p_r1->num_estados);
    if(p_r == NULL) {
        return NULL;
    }
    memcpy(p_r->matriz, p_r1->matriz, p_r1->num_estados*p_r1->num_estados*sizeof(char));
    return p_r;
}

Relacion * RelacionInserta(Relacion * p_r, int i, int j) {
    if(p_r == NULL || i >= p_r->num_estados || j >= p_r->num_estados) {
        return NULL;
    }
    IJ(p_r, i, j) = 1;
    return p_r;
}

int  RelacionObtieneNumEstados(Relacion * p_r) {
    if(p_r == NULL) {
        return 0;
    }
    return p_r->num_estados;
}

Relacion * RelacionCierreReflexivo(Relacion * p_r) {
    int i;
    if(p_r == NULL) {
        return NULL;
    }
    for(i=0; i<p_r->num_estados; i++) {
        IJ(p_r,i,i) = 1;
    }
    return p_r;
}

Relacion * RelacionCierreTransitivo(Relacion * p_r) {
    int i, j, k;
    if(p_r == NULL) {
        return NULL;
    }
    for(i=0; i<p_r->num_estados; i++) {
        for(j=0; j<p_r->num_estados; j++) {
            if(IJ(p_r,i,j) == 1) {
                for(k=0; k<p_r->num_estados; k++) {
                    IJ(p_r,i,k) |= IJ(p_r,j,k);
                }
            }
        }
    }
    return p_r;
}

int  RelacionObtieneEstado(Relacion * p_r, int i, int j) {
    if(p_r == NULL) {
        return 0;
    }
    return IJ(p_r,i,j);
}
