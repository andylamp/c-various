/*
 * =====================================================================================
 *
 *       Filename:  bscan.c
 *
 *    Description:  This file contains the implementation of the body scanner app.
 *    							that was asked to be implemented for C class second part.
 *
 *                  I am really bored to give detailed explanations for all the programs
 *                  that I write but since this is quite the quirky one and is the only
 *                  of the two that has some 'algorithmic' value I will give a detailed
 *                  explanation. Expect nothing of the sort in the other one though.
 *                  Anywhore... let's get on with it shall we? Oh and please note this
 *                  is not a C tutorial... I won't illustrate any of the language features 
 *                  used in this program only the logic behind it.
 *
 *                  We are requested to create a "body scanner" that scans the body in
 *                  vertical/horizontal beams as well as angled versions of the two. We
 *                  are given the mumber of the beams in illustration these are N: 10 
 *                  and M:15; but this can be of any number it's not really important.
 *                  First of all we have to devise a program that will solve the problem
 *                  fulfilling those three criteria:
 *
 *                   1) it has to be correct
 *                   2) it has to be neat and robust
 *                   3) and it has to be FAST (don't abuse the term though!)
 *
 *                   So in order to do that we have to use as little space as possible
 *                   and as little physical operations as possible. There are little quirks
 *                   that can produce faster code but this varies from system to system and
 *                   from compiler to compiler; for example the using the icc (Intel C 
 *                   Compiler) on a Intel based cpu is bound to give better code if you use
 *                   the same compiler with an AMD chip.
 *
 *                   Compiler (and C) use some neat tricks that I won't describe now but you 
 *                   can use them in your code! (If you trust me enough!).
 *
 *                   1) for (;;) {;} almost always produces faster code than do {;}while as 
 *                      well as while {;}
 *
 *                   2) if you have single value check use a switch statement instead of 
 *                       if {} else if {] else {}
 *
 *                   3) if you have a table N-D table map it to 1-D table, it makes compiler
 *                      index maps faster... (Hint: you convert a n-d table to 1-d table by using
 *                      the following formula: tab[x][y][z] -> t[0][0][1] --> tab[x*0+y*0+1]
 *                      I know it's quirky at start but if you get used to it... it comes natural
 *
 *                   The list goes on and on but that's for a start...
 *
 *                   Now to the program itself since we have the beams we have use a single char 
 *                   table to describe our three block states... say we have a 3x4 table for this
 *                   example (this is scalable for any NxM table even when N=M):
 *
 *                   Tab:
 *
 *                      0 1 2 3
 *
 *                   0  U U U U
 *                   1  U U U U
 *                   2  U U U U  
 *
 *                  This is our initial table in it's untagged state...
 *
 *                  Now let's be clever about this... truth to be told must always be
 *                  so say we scan for each of then we have enough information to at 
 *                  'least' surround the dense/empty area by a big margin... that's
 *                  because of the zeros that are in place! So if we take the table
 *                  and put zeros in the places we have zeros we have the following:
 *                  
 *                  Table:
 *
 *                      0 1 2 3
 *                  0   . # . #
 *                  1   # # # .
 *                  2   . # . .
 *                  
 *                  Sensor Inputs:
 *
 *                  Sensor 1: 2 3 1
 *                  Sensor 2: 0 2 1 2 0 0
 *                  Sensor 3: 1 3 1 1
 *                  Sensor 4: 0 2 1 2 0 1
 *
 *
 *                  Now let's say we have the following input:
 *                  
 *                    0 1 2 3    Filter Zeros     0 1 2 3
 *                  0 U U U U                   0 . U . U
 *                  1 U U U U   ------------->  1 U U U .
 *                  2 U U U U                   2 . U . .                
 *                   
 *                  So in one iteration we found almost all of th needed space
 *                  and we surrounded the dense area a lot but no always fully 
 *                  this is not clear here due to the fact that we have a very
 *                  small matrix to play with and a lot of sensors for that matrix.
 *                  If you scale it up for bigger N's and M's you will have a lot 
 *                  of missing info!
 *
 *                  Now if we apply the following method for each line we scan
 *                  we get our algorithm!
 *
 *                  Scan the line and populate the untagged, tagged (dense) and
 *                  empty cells; so we have a picture of the line...now check if 
 *                  these are true:
 *
 *                  if the untagged cells are equal to the dense cells set every
 *                  untagged cell to be dense
 *
 *                  if the dense cells cells are equal to the dense readings in the
 *                  file set the untagged cells to be empty!
 *
 *                  repeat these steps... and you have our algorithm! Although 
 *                  it looks like it has many it steps, it really doesn't as they
 *                  are mostly done when there is a case for them... usually you need
 *                  2-3 full sensor table checks to populate a 20x20~25x25 table; which
 *                  considering the cases we have are not that many... and don't forget
 *                  that memset operations are blazing fast :).
 * 
 *       Version:  1.0
 *       Created:  01/29/2012 08:56:03 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 * =====================================================================================
 */


