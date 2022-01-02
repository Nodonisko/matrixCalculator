#ifndef matice_h
#define matice_h

#include <stdio.h>

#define CHYBA_ALOKACE 2
#define CHYBA_OTEVRENI 3
#define CHYBA_ZAVRENI 5
#define CHYBA_TYPU 7
#define CHYBA_JINA 11
#define BEZ_CHYBY 42

extern int chyba;

typedef struct matice {
    int m; // number of rows
    int n; // number of columns
    float **data;
} matice;

matice inicializace(int m, int n);

matice nulova(int m, int n);

matice jednotkova(int m, int n);

void odstran( matice mat);

void vypis(matice mat);

matice plus(matice mat1, matice mat2);

matice minus(matice mat1, matice mat2);

matice nasobeni(matice mat, float skalar);

matice transpozice(matice mat);

matice krat(matice mat1, matice mat2);

matice nacti_ze_souboru(const char *soubor);

void uloz_do_souboru(matice mat, const char *soubor);

int velikost(matice mat, int dimenze);

float prvek(matice mat, int i, int j);

void nastav_prvek(matice mat, int i, int j, float hodnota);

#endif /* matice_h */
