
#include "helper.h"
struct point {
    int x;
    int y;
};


int main(){
    struct point* p = (struct point*)malloc(sizeof(struct point));
    (*p).x = 1;
    (*p).y = 99;
    //printf("%s", toString(1023));
    short* board = (short*)malloc(20);
    board[19] = 512;
    board[18] = 511;
    board[17] = 513;
    //printf("\n");
    //printf("%s", IntToString(getCol(board,0)));
    //printf("\n");
    //short* o = orientation(0,0,1);
    for (int i = 0; i < 8; i++){
        //printf("%d",o[i]);
    }
    //printf("%d",orientation(1,0,4)[0]);
    int d[] = {2, 1, 4, 4, 4, 2, 2};
    for (int i = 0; i < 7; i ++){
        for (int j = 0; j < d[i] ; j++){
            short* z = orientation(j,0,i);
            if (z[0] > 4){
                printf("BIG OOF %d\n", z[0]);
                exit(-1);
            }
            printf("piece = %d, orientation = %d\n",i,j);
            for (short b = 0; b < z[0]; b++){
                if (z[0] > 6){
                    printf("fuck me");
                    break;
                }
                //printf("piece %d rotations%d rot no. %d rows %d row num %d num = %d\n",i,d[i],j,z[0],b,z[b+2]);
                printf("%s\n",toString(z[b+2]));
            }
            printf("\n");
        }
    }
}