#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matice.h"

#include "matice.h"

int main (){
    struct matice mat1, mat2, mat3, mat4, mat5, mat6, mat7, mat8;
    
    mat1 = jednotkova(3, 4);
    
    printf("Chyba %d", chyba);
    printf("Jednotkova: \n");
    vypis(mat1);
    printf("\n\n");
    
    mat2 = nulova(3, 4);
    printf("Nulova: \n");
    vypis(mat2);
    printf("\n\n");
    
    mat3 = nacti_ze_souboru("./matice1.txt");
    printf("Ze souboru 1: \n");
    vypis(mat3);
    printf("\n\n");
    
    mat5 = nasobeni(mat3, 2);
    printf("Nasobena scalar *2: \n");
    vypis(mat5);
    printf("\n\n");
    
    mat4 = nacti_ze_souboru("./matice2.txt");
    printf("Ze souboru 2: \n");
    vypis(mat4);
    printf("\n\n");
    
    mat6 = krat(mat3, mat4);
    printf("Krat: \n");
    vypis(mat6);
    printf("\n\n");
    
    mat7 = plus(mat3, mat4);
    printf("Plus: \n");
    vypis(mat7);
    printf("\n\n");
    
    mat8 = minus(mat6, mat3);
    printf("Minus: \n");
    vypis(mat8);
    printf("\n\n");
    
    uloz_do_souboru(mat8, "./matice3.txt");
    
    nastav_prvek(mat2, 2, 2, 99);
    printf("\n\n Prvek %f", prvek(mat2, 2, 2));
    printf("\n\n Radku %d", velikost(mat2, 1));
    printf("\n\n Sloupcu %d \n", velikost(mat2, 2));
    
    odstran(mat1);
    odstran(mat2);
    odstran(mat3);
    odstran(mat4);
    odstran(mat5);
    odstran(mat6);
    odstran(mat7);
    odstran(mat8);
}
