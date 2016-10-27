#ifndef _ALFABETO_H

#define _ALFABETO_H

typedef char* Letra;
struct _Alfabeto;
typedef struct _Alfabeto Alfabeto;

#define LEN_ALFABETO 1024
Alfabeto* AlfabetoNuevo(int num_simbolos);
Alfabeto* AlfabetoInserta(Alfabeto* sigma, Letra l);
void AlfabetoElimina(Alfabeto* sigma);
char* AlfabetoToString(Alfabeto* sigma);
Letra CopiaLetra(Letra l);
void LiberaLetra(Letra l);
int ComparaLetra(Letra a, Letra b);
int LetraPerteneceAAlfabeto(Alfabeto* sigma, Letra l);
int AlfabetoObtieneNumSimbolos(Alfabeto* sigma);
Letra AlfabetoObtieneLetra(Alfabeto* sigma, int index);
#endif
