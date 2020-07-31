#include <stdio.h>
#include <stdlib.h>

typedef struct tetris {
    short* board;
    short* bag;
    short bagind;
    long cleared;
    double* sVector;
    
}tetris;
short* orientation(short rotation, short piece);
char* toString(short s);
int getCol(short* board, short col);
char* IntToString(int s);
short** createArray(int m, int n);
void freeArray(short**);
short** setArray(short** donor, short** acceptor);
short* hardDrop(short* board, short piece, short rotation, short col, short* dHeight);
void printBoard(short* board);
void shuffle(short *array, size_t n);
short* newBag();
short clear(short* board);
short getCleared(short* board);
double getScore(struct tetris* game, short dHeight,short*board);
short* copyBoard(short* arr, short len);
void bestMove(struct tetris *game);
void initializeTetris(struct tetris *game);
void freeGame(struct tetris *game);
short holes(short* board);
short wells(short* board);
short colTransitions(short* board);
short rowTransitions(short* board);
float distance(short* board);