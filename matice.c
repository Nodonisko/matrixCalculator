// this should fix undeclared getline in some compilers
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "matice.h"

int chyba;

typedef struct MatrixSize {
    int rows;
    int columns;
} MatrixSize;

struct MatrixSize determineMatrixSizeFromFile(const char *filePath) {
    FILE *fp;
    fp = fopen(filePath, "r");
    struct MatrixSize matrixSize = {.rows =  0, .columns =  0};

    if (fp == NULL) {
        chyba = CHYBA_OTEVRENI;
        return matrixSize;
    }
    
    char *line = NULL; // line array
    size_t len = 0;
    ssize_t read;
    int rowCounter = 0;
    int columnCounter = 0;

    while ((read = getline(&line, &len, fp)) != -1) {
        if(rowCounter == 0) {
            char * token = strtok(line, " ");
            while(token != NULL) {
                token = strtok(NULL, " ");
                columnCounter++;
            }
        }
        
        rowCounter++;//counts the number of lines in the file
     
    }
    
    if(fclose(fp) != 0) {
        chyba = CHYBA_ZAVRENI;
    }

    matrixSize.rows = rowCounter;
    matrixSize.columns = columnCounter;
    
    return matrixSize;
}

matice initMatrix(int rows, int columns) {
    matice matrix = {.m = rows, .n = columns};
    
    matrix.data = malloc(rows * sizeof(float *));
    
    if((int *)matrix.data == NULL) {
        chyba = CHYBA_ALOKACE;
        return matrix;
    }
    
    for(int i = 0; i < rows; i++) {
        matrix.data[i] = malloc(columns * sizeof(int));
        
        if((int *)matrix.data[i] == NULL) {
            chyba = CHYBA_ALOKACE;
            return matrix;
        }
    }
    
    return matrix;
}

void deleteMatrix(matice matrix) {
    for(int i = 0; i < matrix.m; i++) {
        free(matrix.data[i]);
    }
    free(matrix.data);
}

void setElement(matice matrix, int row, int column, float value) {
    if(row > matrix.m || column > matrix.n) {
        chyba = CHYBA_TYPU;
    } else {
        matrix.data[row][column] = value;
    }
}

float getElement(matice matrix, int row, int column) {
    if(row > matrix.m || column > matrix.n) {
        chyba = CHYBA_TYPU;
        return 0;
    }
    
    return matrix.data[row][column];
}

char lineString[255];
char* matrixLineToString(matice matrix, int rowNumber) {
    
    int index = 0;
    for (int columnNumber = 0; columnNumber < matrix.n; columnNumber++) {
        float element = getElement(matrix, rowNumber, columnNumber);
        if(columnNumber == (const int) matrix.n - 1) {
            // dont add space after last element, it will break parser next time
            index += sprintf(&lineString[index], "%f", element);
        } else {
            index += sprintf(&lineString[index], "%f ", element);
        }
        
    }
    
    return (char*)lineString;
}

void printMatrix(matice matrix) {
    for (int line = 0; line < matrix.m; line++) {
        printf("%s\n", matrixLineToString(matrix, line));
    }
}


matice createZeroMatrix(int rows, int columns) {
    matice matrix = initMatrix(rows, columns);
    
    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            setElement(matrix, row, column, 0);
        }
    }
    
    return matrix;
}

matice createIdentityMatrix(int m, int n) {
    // Identity matrix must be square
    if(m != n) {
        chyba = CHYBA_TYPU;
    }
    
    matice matrix = createZeroMatrix(n, n);
    
    for (int i = 0; i < n; i++) {
        setElement(matrix, i, i, 1);
    }
    
    return matrix;
}

matice openMatrixFromFile(const char *filePath) {
    // This function is not so smart, because it's opening file twice
    // because I need find out matrix size for dynamic allocation before filling it
    // from file content.
    MatrixSize matrixSize = determineMatrixSizeFromFile(filePath);
    matice matrix = initMatrix(matrixSize.rows, matrixSize.columns);
    
    FILE *fp;
    fp = fopen(filePath, "r");

    if (fp == NULL) {
        chyba = CHYBA_OTEVRENI;
        return matrix;
    }
    
    char *line = NULL; // line array
    size_t len = 0;
    ssize_t read;
    int rowCounter = 0;

    while ((read = getline(&line, &len, fp)) != -1) {
        int columnCounter = 0;
        char * token = strtok(line, " ");
        while(token != NULL) {
            setElement(matrix, rowCounter, columnCounter, atof(token));
            token = strtok(NULL, " ");
            columnCounter++;
        }
        
        rowCounter++;//counts the number of lines in the file
     
    }
    
    if(fclose(fp) != 0) {
        chyba = CHYBA_ZAVRENI;
        return matrix;
    }

    chyba = BEZ_CHYBY;
    
    return matrix;
}