/* Our header includes */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>

/* our defines */
#define RET_OK 0
#define ERR_BAD_INPUT 2
#define ALLOC_ERR 3

/* here are our debug flags, uncomment 
 * them to enable them!
 *
 * WARNING! LVL_2 flags produce insane amount
 * of info... be prepared! */

//#define DEBUG_LVL_1 // first level, just info
//#define DEBUG_LVL_2 // more detail on the application run
//#define DEBUG_FIRST_SENSOR // first sensor print on end
//#define DEBUG_FIRST_SENSOR_LVL_2 // more info on each interation change on S1
//#define DEBUG_SECOND_SENSOR // second sensor print start/end
//#define DEBUG_SECOND_SENSOR_LVL_2 // more info on each iteration change on S2
//#define DEBUG_THIRD_SENSOR //  third sensor print on en
//#define DEBUG_THIRD_SENSOR_LVL_2 // more info on each iteration change on S3
//#define DEBUG_FOURTH_SENSOR // second sensor print start/end
//#define DEBUG_FOURTH_SENSOR_LVL_2 // more info on each iteration change on S4

/* our table N*M elements */
char *tab = NULL;

/* sensor readings */ 
long *firstSensor = NULL,  // first group
		*secondSensor = NULL, // second group
		*thirdSensor = NULL,  // third group
		*fourthSensor = NULL; // fourth group

/* leave it to defaults if no input given  */ 
long int N = 10, // x axis
     M = 15; // y axisi

/* outside vars */
extern char *optarg;

extern int opterr, 
			     optind, 
					 optopt;

char *datfile = NULL;
int fflag = 0,
		nflag = 0,
		mflag = 0,
		errflag = 0;

/* traversers */ 
int trav_x = 0,
		trav_y = 0,
		trav_z = 0,
		trav_v = 0;

/* tags */ 
int taggedZero = 0,
		taggedDense = 0,
		unTagged = 0;

/* char buf */ 
char c = 0;

/* get file name for input */ 
int
getName(int *argc, char *argv[]) {
	int c;

	/* use getopt to parse input */
	while((c = getopt(*argc, argv,":f:")) != -1) {
		/* check what we have */ 
		switch(c) {
			case 'f': {
				fflag++;
				/* put the file there */ 
				datfile = optarg;
				break;	
			}
			
			case ':': {
			  errflag++;
				break;
			}
			/* unknown argument */ 
			default : {
			  errflag++;
				break;
			}	
		}	
	}

	/* let's check it */ 
	if(!fflag)
		{fprintf(stderr, "\nError: Syntax is ./bscan -f infile.dat\n");}

	if(errflag)
		{fprintf(stderr, "\nError: probably have typed an unrecognized argument\n");}

	return (errflag == 0 ? RET_OK : ERR_BAD_INPUT);	
}

