#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "matice.h"

int chyba;

struct MatrixSize {
    int rows;
    int columns;
};

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

struct matice initMatrix(int rows, int columns) {
    struct matice matrix = {.m = rows, .n = columns};
    
    matrix.data = malloc(rows * sizeof(float *));
    
    for(int i = 0; i < rows; i++) {
        matrix.data[i] = malloc(columns * sizeof(int));
    }
    
    return matrix;
}

void deleteMatrix(struct matice matrix) {
    for(int i = 0; i < matrix.m; i++) {
        free(matrix.data[i]);
    }
    free(matrix.data);
}

void setElement(struct matice matrix, int row, int column, float value) {
    matrix.data[row][column] = value;
}

float getElement(struct matice matrix, int row, int column) {
    return matrix.data[row][column];
}

char lineString[255];
char* matrixLineToString(struct matice matrix, int rowNumber) {
    
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

void printMatrix(struct matice matrix) {
    for (int line = 0; line < matrix.m; line++) {
        printf("%s\n", matrixLineToString(matrix, line));
    }
}


struct matice createZeroMatrix(int rows, int columns) {
    struct matice matrix = initMatrix(rows, columns);
    
    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            setElement(matrix, row, column, 0);
        }
    }
    
    return matrix;
}

struct matice createIdentityMatrix(int size) {
    struct matice matrix = createZeroMatrix(size, size);
    
    for (int i = 0; i < size; i++) {
        setElement(matrix, i, i, 1);
    }
    
    return matrix;
}

struct matice openMatrixFromFile(const char *filePath) {
    struct MatrixSize matrixSize = determineMatrixSizeFromFile(filePath);
    struct matice matrix = initMatrix(matrixSize.rows, matrixSize.columns);
    
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

void writeMatrixToFile(struct matice matrix, const char *filePath) {
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
struct matice addOrSubstractMatrices(struct matice matrix1, struct matice matrix2, int operation) {
    struct matice resultMatrix = initMatrix(matrix1.m, matrix1.n);
    
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

struct matice matrixScalarMultiplication(struct matice matrix, float scalar) {
    struct matice resultMatrix = initMatrix(matrix.m, matrix.n);
    
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

struct matice matrixTransposition(struct matice matrix) {
    struct matice resultMatrix = initMatrix(matrix.n, matrix.m);
    
    for (int row = 0; row < resultMatrix.m; row++) {
        for (int column = 0; column < resultMatrix.n; column++) {
            float result = getElement(matrix, column, row);
            setElement(resultMatrix, row, column, result);
        }
    }
    
    chyba = BEZ_CHYBY;
    
    return  resultMatrix;
}

int matrixSize(struct matice matrix, int dimension) {
    if(dimension == 1) {
        return matrix.m;
    }
    
    return matrix.n;
}

struct matice matricesMultiplication(struct matice matrix1, struct matice matrix2) {
    struct matice resultMatrix = initMatrix(matrix1.m, matrix2.n);
    
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

/* ---- START OF PUBLIC API ------ */

struct matice inicializace(int m, int n) {
    return initMatrix(m, n);
}

struct matice nulova(int m, int n) {
    return createZeroMatrix(m, n);
}

struct matice jednotkova(int m, int n) {
    if(m != n) {
        chyba = CHYBA_TYPU;
    }
    
    return createIdentityMatrix(m);
}

void odstran(struct matice mat) {
    deleteMatrix(mat);
}

void vypis(struct matice mat) {
    printMatrix(mat);
}

struct matice plus(struct matice mat1, struct matice mat2) {
    return addOrSubstractMatrices(mat1, mat2, SUM);
}

struct matice minus(struct matice mat1, struct matice mat2) {
    return addOrSubstractMatrices(mat1, mat2, SUBTRACT);
}

struct matice nasobeni(struct matice mat, float skalar) {
    return matrixScalarMultiplication(mat, skalar);
}

struct matice transpozice(struct matice mat) {
    return matrixTransposition(mat);
}

struct matice krat(struct matice mat1, struct matice mat2) {
    return matricesMultiplication(mat1, mat2);
}

struct matice nacti_ze_souboru(const char *soubor) {
    return openMatrixFromFile(soubor);
}

void uloz_do_souboru(struct matice mat, const char *soubor) {
    writeMatrixToFile(mat, soubor);
}

int velikost(struct matice mat, int dimenze) {
    return matrixSize(mat, dimenze);
}

float prvek(struct matice mat, int i, int j) {
    return getElement(mat, i, j);
}

void nastav_prvek(struct matice mat, int i, int j, float hodnota) {
    return setElement(mat, i, j, hodnota);
}

/* ---- END OF PUBLIC API -------- */