void writeMatrixToFile(matice matrix, const char *filePath) {
    FILE *fp;
    fp = fopen(filePath, "w");

    if (fp == NULL) {
        chyba = CHYBA_OTEVRENI;
        return;
    }
    
    for (int row = 0; row < matrix.m; row++) {
        fprintf(fp, "%s\n", matrixLineToString(matrix, row));
    }
    
    if(fclose(fp) != 0) {
        chyba = CHYBA_ZAVRENI;
        return;
    }
    
}

#define SUM 1
#define SUBTRACT 2
matice addOrSubstractMatrices(matice matrix1, matice matrix2, int operation) {
    matice resultMatrix = initMatrix(matrix1.m, matrix1.n);
    
    if(matrix1.m != matrix2.m || matrix1.n != matrix2.n) {
        chyba = CHYBA_TYPU;
        return resultMatrix;
    }
    
    
    for (int row = 0; row < resultMatrix.m; row++) {
        for (int column = 0; column < resultMatrix.n; column++) {
            float result;
            float elementA = getElement(matrix1, row, column);
            float elementB =getElement(matrix2, row, column);
            if(operation == SUM) {
                result = elementA + elementB;
            } else {
                result = elementA - elementB;
            }
            setElement(resultMatrix, row, column, result);
        }
    }

    chyba = BEZ_CHYBY;
    
    return resultMatrix;
}

matice matrixScalarMultiplication(matice matrix, float scalar) {
    matice resultMatrix = initMatrix(matrix.m, matrix.n);
    
    for (int row = 0; row < resultMatrix.m; row++) {
        for (int column = 0; column < resultMatrix.n; column++) {
            float element = getElement(matrix, row, column);
            float result = element * scalar;
            setElement(resultMatrix, row, column, result);
        }
    }
    
    chyba = BEZ_CHYBY;
    
    return  resultMatrix;
}

matice matrixTransposition(matice matrix) {
    matice resultMatrix = initMatrix(matrix.n, matrix.m);
    
    for (int row = 0; row < resultMatrix.m; row++) {
        for (int column = 0; column < resultMatrix.n; column++) {
            float result = getElement(matrix, column, row);
            setElement(resultMatrix, row, column, result);
        }
    }
    
    chyba = BEZ_CHYBY;
    
    return  resultMatrix;
}

int matrixSize(matice matrix, int dimension) {
    if(dimension == 1) {
        return matrix.m;
    }
    
    return matrix.n;
}

matice matricesMultiplication(matice matrix1, matice matrix2) {
    matice resultMatrix = initMatrix(matrix1.m, matrix2.n);
    
    if(matrix1.n != matrix2.m) {
        chyba = CHYBA_TYPU;
        return resultMatrix;
    }
    
    for (int row = 0; row < resultMatrix.m; row++) {
        for (int column = 0; column < resultMatrix.n; column++) {
            float result = 0;
            for(int k = 0; k < resultMatrix.n; k++) {
                result += getElement(matrix1, row, k) * getElement(matrix2, k, column);
            }
            setElement(resultMatrix, row, column, result);
        }
    }
    
    chyba = BEZ_CHYBY;
    
    return  resultMatrix;
}

/* ---- START OF PUBLIC API bindings ------ */

matice inicializace(int m, int n) {
    return initMatrix(m, n);
}

matice nulova(int m, int n) {
    return createZeroMatrix(m, n);
}

matice jednotkova(int m, int n) {
    return createIdentityMatrix(m, n);
}

void odstran(matice mat) {
    deleteMatrix(mat);
}

void vypis(matice mat) {
    printMatrix(mat);
}

matice plus(matice mat1, matice mat2) {
    return addOrSubstractMatrices(mat1, mat2, SUM);
}

matice minus(matice mat1, matice mat2) {
    return addOrSubstractMatrices(mat1, mat2, SUBTRACT);
}

matice nasobeni(matice mat, float skalar) {
    return matrixScalarMultiplication(mat, skalar);
}

matice transpozice(matice mat) {
    return matrixTransposition(mat);
}

matice krat(matice mat1, matice mat2) {
    return matricesMultiplication(mat1, mat2);
}

matice nacti_ze_souboru(const char *soubor) {
    return openMatrixFromFile(soubor);
}

void uloz_do_souboru(matice mat, const char *soubor) {
    writeMatrixToFile(mat, soubor);
}

int velikost(matice mat, int dimenze) {
    return matrixSize(mat, dimenze);
}

float prvek(matice mat, int i, int j) {
    return getElement(mat, i, j);
}

void nastav_prvek(matice mat, int i, int j, float hodnota) {
    return setElement(mat, i, j, hodnota);
}

/* ---- END OF PUBLIC API -------- */
