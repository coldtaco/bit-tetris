#include <stdio.h>
#include <stdlib.h>

int* orientation(int rotation, int x, int piece);
char* toString(int s);
int getCol(short* board, short col);
char* IntToString(int s);
short** createArray(int m, int n);
void freeArray(short**);
short** setArray(short** donor, short** acceptor);