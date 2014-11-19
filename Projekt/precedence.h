/***********************precedence.h*****************************/
/* Soubor: precedence.h - Syntaktická analýza výrazů 			*/
/* Předmět: Formalní jazyky a překladače (IFJ) 					*/
/* Projekt: Implementace interpretu imperativního jazyka IFJ14  */
/* Varianta zadání: b/1/I 										*/
/* Datum: prosinec 2014											*/
/* Kódování: UTF-8												*/
/* Autoři:			Filip Kalous (xkalou03)						*/
/*					Matúš Bútora (xbutor01)						*/
/****************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include <stdbool.h>


typedef enum {

	pMORE,		// > redukce
	pLESS,		// < shift
	pEQUAL,		// = equal
	pEMPTY		// syntakticka chyba - prazdne misto v precedencni tabulce

} tPriority;

typedef enum {

	PLUS,
	MINUS,
	MUL,
	DIV,
	LEFT,
	RIGHT,
	LESS,
	MORE,
	LESSEQUAL,
	MOREEQUAL,
	EQUAL,
	NONEQUAL,
	ID,
	DOLAR

} tOperators;


typedef struct {

	tOperators element;

} tData;

typedef struct tElement {

	tData data;
	struct tElement *pointer;

} *tElement;

typedef struct {

	tElement top;

} tStack;


void stackInit();		// inicializace zasobniku
void stackFull();		// kontrola plneho zasobniku
bool stackEmpty(tStack *stack);		// kontrola prazdneho zasobniku
void stackTop();
void stackPop();
void stackPush();
void stackDispose();