/* read the file */
int
readfile() {

	char l_buf[500] = {0}, // line buf
			 *p = NULL; // string array ptr

	FILE *fp = NULL; // file pointer

	int t_cnt = 0,
			f_lines = 0,
			firstline_tok = 0,
			secondline_tok = 0,
			thirdline_tok = 0,
			fourthline_tok = 0,
			fifthline_tok = 0,
			erflag = 0;

	long res = 0;
	/* check what we have */ 
	assert(datfile);

	/* try to open the file */ 
	if((fp = fopen(datfile, "r")) == NULL)
		{return (ERR_BAD_INPUT);}

	/* parse file and get N and M */
	while((fgets(l_buf, sizeof(l_buf), fp) != NULL) && f_lines < 5) { 

		p = strtok(l_buf, " ");
		while(p != NULL) {
			t_cnt++;
#ifdef DEBUG_LVL_2
			printf("\nToken Extracted: %s t_cnt is: %d lines are: %d", p, t_cnt, f_lines);
#endif
			switch(f_lines) {
				/*get N,M (optional) */ 
				case 0: {
					/* get N,M from file */	
					if(((res = strtol(p, NULL, 0)) > 0) && firstline_tok < 2)
						{if(firstline_tok	== 0) {N = res;}else{M = res;}firstline_tok++;}
					else
						{printf("\nError in first line, more than 2 (N,M) where given or invalid number!");}	
					break;	
				}
				case 1: {
					/* error check */ 
					if(firstline_tok < 2)
						{printf("\nError in first line you gave me only one value!");erflag++;break;}
					/* first sensor array values, should be equal to N */
					if(((res = strtol(p, NULL, 0)) >= 0) && (res >= 0 && res <= M) && t_cnt <= N) {
							/* check if we need to allocate */
					 		if(!firstSensor)
								{if(!(firstSensor = (long *)calloc(N,sizeof(long)))){erflag++;break;}}

							/* we don't, add it! */
							*(firstSensor+t_cnt) = res;secondline_tok++;
#ifdef DEBUG_LVL_1
							printf("\nAdding to first Sensor: %ld and value is: %ld", res, *(firstSensor+t_cnt));
#endif
						}	
					else
						{printf("\nError in second line around token %d with value %s \
(less than zero or larger than M?)", t_cnt, p);erflag++;}

					break;
				}
				case 2: {
					/* error check */ 
					if(secondline_tok != N)
						{printf("\nError in second line, you gave me values not equal to: %ld (N)", N);erflag++;break;}
					/* second sensor array values, should be equal to (N+M)-1 */

					if(((res = strtol(p, NULL, 0)) >= 0)  && res <= N+M-1 && t_cnt < (N+M)) {
							/* check if we need to allocate */
					 		if(!secondSensor)
								{if(!(secondSensor = (long *)calloc(N+M-1,sizeof(long)))){erflag++;break;}}

							/* we don't, add it! */
							*(secondSensor+t_cnt) = res;thirdline_tok++;
#ifdef DEBUG_LVL_1
							printf("\nAdding to second Sensor: %ld and value is: %ld", res, *(secondSensor+t_cnt));	
#endif
					}	
					else
						{printf("\nError in third line around token %d with value %s \
(less than zero or larger than N+M-1?)", t_cnt, p);erflag++;}
					break;
				}
				case 3: {
					/* error check */ 
					if(thirdline_tok != N+M-1) 
					{printf("\nError in third line, you gave me values not equal to: %ld (N+M-1)", N+M-1);erflag++;break;}

					if(((res = strtol(p, NULL, 0)) >= 0) && res <= M && t_cnt <= M) {
						/* check if we need to allocate */
						if(!thirdSensor)
							{if(!(thirdSensor = (long *)calloc(M,sizeof(long)))){erflag++;break;}}

						/* we don't, add it */
						*(thirdSensor+t_cnt) = res;fourthline_tok++;
#ifdef DEBUG_LVL_1
						printf("\nAdding to third Sensor: %ld and value is: %ld", res, *(thirdSensor+t_cnt));
#endif
					}
					else
						{printf("\nError in fourth line around token %d with value %s \
(less than zero or larger than M)", t_cnt, p);erflag++;}	
					break;
				}
				case 4: {
					/* error check */ 
					if(fourthline_tok != M)
						{printf("\nError in fourth line, you gave me values not equal to: %ld (M)", M);erflag++;break;}
					/* fourth sensor array values,, should be equal to (N+M)-1 */
					if(((res = strtol(p, NULL, 0)) >= 0) && res < N+M && t_cnt < (N+M)) {
							/* check if we need to allocate */
					 		if(!fourthSensor)
								{if(!(fourthSensor = (long *)calloc(N+M-1,sizeof(long)))){erflag++;break;}}
							
							/* we don't, add it! */
							*(fourthSensor+t_cnt) = res;fifthline_tok++;
#ifdef DEBUG_LVL_1
							printf("\nAdding to fourth Sensor: %ld and value is: %ld", res, *(fourthSensor+t_cnt));
#endif
					}	
					else
						{printf("\nError in fifth line around token %d with value %s \
(less than zero or larger than N+M-1?)", t_cnt, p);erflag++;}
					break;	
				}
			}

			/* don't waste cycles */ 	
			if(erflag)
				{break;}
			p = strtok(NULL, " ");
		}

		
		f_lines++;	
		/* reset token count */ 
		t_cnt = -1;
	}

	/* error check fifth line */ 
	if(!erflag && fifthline_tok != (N+M)-1)
		{printf("\nError in fifth line, you gave me values not equal to: %ld (N+M-1)", N+M-1);erflag++;}

	if(erflag > 0) {
		if(firstSensor) {free(firstSensor);}
		if(secondSensor) {free(secondSensor);}
		if(thirdSensor) {free(thirdSensor);}
		if(fourthSensor) {free(fourthSensor);}
	}

	/* close file */ 
	fclose(fp);

	/* return */ 	
	return(erflag == 0 ? RET_OK : ERR_BAD_INPUT);
}

