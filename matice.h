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

struct matice {
    int m; // number of rows
    int n; // number of columns
    float **data;
};

struct matice inicializace(int m, int n);

struct matice nulova(int m, int n);

struct matice jednotkova(int m, int n);

void odstran(struct matice mat);

void vypis(struct matice mat);

struct matice plus(struct matice mat1, struct matice mat2);

struct matice minus(struct matice mat1, struct matice mat2);

struct matice nasobeni(struct matice mat, float skalar);

struct matice transpozice(struct matice mat);

struct matice krat(struct matice mat1, struct matice mat2);

struct matice nacti_ze_souboru(const char *soubor);

void uloz_do_souboru(struct matice mat, const char *soubor);

int velikost(struct matice mat, int dimenze);

float prvek(struct matice mat, int i, int j);

void nastav_prvek(struct matice mat, int i, int j, float hodnota);

#endif /* matice_h */
