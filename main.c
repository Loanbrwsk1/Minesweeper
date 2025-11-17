#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LENGHT 20
#define ONE_MINE "\x1b[34;1m1\x1b[0m"
#define TWO_MINE "\x1b[32;1m2\x1b[0m"
#define THREE_MINE "\x1b[31;1m3\x1b[0m"
#define FOUR_MINE "\x1b[35;1m4\x1b[0m"
#define FIVE_MINE "\x1b[36;1m5\x1b[0m"
#define SIX_MINE "\x1b[33;1m6\x1b[0m"
#define SEVEN_MINE "\x1b[90;1m7\x1b[0m"
#define EIGHT_MINE "\x1b[91;1m8\x1b[0m"
#define MINE "\x1b[33;1mX\x1b[0m"

/*
? 1 : 1 mine around
? -1 : mine hidden around
? 0 : nothing
? 20 : broken block
? 50 : broken mine
? -50 : hidden mine 
*/

void printGame(int game[LENGHT][LENGHT])
{
    int i, j;

    printf("\x1b[1;1H\x1b[2J");
    fflush(stdout);

    printf("   ");
    for(j = 1 ; j <= LENGHT ; j++)
        j >= 10 ? printf(" %d ", j) : printf("  %d ", j);
    printf("\n");

    for(i = 0 ; i < LENGHT ; i++){
        printf("   ");

        for(j = 0 ; j < LENGHT ; j++)
            printf("+---");
        i + 1 >= 10 ? printf("+\n%d ", i + 1) : printf("+\n %d ", i + 1);

        for(j = 0 ; j < LENGHT ; j++){
            if(game[i][j] == 1)
                printf("| %s ", ONE_MINE);
            else if(game[i][j] == 2)
                printf("| %s ", TWO_MINE);
            else if(game[i][j] == 3)
                printf("| %s ", THREE_MINE);
            else if(game[i][j] == 4)
                printf("| %s ", FOUR_MINE);
            else if(game[i][j] == 5)
                printf("| %s ", FIVE_MINE);
            else if(game[i][j] == 6)
                printf("| %s ", SIX_MINE);
            else if(game[i][j] == 7)
                printf("| %s ", SEVEN_MINE);
            else if(game[i][j] == 8)
                printf("| %s ", EIGHT_MINE);
            else if(game[i][j] == 50)
                printf("| %s ", MINE);
            else if(game[i][j] == 20)
                printf("|   ");
            else if((game[i][j] >= 10 && game[i][j] <= 18) || game[i][j] == 30 || game[i][j] == 60)
                printf("| \x1b[31;1m■\x1b[0m ");
            else
                printf("| ■ ");
        }
        printf("|\n");
    }
    printf("   ");
    for(j = 0 ; j < LENGHT ; j++)
        printf("+---");
    printf("+\n");
}

int initMines(int game[LENGHT][LENGHT])
{
    int i, nb_mine = rand() % 25 + 25;

    for(i = 0 ; i < nb_mine ; i++){
        game[rand() % LENGHT][rand() % LENGHT] = -50;
    }

    return nb_mine;
}

void initGame(int game[LENGHT][LENGHT])
{
    int i, j, k, l, nb_mines = 0;

    for(k = 0 ; k < LENGHT ; k++){
        for(l = 0 ; l < LENGHT ; l++){
            if(game[k][l] != -50){
                nb_mines = 0;
                for(i = k - 1 ; i <= k + 1 ; i++){
                    for(j = l - 1 ; j <= l + 1 ; j++){
                        if(i >= 0 && i < LENGHT && j >= 0 && j < LENGHT){
                            if(game[i][j] == -50){
                                nb_mines++;
                            }
                        }
                    }
                }
            game[k][l] = -nb_mines;
            }
        }
    }
}

void cascade(int game[LENGHT][LENGHT], int row, int column)
{
    int i, j;

    if (game[row][column] == -50){
        return;
    }
    game[row][column] = 20;
    for(i = -1 ; i <= 1 ; i++){
        for(j = -1 ; j <= 1 ; j++){
            if((row + i >= 0 && row + i < LENGHT && column + j >= 0 && column + j < LENGHT) && (game[row + i][column + j] <= 0)){
                if(game[row + i][column + j] == 0){
                    cascade(game, row + i, column + j);
                }
                else{
                    game[row + i][column + j] *= -1;
                }
            }
        }
    }
}