/* prints the table */ 
int
printTable() {

	/* travesers in our table */ 
	trav_x = 0;
	trav_y = 0;

	if(!tab)
		{return(ERR_BAD_INPUT);}
	/* print up */ 
	printf("\n\n");
	for(trav_y = 0; trav_y < N; trav_y++) {
		for(trav_x = 0; trav_x < M; trav_x++) {
			c = *(tab+((trav_y*M /* y offset */ )+trav_x) /* x-offset */ ) + '\0';
			printf(" %c ", c);		
		}
		printf("\n");
	}

	return(RET_OK);
}


/* calcualtes the line statistics */ 
void
populateTags(int *yAxis, int *xAxis) {
		if(*(tab+((*yAxis)*M)+(*xAxis)) == '.')
			{taggedZero++;}
		else if(*(tab+((*yAxis)*M)+(*xAxis)) == '#')
			{taggedDense++;}
		else
			{unTagged++;}
}

void
tagFourthSensor() {

	/* initialize tags */
	taggedZero = taggedDense = unTagged = 0;
	/* fourth sensor tags, part one (x-axis sensors) */
	for(trav_x = 0; trav_x < M; trav_x++) {
		for(trav_y = N-1, trav_z = trav_x; 
				trav_y >= 0 && trav_z >= 0; 
				trav_y--, trav_z--) {
			/* understand the line */
			populateTags(&trav_y, &trav_z);

		}

#ifdef DEBUG_FOURTH_SENSOR_LVL_2
		printf("\n\nTagged stats for x: %d z: %d and y: %d are:\n",trav_x, trav_z, trav_y);
		printf("\n\tTagged zero: %d", taggedZero);
		printf("\n\tTagged dense: %d", taggedDense);
		printf("\n\tTagged nothing: %d", unTagged);
		printf("\n\tTagged in input: %ld\n\n", *(fourthSensor+trav_x));
#endif
		if(taggedDense+unTagged == *(fourthSensor+trav_z)) {
			for(trav_v = N-1, trav_z = trav_x;
					trav_v >= 0 && trav_z >= 0;
					trav_v--, trav_z--)

			/* tag it */
			if(*(tab+((trav_v)*M)+trav_z) == 'U')
				{memset(tab+(trav_v*M)+trav_z, '#', sizeof(char));}	
		}

		if(taggedDense == *(fourthSensor+trav_x)) {
			for(trav_v = N-1, trav_z = trav_x;
			trav_v >= 0 && trav_z >= 0;
			trav_v--, trav_z--)	
		
			/* tag it! */ 
			if(*(tab+((trav_v*M)+trav_z)) == 'U')
				{memset(tab+((trav_v*M)+trav_z), '.', sizeof(char));}		
		}
#ifdef DEBUG_FOURTH_SENSOR
		(void)printTable();
#endif

		unTagged = taggedDense = taggedZero = 0;	
	}

	/* fourth sensor tags, part two (y-axis sensors) */ 
	for(trav_x = 0; trav_x < N; trav_x++) {
		for(trav_y = N-(trav_x+1), trav_z = M-1; 
				trav_y >= 0 && trav_z >= 0;
				trav_y--, trav_z--) {
			/* understand the line */
			populateTags(&trav_y, &trav_z);					
		}

#ifdef DEBUG_FOURTH_SENSOR_LVL_2
		printf("\n\nTagged stats for x: %d z: %d and y: %d are:\n",trav_x, trav_z, trav_y);
		printf("\n\tTagged zero: %d", taggedZero);
		printf("\n\tTagged dense: %d", taggedDense);
		printf("\n\tTagged nothing: %d", unTagged);
		printf("\n\tTagged in input: %ld\n\n", *(fourthSensor+trav_x+M-1));
#endif
		if(taggedDense+unTagged == *(fourthSensor+trav_x+M-1)) {
			for(trav_v = (N-1)-trav_x, trav_z = M-1;
					trav_v >= 0 && trav_z >= 0;
					trav_v--, trav_z--)

			/* tag it */
			if(*(tab+((trav_v)*M)+trav_z) == 'U')
				{memset(tab+(trav_v*M)+trav_z, '#', sizeof(char));}
		}

		if(taggedDense == *(fourthSensor+trav_x+M-1)) {
			for(trav_v = N-1-trav_x, trav_z = M-1;
					trav_v >= 0 && trav_z >= 0;
					trav_v--, trav_z--)
			/* tag it */
			if(*(tab+((trav_v)*M)+trav_z) == 'U')
				{memset(tab+(trav_v*M)+trav_z, '.', sizeof(char));}
		}

#ifdef DEBUG_FOURTH_SENSOR
		(void)printTable();
#endif
		unTagged = taggedDense = taggedZero = 0;	
	}

}

