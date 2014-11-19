/*
 * =====================================================================================
 *
 *       Filename:  p1.c
 *
 *    Description:  This is the snake program
 *
 *        Version:  1.0
 *        Created:  11/11/2012 22:24:06
 *       Revision:  none
 *       Compiler:  gcc
 *
 * =====================================================================================
 */

/* defines */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

/* our board size */

/* NOTE: I do this to generalize the problem... */
#define BOARD_DIM 10
#define BOARD_SIZE (BOARD_DIM*BOARD_DIM)

/* player pos [x,y] */

/* they require no helper tables... this
 * could be taken as one... you could change
 * this to take the form int x, int y and use
 * these two as a reference... but this is 
 * idiotic...and I won't do it. */
int playerPos[2] = { 0, 0 };
/* translated game position */
int gamePos = 0;
/* inverted numbering */
int invert = 0;

/* our board */
char board[BOARD_SIZE] = {

        '*','_','_','l','_','_','_','L','_','_',\
        '_','_','_','_','s','l','_','_','_','_',\
        '_','_','S','_','_','_','_','_','_','S',\
        'L','_','_','s','_','_','L','_','_','_',\
        '_','_','_','_','l','_','_','S','_','_',\
        '_','_','_','_','_','_','_','_','_','_',\
        '_','_','S','_','_','l','_','s','_','L',\
        '_','_','_','L','_','_','_','_','S','_',\
        '_','l','_','_','_','_','_','_','_','_',\
        '_','_','S','_','_','_','_','S','_','_',\

};

/* prints a nice board */
void
printBoard() {
    /* traversers */
    int travx = 0,
        travy = 0;

    /* hack for printing a nice board... */
    printf("\nPrinting Board...\n\n");
    for(travx = 0; travx < BOARD_DIM; travx++)
        {printf("   %d   ", travx+1);}
    printf("\n\n");
    for(travx = 0; travx < BOARD_DIM; travx++) {
        for(travy = 0; travy < BOARD_DIM; travy++) {
            printf(" | %c | ", board[travx*BOARD_DIM+travy]);
        }
        printf("   %d\n", travx+1); 
    }
}


/* function to roll a dice */
int
rollDice() 
    {return(rand()%6+1);}

/* function to calculate game position */
int
getPosValue(int *x, int *y) {
    /* check if we have inverse */
    if((*x%2) != 0)
        {invert = 1;}
    else
        {invert = 0;}

    /* calculate position */
    gamePos = ((*x))*BOARD_DIM + (invert ? BOARD_DIM - *y : (*y)+1);

    /* print our results */
    printf("\nPlayer Pos is (%d , %d)", playerPos[0], playerPos[1]);
    printf("\nTranslated Game Pos is: %d\n", gamePos); 
    
    return(0);
}

/* check for an action */
void
performAction(int *x, int *y) {
    /* check if any of the symbols are presnet */
    char s = board[(*x)*BOARD_DIM+(*y)];

    printf("\nWe got symbol: %c", s);
    switch(s) {
        /* small ladder */
        case 'l': {
            /* go 1 down, just advance x then */
            (*x)++;
            printf("\nPerforming small ladder, going 1 down.\n");
            /* update game position */
            getPosValue(x, y);
            break;
        }
        /* big ladder */
        case 'L': {
            /* go 2 down, just advance x then */
            (*x) = (*x) + 2;
            printf("\nPerforming large ladder, going 2 down.\n");
            /* update game position */
            getPosValue(x, y);
            break;
        }
        /* small snake */
        case 's': {
            /* go 1 up, just decrease x then */
            (*x)--;
            printf("\nPerforming small snake, going 1 up.\n");
            /* update game position */
            getPosValue(x, y);
            break;
        }
        /* big snake */
        case 'S': {
            /* go 2 up, just decrease x then */
            (*x) = (*x) - 2;
            printf("\nPerforming large snake, going 2 up.\n");
            /* update game position */
            getPosValue(x, y);
            break;
        }
        /* _ */
        default: {
            /* we do nothing... */
            printf("\nNo valid action found in current position...");
        }
    }

    /* finally print the snake on the baord */
    board[(*x)*BOARD_DIM+(*y)] = '*';
}


/* function that plays the move */
void
playMove() {

    int dice = 0,
        yposCache = 0;

    /* print the board initially */
    printBoard();

    /* let's roll the dice */
    dice = rollDice();
    printf("\nWe rolled: %d\n", dice);
    /* get position value */
    (void)getPosValue(&playerPos[0],&playerPos[1]);
    /* cache the value */
    yposCache = (invert ?  playerPos[1] - dice : playerPos[1] + dice);
    /* erase pos */
    board[playerPos[0]*BOARD_DIM+playerPos[1]] = '_';

    /* play our move */
    if(gamePos + dice >= BOARD_SIZE) {
        playerPos[1] = (yposCache*(-1));   
    }
    else if(yposCache < BOARD_DIM && yposCache > 0) {
        /* only, adjust y */
        playerPos[1] = yposCache;
    } else if(yposCache >= BOARD_DIM && playerPos[0] < BOARD_DIM-1) {
        /* advance x */
        playerPos[0]++;
        /* advance y */
        playerPos[1] = BOARD_DIM - (yposCache - BOARD_DIM) - 1;
    } else if (yposCache <= 0 && playerPos[0] < BOARD_DIM-1) {
        /* decrease x */
        playerPos[0]++;
        /* adjust y */
        playerPos[1] =  yposCache == 0 ? yposCache : (yposCache+1)*(-1);
    } 
    /* print new position */
    (void)getPosValue(&playerPos[0], &playerPos[1]);
    
    /* let's find out what action we have, if any */
    performAction(&playerPos[0], &playerPos[1]);

    /* print new board */
    printBoard();
}

/* just...main */
int
main ( int argc, char *argv[] )
{
    /* seed rand */
    srand(time(NULL));

    /* play the game! */
    while(gamePos != BOARD_SIZE) {
        playMove();
        /* this is a dirty hack... to pause! */
        getc(stdin);
    }

    return EXIT_SUCCESS;
}				
/* ----------  end of function main  ---------- */
