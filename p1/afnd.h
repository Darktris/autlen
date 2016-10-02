#ifndef _AFND_H
#define _AFND_H

#include <stdio.h>
#include "estado.h"
#include "ftrans.h"
#include "alfabeto.h"

#define _AFND_format    "%s={\n"\
                            "\tnum_simbolos = %d\n"\
                            "\tA = {%s}\n"\
                            "\tnum_estados = %d\n"\
                            "\tQ = {%s}\n"\
                            "\tFuncion de Transición = {\n"\
                                "%s"\
                            "\t}\n"\
                        "}"


struct AFND;
typedef struct AFND AFND;

AFND * AFNDNuevo(char * nombre, int num_estados, int num_simbolos);

void AFNDElimina(AFND * p_afnd);

void AFNDImprime(FILE * fd, AFND* p_afnd);

AFND * AFNDInsertaEstado(AFND * p_afnd, char * nombre, int tipo);

AFND * AFNDInsertaSimbolo(AFND * p_afnd, char * simbolo);

AFND * AFNDInsertaTransicion(AFND * p_afnd, char * nombre_estado_i, char * nombre_simbolo_entrada, char * nombre_estado_f );

AFND * AFNDInsertaLetra(AFND * p_afnd, char * letra);

void AFNDImprimeConjuntoEstadosActual(FILE * fd, AFND * p_afnd);

void AFNDImprimeCadenaActual(FILE *fd, AFND * p_afnd);

AFND * AFNDInicializaEstado (AFND * p_afnd);

void AFNDProcesaEntrada(FILE * fd, AFND * p_afnd);

void AFNDTransita(AFND * p_afnd);

#endif
