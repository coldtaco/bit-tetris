#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "helper.h"
#include <string.h>
#include <limits.h>


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

short* orientation(short rotation, short piece){
    //printf("piece = %d, orientation = %d\n",piece,rotation);
    //{0 : "I", 1 : "O", 2 : "T", 3 : "J", 4 : "L", 5 : "S", 6 : "Z"}
    static short coords[6] = {0,0,0,0,0,0};
    switch(piece){
        case 0:
            switch(rotation){
                case 0:
                    memcpy(coords, (short[]){4,1,0x1,0x1,0x1,0x1}, sizeof(coords));
                    break;
                case 1:
                    memcpy(coords, (short[]){1,4,0xf}, sizeof(coords));
                    break;
            }
            break;
        case 1:
            memcpy(coords, (short[]){2,2,0x3,0x3}, sizeof(coords));
            break;
        case 2:
            switch(rotation){
                case 0:
                    memcpy(coords, (short[]){2,3,0x7,0x2}, sizeof(coords));
                    break;
                case 1:
                    memcpy(coords, (short[]){3,2,0x1,0x3,0x1}, sizeof(coords));
                    break;
                case 2:
                    memcpy(coords, (short[]){2,3,0x2,0x7}, sizeof(coords));
                    break;
                case 3:
                    memcpy(coords, (short[]){3,2,0x2,0x3,0x2}, sizeof(coords));
                    break;
            }
            break;
        case 3:
            switch(rotation){
                case 0:
                    memcpy(coords, (short[]){3,2,0x1,0x1,0x3}, sizeof(coords));
                    break;
                case 1:
                    memcpy(coords, (short[]){2,3,0x4,0x7}, sizeof(coords));
                    break;
                case 2:
                    memcpy(coords, (short[]){3,2,0x3,0x2,0x2}, sizeof(coords));
                    break;
                case 3:
                    memcpy(coords, (short[]){2,3,0x7,0x1}, sizeof(coords));
                    break;
            }
            break;
        case 4:
            switch(rotation){
                case 0:
                    memcpy(coords, (short[]){3,2,0x2,0x2,0x3}, sizeof(coords));
                    break;
                case 1:
                    memcpy(coords, (short[]){2,3,0x7,0x4}, sizeof(coords));
                    break;
                case 2:
                    memcpy(coords, (short[]){3,2,0x3,0x1,0x1}, sizeof(coords));
                    break;
                case 3:
                    memcpy(coords, (short[]){2,3,0x1,0x7}, sizeof(coords));
                    break;
            }
            break;
        case 5:
            switch(rotation){
                case 0:
                    memcpy(coords, (short[]){2,3,0x3,0x6}, sizeof(coords));
                    break;
                case 1:
                    memcpy(coords, (short[]){3,2,0x2,0x3,0x1}, sizeof(coords));
                    break;
            }
            break;
        case 6:
            switch(rotation){
                case 0:
                    memcpy(coords, (short[]){2,3,0x6,0x3}, sizeof(coords));
                    break;
                case 1:
                    memcpy(coords, (short[]){3,2,0x1,0x3,0x2}, sizeof(coords));
                    break;
            }
    }
    return coords;
}

char* toString(short s){
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
    
    for (int i = 1; i < 21; i++){
        //printf("%u",index & s ? 1 : 0);
        string[i] = index & s ? '1' : '0';
        index = index >> 1;
    }
    string[20] = '\0';
    return string;
}
/**
 * returns column where lowest row is least significant bit
 */
int getCol(short* board, short col){
    short index = 512;
    int result = 0;
    index = index >> col;
    for (int row = 0; row < 20; row++){
        if (board[row+1] & index){
            result += pow(2,19-row);
        }
    }
    return result;
}

void printBoard(short* board){
    //printf("printboard");
    /*
    char* string = malloc(20*11*sizeof(char)+1);
    for (short i = 0; i < 20; i++){
        char* str = toString(board[i]);
        for (short j = 0; j < 10; j++){
            string[j+i*11] = str[j];
            printf("%d\n",j);
        }
        string[11*i+1] = '\n';
        //printf("%d\n",11*i);
    }
    string[221] = '\0';
    return string;*/
    for (short i = 0; i < 20; i++){
        char* str = toString(board[i + 1]);
        printf("%s %d\n",str, i);
        free(str);
    }
    printf("\n");
}