void
tagSecondSensor() {

	/* initialize tags */ 
	unTagged = taggedZero = taggedDense = 0;
	/* tags for second sensor part one */
	for(trav_x = 0; trav_x < N-1; trav_x++) {
		for(trav_y = trav_x, trav_z = 0; 
				trav_y >= 0 && trav_z < M; 
				trav_y--, trav_z++){
				/* understand the line */
				populateTags(&trav_y, &trav_z);
			}

#ifdef DEBUG_SECOND_SENSOR_LVL_2
		printf("\n\nTagged stats for x: %d and y: %d are:\n", trav_x, trav_y);
		printf("\n\tTagged zero: %d", taggedZero);
		printf("\n\tTagged dense: %d", taggedDense);
		printf("\n\tTagged nothing: %d", unTagged);
		printf("\n\tTagged in input: %ld\n\n", *(secondSensor+trav_x));
#endif
			/* tag it, if needed! */

			if(taggedDense+unTagged == *(secondSensor+trav_x)) {
				for(trav_v = trav_x, trav_z = 0; 
						trav_v >= 0 && trav_z < M; 
						trav_v--, trav_z++)

					/* tag it! */ 
					if(*(tab+((trav_v*M)+trav_z)) == 'U')
						{memset(tab+((trav_v*M)+trav_z), '#', sizeof(char));}
			}

			if (taggedDense == *(secondSensor+trav_x)) {
					for(trav_v = trav_x, trav_z = 0; 
						trav_v >= 0 && trav_z < M; 
						trav_v--, trav_z++)

					/* tag it! */ 
					if(*(tab+((trav_v*M)+trav_z)) == 'U')
						{memset(tab+((trav_v*M)+trav_z), '.', sizeof(char));}
			}

			unTagged = taggedZero = taggedDense = 0;
	}	
#ifdef DEBUG_SECOND_SENSOR
	printTable();
#endif
	/* tags second sensor part two! */ 
	for(trav_x = 0; trav_x < M; trav_x++) {
		for(trav_y = N-1, trav_z = trav_x; 
				trav_y >= 0 && trav_z < M; 
				trav_y--, trav_z++)
			/* understand the line */
			{populateTags(&trav_y, &trav_z);}

#ifdef DEBUG_SECOND_SENSOR_LVL_2
		printf("\n\nTagged stats for x: %d and y: %d are:\n", trav_x, trav_y);
		printf("\n\tTagged zero: %d", taggedZero);
		printf("\n\tTagged dense: %d", taggedDense);
		printf("\n\tTagged nothing: %d", unTagged);
		printf("\n\tTagged in input: %ld\n\n", *(secondSensor+trav_x+N-1));
#endif
		/* tag it, if needed! */
		if((taggedDense+unTagged) == *(secondSensor+trav_x+N-1)) {
			for(trav_v = N-1, trav_z = trav_x;
					trav_v >= 0 && trav_z < M;
					trav_v--, trav_z++)

				/* tag it! */ 
				if(*(tab+((trav_v*M)+trav_z)) == 'U')
					{memset(tab+((trav_v*M)+trav_z), '#', sizeof(char));}
		}

		if(taggedDense == *(secondSensor+trav_x+N-1)) {
			for(trav_v = N-1, trav_z = trav_x;
				trav_v >= 0 && trav_z < M;
				trav_v--, trav_z++)	
			
			/* tag it! */ 
			if(*(tab+((trav_v*M)+trav_z)) == 'U')
				{memset(tab+((trav_v*M)+trav_z), '.', sizeof(char));}
		}

		unTagged = taggedDense = taggedZero = 0;
	
	}
#ifdef DEBUG_SECOND_SENSOR
	printTable();
#endif
}

