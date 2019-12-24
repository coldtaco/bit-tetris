#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "helper.h"
#include <string.h>

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
    printf("piece = %d, orientation = %d\n",piece,rotation);
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
        printf("%s\n",toString(board[i]));
    }
}

short* hardDrop(short* board, short piece, short rotation, short col){
    int d[] = {2, 1, 4, 4, 4, 2, 2};
    short* or = orientation((rotation-1+d[piece])%d[piece],piece);
    short height = or[1];
    short width = or[0];
    int coords[height];
    short droppedHeight = 0;
    short quit = 1;
    int cols[or[1]];
    for (short x = 0; x < or[0]; x++){
        cols[x] = getCol(board,col+x);
    }
    for (short i = 0; i < width; i++){
        coords[i] = or[i+2]<< 19 - height;
    }
    printf("h = %d w = %d\n",height,width);
    for (; droppedHeight < 20 - height && quit; droppedHeight++){
        for (short i = 0; i < width; i ++){
            printf("coords = %d, cols = %d\n",coords[i]>>droppedHeight,cols[i]);
            if ((coords[i] >> droppedHeight) & cols[i]){
                quit = 0;
                droppedHeight --;
                printf(" early exit dropped height = %d\n",droppedHeight);
                break;
            }
        }
    }
    printf("dropped height = %d\n",droppedHeight);
    or = orientation(rotation,piece);
    height = or[0];
    width = or[1];
    for (short i = 0; i < height; i++){
        coords[i] = or[i+2] << 10 - col - width;
    }
    for (short i = 0; i < height; i++){
        printf("i=%d\n",i);
        if (board[droppedHeight + i] | coords[i]){
            printf("board = %d piece = %d\n",board[droppedHeight],coords[i]);
            //exit(-1);
        }
        board[droppedHeight + i] += coords[i]; 
    }
    return board;
}

void shuffle(int *array, size_t n)
{
    if (n > 1) 
    {
        size_t i;
        for (i = 0; i < n - 1; i++) 
        {
          size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          int t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}

short* newBag(){
    short bag[14] = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6};
    shuffle(bag, 14);
    return bag;
}

short clear(short* board){
    short* nb = calloc(20, sizeof(short));
    short cleared = 0;
    for (short i = 19; i >= 0; i++){
        if (board[i] == 1023){
            cleared ++;
        } else {
            nb[i + cleared] = board[i];
        }
    }
    return cleared;
}