short* hardDrop(short* board, short piece, short rotation, short col, short* dHeight){
    //printf("piece = %d, rotation = %d, col = %d\n",piece, rotation, col);
    //int d[] = {2, 1, 4, 4, 4, 2, 2};
    short* or = orientation(rotation,piece);
    short height = or[0];
    short width = or[1];
    int coords[4];
    short droppedHeight = 0;
    short quit = 2;
    //int cols[or[1]];
    for (short i = 0; i < height; i++){
        coords[i] = or[i+2]<< (9 - col - width + 1);
        //printf("coords = %d, %d, %d\n",coords[width - 1 - i],or[i+2], width - 1 - i);
    }
    for (;droppedHeight < 21 - height && quit;) {
        droppedHeight ++;
    // Check if piece collides with the cells of the current row.
        for (short i = 0; i < height; i++) {
            if ((board[droppedHeight + i] & coords[i])) {
            // Found collision - place piece on row above.
            quit = 0;
            droppedHeight --;
            //printf("DROPPEDHEIGHT = %d\n",droppedHeight);
            break;
            }
        }
    }
    //printf("droppedheight = %d\n",droppedHeight);
    //printf("dropped height = %d, height = %d, piece = %d\n", droppedHeight,height,piece);
    for (short i = 0; i < height; i++){
        if (droppedHeight + i > 21){
            printf("217  %d, %d, %d, %d\n",droppedHeight + height -1,height,droppedHeight,droppedHeight + i);
            exit(-1);
        }
        if (board[droppedHeight + i] & coords[i]){
            //printf("fuck la board = %d piece = %d dropped height = %d\n",board[droppedHeight+i],coords[i],droppedHeight + i);
            //printf("piece = %d, rotation = %d, col = %d\n", piece, rotation, col);
            //printBoard(board);
            board[0] = 1;
            //exit(-1);
        }
        //printf("226  %d, %d, %d, %d\n",board[droppedHeight + i],coords[i],board[droppedHeight + i] + coords[i],droppedHeight + i);
        //printBoard(board);
        board[droppedHeight + i] |= coords[i]; 
    }
    *dHeight = droppedHeight + height - 1;
    //printBoard(board);
    //printf("finsihed\n");
    return board;
}

