/*
 * =====================================================================================
 *
 *       Filename:  p2.c
 *
 *    Description: Turtle...Rabbit...Go! 
 *
 *        Version:  1.0
 *        Created:  12/03/10 23:53:34
 *       Revision:  none
 *       Compiler:  gcc
 *
 *	  License:  This software is Licensed under the terms and condtions of the BSD License model
 *
 * =====================================================================================
 */

#include	<stdlib.h>
#include 	<stdio.h>

#define MAX_DIST 70 /* by who? */


/* pads! */
char rabbitRun[MAX_DIST] = {0};
char turtleRun[MAX_DIST] = {0};

/* show-offs */
const char TURTLE = 'T';
const char RABBIT = 'H';
const char TIE 	  = 'X';

/* program-wide clock...I guess that's what they want...unless you want me to write
 	about real-time clocks from POSIX which I highly doupt */
unsigned int raceClock = 0;

/* forward declarations */

/* do race? */
int
doRace();

/* draw screen */
int
drawScreen(int *tPos, int *rPos);

/* advance turtle */
int
turtleAdvance(int *tPos);

/* advance rabbit */
int
rabbitAdvance(int *rPos);

/* 
 * ===  FUNCTION  ======================================================================
 *
 *         Name:  main
 *
 *  Description: ^_^ 
 *
 * =====================================================================================
 */
int
main ( int argc, char *argv[] )
{
	/* seed rand */
	(void)srand(0x12 << 0x4);	

	doRace();
	return EXIT_SUCCESS;
}				
/* ----------  end of function main  ---------- */

/* do race */
int
doRace() {
	/* positions */
	int tPos = 0,
		rPos = 0;
	/* start the clock, I guess */
	raceClock++;
	
	drawScreen(&tPos, &rPos);

	(void)fprintf(stdout, "\nPress any key to start the race!\n");
	(void)getchar();
	(void)fprintf(stdout, "\nBang and they are off!\n");

	while(raceClock &&  /* ensure our clock is working */
		(tPos < MAX_DIST && rPos < MAX_DIST)) { /* any1 won? */	
		/* advance turtle */
		turtleAdvance(&tPos);
		/* advance rabbit */
		rabbitAdvance(&rPos);
		fprintf(stdout, "\nRabbit Pos: %d", rPos);
		fprintf(stdout, "\nTurtle Pos: %d\n", tPos);

		(void)drawScreen(&tPos, &rPos);
		getchar(); raceClock++;
	}

	/* race results! */
	if(tPos == rPos)
		(void)fprintf(stdout, "\nRace Result: It's a Tie!\n");
	else if(tPos < rPos)
		(void)fprintf(stdout, "\nRace Result: Rabbit Won!\n");
	else
		(void)fprintf(stdout, "\nRace Result: Turtle Won!\n");

	/* done? */
	return(1);
}

/* advance turtle */
int
turtleAdvance(int *tPos) {
	/* get turtle chance of advancing */
	int rndRes = rand()%100;

	(void)fprintf(stdout, "\nTurtle Random Chance: %d\n", rndRes);

	/*  50 % chance, fast pace */
	if(rndRes <= 50) {
		(*tPos) += 3; if((*tPos) > MAX_DIST) (*tPos) = MAX_DIST;
	/* 20% chance, slip */
	}else if (rndRes > 50 && rndRes <= 70) {
	 	(*tPos) -= 6; if((*tPos) < 0) (*tPos) = 0;
	/* 30% chance, steady advance */
	}else {
		(*tPos)++; if((*tPos) > MAX_DIST) (*tPos) = MAX_DIST;
	}
	/* return it */
	return(*tPos);
}

/* advance rabbit */
int
rabbitAdvance(int *rPos) {
	/* get rabbit chance of advancing */
	int rndRes = rand()%100;

	(void)fprintf(stdout, "\nTurtle Random Chance: %d\n", rndRes);

	/* 20% chance, sleep */
	if(rndRes <= 20) {
		(*rPos) += 0;
	/* 20% chance, grand leap */
	}else if(rndRes > 20 && rndRes <= 40) {
		(*rPos) += 9; if((*rPos) > MAX_DIST) (*rPos) = MAX_DIST;
	/* 10% chance, grand slip */
	}else if(rndRes > 40 && rndRes <= 50) {
		(*rPos) -= 12; if((*rPos) < 0) (*rPos) = 0;
	/* 30% chance, small leap */
	}else if(rndRes > 50 && rndRes <= 80) {
		(*rPos) += 1; if((*rPos) > MAX_DIST) (*rPos) = MAX_DIST;
	/* 20% chance, small slip */
	}else {
		(*rPos) -= 2; if((*rPos) < 0) (*rPos) = 0;
	}
	
	/* return it */	
	return(*rPos);
}

/* draw the screen */
int
drawScreen(int *tPos, int *rPos) {
	int h = 0, v = 0;

	fprintf(stdout, "\n --> Race clock: %d\n\n", raceClock);
	for(h = 0; h < 3; h++) {
		for(v = 0; v <= MAX_DIST+2; v++) {
			if(v==0 || v == MAX_DIST+2) {
				if(h==0 || h == 2) {
					v == 0 ? (void)fprintf(stdout, "|") : (void)fprintf(stdout, "|");
				}else {
					v == 0 ? (void)fprintf(stdout, "|") : (void)fprintf(stdout, "|");
				}
			}else {
				if( ((v-1) != (*tPos) && (v-1) != (*rPos)) || h != 1 )
					(void)fprintf(stdout, "*");
				else if( (*tPos) == (*rPos) )
					(void)fprintf(stdout, "%c", TIE);
				else if ( (*tPos) == (v-1) )
					(void)fprintf(stdout, "%c", TURTLE);
				else
					(void)fprintf(stdout, "%c", RABBIT); 
			}
		}
		/* beautification */
		(void)fprintf(stdout, "\n");
	}
	/* we done! */
	return(1);
}
