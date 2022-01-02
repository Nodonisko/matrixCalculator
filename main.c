#include <stdio.h>
#include <stdlib.h>
#include "matice.h"

int main()
{
    matice mat1, mat2, mat3, mat4, mat5, mat6;
    mat1 = jednotkova(3, 4);
    nastav_prvek(mat1, 0, 1, 2);
    nastav_prvek(mat1, 0, 2, 3);
    nastav_prvek(mat1, 0, 3, 4);
    nastav_prvek(mat1, 1, 0, 5);
    nastav_prvek(mat1, 2, 3, 1);
    printf("Matice mat1: \n");
    vypis(mat1);
    printf("\n\n");
    mat2 = transpozice(mat1);
    printf("Transpozice matice mat1: \n");
    vypis(mat2);
    printf("\n\n");
    mat3 = plus(mat1, mat1);
    printf("Soucet matic mat1 + mat1: \n");
    vypis(mat3);
    printf("\n\n");
    mat4 = nasobeni(mat1, 3);
    printf("Matice mat1 vynasobena 3: \n");
    vypis(mat4);
    printf("\n\n");
    mat5 = krat(mat1, mat2);
    printf("Matice mat1 vynasobena transponovanou matici mat1: \n");
    switch (chyba)
    {
    case CHYBA_TYPU:
        printf("S maticemi neni mozne provest nasobeni.");
        break;
    case CHYBA_ALOKACE:
        printf("Nastala chyba alokace pameti.");
        break;
    case BEZ_CHYBY:
        vypis(mat5);
        break;
        printf("\n\n");
        mat6 = krat(mat1, mat1);
        printf("Matice mat1 vynasobena matici mat1: \n");
        switch (chyba)
        {
        case CHYBA_TYPU:
            printf("S maticemi neni mozne provest nasobeni.");
            break;
        case CHYBA_ALOKACE:
            printf("Nastala chyba alokace pameti.");
            break;
        case BEZ_CHYBY:
            vypis(mat6);
            break;
        }
    }
    printf("\n\n");
    odstran(mat1);
    odstran(mat2);
    odstran(mat3);
    odstran(mat4);
    odstran(mat5);
    odstran(mat6);
    return 0;
}
