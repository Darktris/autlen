#ifndef _ALFABETO_H

#define _ALFABETO_H

typedef char* Letra;
typedef struct {
	Letra* letras;
	int num_simbolos;
} Alfabeto;

Alfabeto* AlfabetoNuevo(int num_simbolos);
Alfabeto* AlfabetoInserta(Alfabeto* sigma, Letra l);
void AlfabetoElimina(Alfabeto* sigma);
char* AlfabetoToString(Alfabeto* sigma);
Letra CopiaLetra(Letra l);
#endif