void
tagThirdSensor() {

	/* initialize tags */ 
	unTagged = taggedZero = taggedDense = 0;
	/* tags for third sensor */
	for(trav_x = 0; trav_x < M; trav_x++) {
		for(trav_y = 0; trav_y < N; trav_y++) {
			/* understand the line */ 
			populateTags(&trav_y, &trav_x);
		}
#ifdef DEBUG_THIRD_SENSOR_LVL_2	
		printf("\n\nTagged stats for x: %d and y: %d are:\n", trav_x, trav_y);
		printf("\n\tTagged zero: %d", taggedZero);
		printf("\n\tTagged dense: %d", taggedDense);
		printf("\n\tTagged nothing: %d", unTagged);
		printf("\n\tTagged in input: %ld\n\n", *(firstSensor+trav_x));
#endif
		/* tag it, if needed */ 
		if(taggedDense+unTagged == *(thirdSensor+trav_x))
			{
				for(trav_z = 0; trav_z < N; trav_z++)
					{if(*(tab+((trav_z*M)+trav_x)) == 'U')
						{memset(tab+((trav_z*M)+trav_x), '#', sizeof(char));}}
			}
		if(taggedDense == *(thirdSensor+trav_x)) {	
				for(trav_z = 0; trav_z < N; trav_z++)
					{if(*(tab+((trav_z*M)+trav_x)) == 'U')
						{memset(tab+((trav_z*M)+trav_x), '.', sizeof(char));}}
		}

			/* reset counters */
	 		taggedDense = taggedZero = unTagged = 0;	
	}
#ifdef DEBUG_THIRD_SENSOR
	(void)printTable();
#endif
}


