#ifndef _RELACION_H
#define _RELACION_H

struct _Relacion;
typedef struct _Relacion Relacion;

Relacion * RelacionNueva(int num_estados);
char * RelacionToString(Relacion * p_r);
void RelacionElimina(Relacion * p_r);
Relacion * RelacionCopia(Relacion * p_r1);
Relacion * RelacionInserta(Relacion * p_r, int i, int j);
int  RelacionObtieneNumEstados(Relacion * p_r);
Relacion * RelacionCierreReflexivo(Relacion * p_r);
Relacion * RelacionCierreTransitivo(Relacion * p_r);
int  RelacionObtieneEstado(Relacion * p_r, int i, int j);
int RelacionCompara(Relacion * p_r1, Relacion * p_r2);
#endif
