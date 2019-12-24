#include <stdio.h>
#include <stdlib.h>

short* orientation(short rotation, short piece);
char* toString(short s);
int getCol(short* board, short col);
char* IntToString(int s);
short** createArray(int m, int n);
void freeArray(short**);
short** setArray(short** donor, short** acceptor);
short* hardDrop(short* board, short piece, short rotation, short col);
void printBoard(short* board);
void shuffle(int *array, size_t n);
short* newBag();