void
tagFirstSensor() {

	/* intialize tags */
	unTagged = taggedZero = taggedDense = 0;	
 	/* tags for first sensor */
 	for(trav_x = 0; trav_x < N; trav_x++) {
			
		for(trav_y = 0; trav_y < M; trav_y++) {
			/* understand the line! */ 
			populateTags(&trav_x, &trav_y);
		}

#ifdef DEBUG_FIRST_SENSOR_LVL_2	
		printf("\n\nTagged stats for x: %d and y: %d are:\n", trav_x, trav_y);
		printf("\n\tTagged zero: %d", taggedZero);
		printf("\n\tTagged dense: %d", taggedDense);
		printf("\n\tTagged nothing: %d", unTagged);
		printf("\n\tTagged in input: %ld\n\n", *(firstSensor+trav_x));
#endif
		/* explain it! */
	 	if(taggedDense+unTagged == *(firstSensor+trav_x))
			/* tag it */ 
			{
				for(trav_z = 0; trav_z < M; trav_z++)
				{
					if((c = *(tab+((trav_x*M)+trav_z)) + '\0') == 'U')
					{memset(tab+((trav_x*M)+trav_z),'#',sizeof(char));}}
					/* print updated table */
			}	
		 	if(taggedDense == *(firstSensor+trav_x))
			/* tag it */ 
			{
				for(trav_z = 0; trav_z < M; trav_z++)
				{
					if((c = *(tab+((trav_x*M)+trav_z)) + '\0') == 'U')
					{memset(tab+((trav_x*M)+trav_z),'.',sizeof(char));}}
					/* print updated table */
			}
		/* reset counters */ 
		taggedZero = taggedDense = unTagged = 0;	

	}

#ifdef DEBUG_FIRST_SENSOR
	(void)printTable();
#endif
}
/* analyze the data */
int
scan() {


	/* allocate our table */
	if(!tab)
		{if(!(tab = (char *)calloc(N*M+1, sizeof(char)))){errflag++;}}	

	/* Untag table */ 	
	memset(tab, 'U', (N*M)*sizeof(char));
	memset(tab+(N*M), '\0', sizeof(char));	

	/* initial tag, and print the diff */ 
	if(printTable())
		{return(ERR_BAD_INPUT);}	

	/* run until everything is good! */ 
	while(strchr(tab, 'U') != NULL) {
			/* first sensor tag */
		tagFirstSensor(); 
		/* second sensor tag */
		tagSecondSensor();
		/* third sensor tag */
		tagThirdSensor();	
		/* fourth sensor tag */
		tagFourthSensor();	
	}

	/* print our results... in the end */ 
	printf("\n\nFinished Processing Data\n\nPrinting results:");
	printTable();
	/* greetings! */ 
	printf("\n\nAll Ok!\nBye Bye!\n\n");
	return(RET_OK);
}

/* clean up our mess */ 
int
cleanup() {
	if(firstSensor) {free(firstSensor);}
	if(secondSensor) {free(secondSensor);}
	if(thirdSensor) {free(thirdSensor);}
	if(fourthSensor) {free(fourthSensor);}
	if(tab) {free(tab);}
	return(RET_OK);
}

int
main ( int argc, char *argv[] ) {

	/* get filename from args */ 
	if(getName(&argc, argv) == ERR_BAD_INPUT)
		{return (EXIT_FAILURE);}

	/* parse the file */ 	
	if(readfile())
		{printf("\n\nBad Input file...terminating\n");return(EXIT_FAILURE);}	

	/* print message */ 
	printf("\n\nStarting Scanner with parameters: \n\n\tdata file: \
%s\n\tN: %ld \n\tM: %ld\n", datfile, N, M);

	scan();
	/* clean up */
	cleanup();
	return EXIT_SUCCESS;
}				
/* ----------  end of function main  ---------- */