void shuffle(short *array, size_t n){
    if (n > 1) 
    {
        size_t i;
        for (i = 0; i < n - 1; i++) 
        {
          size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          short t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}

short* newBag(){
    short* b = calloc(14, sizeof(short));
    short bag[14] = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6};
    shuffle(bag, 14);
    memcpy(b,bag,sizeof(bag));
    return b;
}

short clear(short* board){
    short cleared = 0;
    for (short i = 20; i >= 1; i--){
        if (board[i] == 1023){
            cleared ++;
            //printf("cleared at %d",i);
        } else {
            board[i + cleared] = board[i];
        }
    }
    for (short i = 0; i < cleared; i++){
        board[i + 1] = 0;
    }
    return cleared;
}

short getCleared(short* board){
    if (board ==NULL){
        //printf("clear");
        exit(-1);
    }
    short cleared = 0;
    for (short i = 20; i >= 1; i--){
        if (board[i] == 1023){
            cleared ++;
        }
    }
    return cleared;
}

float getScore(struct tetris game, short* dHeight,short*board){ 
    if (board[0]){
        return -9999;
    }
    float score = 0;
    score += game.sVector[0] * getCleared(board);
    score += game.sVector[1] * wells(board);
    //printf("wells = %f",game.sVector[1] * wells(board));
    score += game.sVector[2] * holes(board);
    score += game.sVector[3] * distance(board);
    score += game.sVector[4] * (20 - dHeight[0]);
    score += game.sVector[5] * rowTransitions(board);
    score += game.sVector[6] * colTransitions(board);
    //printf("got score of %f\n",score);
    //printf("cleared = %d, wells = %d, holes = %d, distance = %f, height =  %d\n",getCleared(game.board), wells(game.board),holes(game.board),distance(game.board),20 - dHeight[0]);
    return score;
}

short* copyBoard(short* arr, short len){
    short* arr2 = calloc(len,sizeof(short));
    for (int x = len -1; x >= 0; x--){
        arr2[x] = arr[x];
    }
    return arr2;
}

void bestMove(struct tetris *game){
    if (game->board ==NULL){
        //printf("bestnmove");
        exit(-1);
    }
    int tested = 0;
    int d[] = {2, 1, 4, 4, 4, 2, 2};
    short piece = game->bag[game->bagind];
    float bestScore = INT_MIN;
    short* bestBoard;
    for (short rotation = 0; rotation < d[piece]; rotation++){
        short* or = orientation(rotation, piece);
        for ( short x = 0; x <= 10 - or[1]; x++ ){
            tested ++;
            //short* nb = hardDrop(game.board,piece,rotation,x);
            short* dHeight = calloc(1, sizeof(short));
            short* nb = hardDrop(copyBoard(game->board,21),piece,rotation,x, dHeight);
            float score = getScore(*game, dHeight,nb);
            free(dHeight);
            if (score > bestScore){
                //printf("set best");
                bestScore = score;
                bestBoard = nb;
            } else {
                free(nb);
            }
        }
    }
    //printf("tested %d times piece = %d\n", tested,piece);
    free(game->board);
    game->board = bestBoard;
    //printf("bestscore = %f\n",bestScore);
    //printBoard(bestBoard);
    game->cleared += clear(game->board);
}

void initializeTetris(struct tetris *game){
    short* _ = calloc(21,sizeof(short));
    game->board = _;
    game->bag = newBag();
    game->bagind = 0;
    game->cleared = 0;
    game->sVector = calloc(1,sizeof(float));
    if (game->board ==NULL){
        printf("inittetris");
        exit(-1);
    }
}

void freeGame(struct tetris *game){
    free(game->board);
    free(game->bag);
    free(game->sVector);
    free(game);
}

short holes(short* board){
    short totalHoles = 0;
    short prevRow = board[1];
    short holes = 0;
    for (short i = 2; i < 21; i++){
        prevRow = board[i-1];
        holes = ~board[i] & (holes | prevRow);
        for (short j = 0; j < 10; j++){
            if (1 << j & holes){
                totalHoles++;
            }
        }
    }
    return totalHoles;
}

short wells(short* board){
    int cols[10];
    for (short i = 0; i < 10; i ++){
        cols[i] = getCol(board,i);
    }
    int occupiedTrips[10] = {0};
    int unoccupiedTrips[10] = {0};
    short wells = 0;
    for (short i = 0; i < 10; i ++){
        int currCol = cols[i];
        for (short j = 1; j < 20-3; j++){
            if ((currCol & 7 << j) == 7 <<j){
                occupiedTrips[i] += (1 << j);
            } else if ((~currCol &  7 <<  j) == 7 << j ){
                unoccupiedTrips[i] += (1 << j);
            }
        }
    }
    for (short i = 0; i < 17;i++){
        if ((occupiedTrips[1] & unoccupiedTrips[0]) & 1 << i){
            //printf("well at 0");
            wells += 1;
        }
    }
    for (short i = 0; i < 17;i++){
        if ((occupiedTrips[8] & unoccupiedTrips[9]) & 1 << i){
            //printf("well at 9");
            wells += 1;
        }
    }
    for (short j = 1; j < 8; j++){
        int compare = occupiedTrips[j-1] & unoccupiedTrips[j]& occupiedTrips[j+1];
        for (short i = 0; i < 17;i++){
            if (compare & 1 << i){
                //printf("well at %d",j);
                wells += 1;
            }
        }
    }
    return wells;
}

float distance(short* board){
    short lowest[10] = {19,19,19,19,19,19,19,19,19,19};
    float distance = 0;
    for (short x = 0; x < 10; x++){
        int col = getCol(board,x);
        for (short i = 0; i < 20; i++){
            if (col & 1 << (20-i)){
                lowest[x] = 20 - i;
                break;
            }
        }
        //printf("%d ", lowest[x]);
    }
    //printf("\n");
    for (short x = 2; x < 8; x++){
        short curr = lowest[x];
        distance += (abs(curr - lowest[x-2]) + abs(curr - lowest[x-1]) + abs(curr - lowest[x+1]) + abs(curr - lowest[x+2]))/4;
        //printf("distance = %d,%d,%d,%d ",abs(curr - lowest[x-2]) , abs(curr - lowest[x-1]) , abs(curr - lowest[x+1]) , abs(curr - lowest[x+2]));
    }
    //printf("\n");
    return distance;
}

short rowTransitions(short* board){
    short transitions = 0;
    for (short i = 1; i < 21; i++){
        short curr = ~board[i] & (board[i] >> 1);
        for (short j = 0;j < 10; j++){
            if (curr & (1 << j)){
                transitions++;
            }
        }
    }
    return transitions;
}

short colTransitions(short* board){
    short transitions = 0;
    for (short i = 0; i < 10; i++){
        int currCol = getCol(board,i);
        int curr = ~currCol & (currCol >> 1);
        for (short j = 0;j < 20; j++){
            if (curr & (1 << j)){
                transitions++;
            }
        }
    }
    return transitions;
}