int verifyWin(int game[LENGHT][LENGHT], int *nb_mines)
{
    int nb_flags = 0, nb_broken_block = 0, i, j;

    for(i = 0 ; i < LENGHT ; i++){
        for(j = 0 ; j < LENGHT ; j++){
            if(game[i][j] == 60){
                nb_flags++;
            }
            if(game[i][j] < 0 && game[i][j] != -50){
                nb_broken_block++;
            }
        }
    }
    if(nb_flags == *nb_mines || nb_broken_block == 0){
        printf("\x1b[32;1mYou win !\x1b[0m\n");
        return 1;
    }
    printf("\x1b[33;1mYou have not find all the mines !\x1b[0m\n");
    return 0;
}

void game(void)
{
    if(LENGHT < 10){
        printf("\x1b[31;1mThe size of the grid must be greather than or equal to 10 !\x1b[0m\n");
        return;
    }

    int game[LENGHT][LENGHT] = {{0}}, i, j, nb_flags_available, nb_mines, row, column, action, is_occuped, win;

    nb_flags_available = initMines(game);
    nb_mines = nb_flags_available;
    initGame(game);
    printGame(game);

    while (1){
        printf("Remaining flags : %d\n", nb_flags_available);
        printf("Break (1), Flag (2) or Verify (3) : ");
        scanf("%d", &action);
        if(action != 3){
            printf("Row : ");
            scanf("%d", &row);
            printf("Column : ");
            scanf("%d", &column);
            row--;
            column--;
        }
        is_occuped = 0;

        if(action != 1 && action != 2 && action != 3){
            is_occuped = 1;
            printGame(game);
            printf("\x1b[31;1mAction not recognized !\x1b[0m\n");
        }

        if(action == 3 && !is_occuped){
            if(verifyWin(game, &nb_mines) == 1){
                break;
            }
            else{
                is_occuped = 1;
            }
        }

        if((row < 0 || row > LENGHT - 1 || column < 0 || column > LENGHT - 1) && !is_occuped){
            is_occuped = 1;
            printGame(game);
            printf("\x1b[31;1mPlace not in the grid !\x1b[0m\n");
        }

        if(action == 2 && nb_flags_available == 0 && !is_occuped){
            is_occuped = 1;
            printGame(game);
            printf("\x1b[33;1mNo more flags available !\x1b[0m\n");
        }

        if(((game[row][column] >= 10 && game[row][column] <= 18) || game[row][column] == 30 || game[row][column] == 60) && !is_occuped && action == 1){
            is_occuped = 1;
            printGame(game);
            printf("\x1b[33;1mThis block is flaged !\x1b[0m\n");
        }

        if(game[row][column] == 20 && !is_occuped){
            is_occuped = 1;
            printGame(game);
            printf("\x1b[33;1mThis block is already broken !\x1b[0m\n");
        }

        if(!is_occuped){
            game[row][column] *= -1;
            if(game[row][column] == 0 && action == 1){
                cascade(game, row, column);
            }
            if(action == 2){
                if(game[row][column] <= 0){
                    game[row][column] += 10;
                    nb_flags_available++;
                }
                else{
                    game[row][column] += 10;
                    nb_flags_available--;
                }
            }
            printGame(game);
            if(game[row][column] == 50 && action == 1){
                for(i = 0 ; i < LENGHT ; i++){
                    for(j = 0 ; j < LENGHT ; j++){
                        if(game[i][j] == -50 || game[i][j] == 60){
                            game[i][j] *= -1;
                        }
                    }
                }
                printGame(game);
                printf("\x1b[31;1mGame over !\x1b[0m\n");
                break;
            }

            if(verifyWin(game, &nb_mines) == 1){
                break;
            }
        }
    }
}

int main(void)
{
    srand(time(NULL));
    game();
    return 0;
}