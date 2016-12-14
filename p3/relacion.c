#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "relacion.h"

#define IJ(r,i,j) (r)->matriz[(i)*(r)->num_estados+(j)]

struct _Relacion {
    char * matriz;
    int num_estados;
    int num_trans;
    char reflex;
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
    p_r->num_trans = 0;
    p_r->reflex = 0;
    return p_r;
}

char * RelacionToString(Relacion * p_r, char * extra) {
    int i, j, n, l, hlen, slen;
    char * h = NULL, * s = NULL, * aux = NULL;
    hlen = strlen(extra);
    if(hlen > 0) {
        h = (char *) malloc(hlen*sizeof(char));
        if(h == NULL) {
            return NULL;
        }
        memcpy(h, extra, hlen*sizeof(char));
    }
    slen = 0;
    for(i=0; i<p_r->num_estados; i++) {
        l = !i? 1:0;
        for(n=i; n>0; n/=10) l++;
        aux = (char *) realloc(h, (hlen+l+3)*sizeof(char));
        if(aux == NULL) {
            if(h != NULL) {
                free(h);
            }
            if(s != NULL) {
                free(s);
            }
            return NULL;
        }
        h = aux;
        sprintf(h+hlen, "\t[%d", i);
        h[hlen+l+2] = ']';
        hlen += l+3;
        j = strlen(extra);
        if(j > 0) {
            aux = (char *) realloc(s, (slen+j)*sizeof(char));
            if(aux == NULL) {
                if(h != NULL) {
                    free(h);
                }
                if(s != NULL) {
                    free(s);
                }
                return NULL;
            }
            s = aux;
            memcpy(s+slen, extra, j*sizeof(char));
            slen += j;
        }
        aux = (char *) realloc(s, (slen+l+2*p_r->num_estados+3)*sizeof(char));
        if(aux == NULL) {
            if(h != NULL) {
                free(h);
            }
            if(s != NULL) {
                free(s);
            }
            return NULL;
        }
        s = aux;
        sprintf(s+slen, "[%d", i);
        s[slen+l+1] = ']';
        slen += l+2;
        for(j=0; j<p_r->num_estados; j++) {
            sprintf(s+slen, "\t%d", IJ(p_r,i,j));
            slen += 2;
        }
        s[slen++] = '\n';
    }
    aux = (char *) malloc((hlen+slen+2)*sizeof(char));
    if(aux == NULL) {
        if(h != NULL) {
            free(h);
        }
        if(s != NULL) {
            free(s);
        }
        return NULL;
    }
    memcpy(aux, h, hlen*sizeof(char));
    free(h);
    aux[hlen] = '\n';
    memcpy(aux+hlen+1, s, slen*sizeof(char));
    free(s);
    aux[hlen+slen+1] = '\0';
    return aux;
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
    p_r->num_trans = p_r1->num_trans;
    p_r->reflex = p_r1->reflex;
    return p_r;
}

Relacion * RelacionInserta(Relacion * p_r, int i, int j) {
    if(p_r == NULL || i >= p_r->num_estados || j >= p_r->num_estados) {
        return NULL;
    }
    IJ(p_r,i,j) = 1;
    return p_r;
}

int  RelacionObtieneNumEstados(Relacion * p_r) {
    if(p_r == NULL) {
        return 0;
    }
    return p_r->num_estados;
}

int  RelacionObtieneNumTrans(Relacion * p_r) {
    if(p_r == NULL) {
        return 0;
    }
    return p_r->num_trans;
}

char  RelacionObtieneReflex(Relacion * p_r) {
    if(p_r == NULL) {
        return 0;
    }
    return p_r->reflex;
}

Relacion * RelacionCierreReflexivo(Relacion * p_r) {
    int i;
    if(p_r == NULL) {
        return NULL;
    }
    for(i=0; i<p_r->num_estados; i++) {
        IJ(p_r,i,i) = 1;
    }
    p_r->reflex = 1;
    return p_r;
}

Relacion * RelacionCierreTransitivo(Relacion * p_r) {
    int i, j, k;
    Relacion *r_old = NULL;
    if(p_r == NULL) {
        return NULL;
    }

    while(!RelacionCompara(p_r, r_old)) {
        RelacionElimina(r_old);
        r_old = RelacionCopia(p_r);
        for(i=0; i<p_r->num_estados; i++) {
            for(j=0; j<p_r->num_estados; j++) {
                if(IJ(p_r,i,j) == 1) {
                    for(k=0; k<p_r->num_estados; k++) {
                        IJ(p_r,i,k) |= IJ(p_r,j,k);
                    }
                }
            }
        }
        p_r->num_trans++;
    }
    RelacionElimina(r_old);
    return p_r;
}

int  RelacionObtieneEstado(Relacion * p_r, int i, int j) {
    if(p_r == NULL) {
        return 0;
    }
    return IJ(p_r,i,j);
}

int RelacionCompara(Relacion * p_r1, Relacion * p_r2) {
    if(p_r1 == NULL || p_r2 == NULL || p_r1->num_estados != p_r2->num_estados) {
        return 0;
    }
    return !memcmp(p_r1->matriz, p_r2->matriz, p_r1->num_estados*p_r2->num_estados*sizeof(char));
}
