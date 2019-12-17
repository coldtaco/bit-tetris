#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "helper.h"

short** createArray(int m, int n)
{
    //m is rows, n is cols
    short* values = calloc(m*n, sizeof(short));
    short** rows = malloc(m*sizeof(short*));
    for (int i=0; i<m; ++i)
    {
        rows[i] = values + i*n;
    }
    return rows;
}

void freeArray(short** arr){
    free(*arr);
    free(arr);
}

short** setArray(short** donor, short** acceptor){
    short rows = donor[0][0];
    short cols = donor[0][1];
    for (int y = 0; y < rows; y++){
        for (int x = 0; x < cols; x++){
            acceptor[y][x] = donor[y][x];
        }
    }
    return acceptor;
}

int* orientation(int rotation, int x, int piece){
    //{0 : "I", 1 : "O", 2 : "T", 3 : "J", 4 : "L", 5 : "S", 6 : "Z"}
   int* coords = malloc(100);
    switch(piece){
        case 0:
            switch(rotation){
                case 0:
                    coords = (int[]) {4,1,0x1,0x1,0x1,0x1};
                    break;
                case 1:
                    coords = (int[]) {1,4,0xf};
                    break;
            }
            break;
        case 1:
            coords = (int[]) {2,2,0x5,0x5};
            break;
        case 2:
            switch(rotation){
                case 0:
                    coords = (int[]) {2,3,0x7,0x2};
                    break;
                case 1:
                    coords = (int[]) {3,2,0x1,0x3,0x1};
                    break;
                case 2:
                    coords = (int[]) {2,3,0x2,0x7};
                    break;
                case 3:
                    coords = (int[]) {3,2,0x2,0x3,0x2};
                    break;
            }
            break;
        case 3:
            switch(rotation){
                case 0:
                    coords = (int[]) {3,2,0x1,0x1,0x3};
                    break;
                case 1:
                    coords = (int[]) {2,3,0x4,0x7};
                    break;
                case 2:
                    coords = (int[]) {3,2,0x3,0x2,0x2};
                    break;
                case 3:
                    coords = (int[]) {2,3,0x7,0x1};
                    break;
            }
            break;
        case 4:
            switch(rotation){
                case 0:
                    coords = (int[]) {3,2,0x2,0x2,0x3};
                    break;
                case 1:
                    coords = (int[]) {2,3,0x7,0x4};
                    break;
                case 2:
                    coords = (int[]) {3,2,0x3,0x1,0x1};
                    break;
                case 3:
                    coords = (int[]) {2,3,0x1,0x7};
                    break;
            }
            break;
        case 5:
            switch(rotation){
                case 0:
                    coords = (int[]) {2,3,0x3,0x6};
                    break;
                case 1:
                    coords = (int[]) {3,2,0x2,0x3,0x1};
                    break;
            }
        case 6:
            switch(rotation){
                case 0:
                    coords = (int[]) {2,3,0x6,0x3};
                    break;
                case 1:
                    coords = (int[]) {3,2,0x1,0x3,0x2};
                    break;
            }
    }
    return coords;
}

char* toString(int s){
    char* string = malloc(11*sizeof(char));
    int index = 512;//2^9
    for (int i = 0; i < 10; i++){
        //printf("%u",index & s ? 1 : 0);
        string[i] = index & s ? '1' : '0';
        index = index >> 1;
    }
    string[10] = '\0';
    return string;
}

char* IntToString(int s){
    char* string = malloc(21*sizeof(char));
    int index = 524288;//2^19
    
    for (int i = 0; i < 20; i++){
        //printf("%u",index & s ? 1 : 0);
        string[i] = index & s ? '1' : '0';
        index = index >> 1;
    }
    string[20] = '\0';
    return string;
}

int getCol(short* board, short col){
    short index = 512;
    int result = 0;
    index = index >> col;
    for (int row = 0; row < 20; row++){
        if (board[row] & index){
            result += pow(2,19-row);
        }
    }
    return result;
}