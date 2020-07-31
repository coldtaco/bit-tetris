#include "helper.h"

int main(int argc, char** argv){
    struct tetris *game = calloc(1,sizeof(tetris));
    initializeTetris(game);
    int move = 0;
    long cleared;
    short dheight = 0;
    double svector[7];
    if (argc == 8){
        for (int i = 0; i < 7; i++){
            svector[i] = strtod(argv[i + 1], NULL);
        }
    }
    while (!game->board[0]){
        cleared = 0;
        dheight = 0;
        int result = scanf("%d", &move);
        //fprintf(stderr,"Got %d", move);
        if (result != 1){
            return -1;
        }
        game -> board = hardDrop(game -> board, game -> bag[game -> bagind],move / 4, move % 4, &dheight);
        cleared = clear(game -> board);
        game->bagind++;
        if (game->bagind >= 14){
            game->bagind = 0;
            game->bag = newBag();
        }
        printBoard(game->board);
        if (argc == 8){
            printf("%f\n", getScore(game, dheight, game -> board));
        } else {
            printf("%ld\n", cleared*100 + dheight);
        }        
    }
    freeGame(game);
    return 0;
}