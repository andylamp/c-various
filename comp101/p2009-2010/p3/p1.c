/*
 * =====================================================================================
 *
 *       Filename:  p1.c
 *
 *    Description:  This file will generate 'word's based on an algorithm that was
 *    					maybe created by some 'brainicas' that wanted to waste
 *    					CPU cycles in order to help the 'memory brain dead'
 *
 *    					complexity is 3^n...super-duper efficient, as they say.
 *
 *    		Now for the boring theoretical stuff...because the program to be understood 
 *    			completly I must explain how I came up with tne DnQ algorithm that performs 
 *    			that stupid task. First of all you  HAVE to learn to write efficient code...
 *    			otherwise you'd be better off doing another job, like creating web-sites and 
 *    			the likes. Anyway that quest starts by learning how to actually THINK how to 
 *    			write good algorithms for your program. Anyway, in this I'll explain my take on 
 *    			the solution of this program. I won't explain anything of the language features
 *    			used but only and briefly the algorithm used. We had to create a program that 
 *    			would output all the possible combinations of letters that would come up using
 *    			numbers (presumably) typed from the keyboard. Each number would be assigned
 *    			3 letter of the Enligh Roman Alphabet except the likes of 0 & 1 (the two most
 *    			used numbers in our science, but hey! we are geeks! We won't do well with tuxedos
 *    			and businesses!). Anyhow we were asked to create a recursive method to accomplish
 *    			this, hence a devide and conquer algorithm (or also known as DnQ in algorithmics).
 *
 *    		The problem has multiple solution, from either getting insanly complex and creating
 *    			a dynamic memory allocation for each entry, then printing them; parsing each seperate
 *    			entry while allocating it and constructing it...many things. My take is quite simple 
 *    			and uses very few cycles and 1 memory allocation (which has size equal to the number
 *				of digits our input has, duh); so I think we are pretty good there. Also there no
 *				string manipulation methods called during the iterations and basically all the function
 *				does is check values and assignments, which are pretty fast too! So there you have it!
 *				Efficienty
 *				!!
 *
 *			The algorithm used as said above is a DnQ one, but I'll explain it by illustrating an actual
 *				example, say you have an input of 45, that means 4 { G,H,I } and 5 { J,K,L } and 9 cases,
 *				let's contruct our table:
 *
 * 			Group 1:
 *					G J
 *					G K
 *					G L
 *			Group 2:
 *					H J
 *					H K
 *					H L
 *			Group 3:
 *					I J
 *					I K
 *					I L
 *
 *			If you don't get the formula yet...I'll get the first 6 examples of a 3 digit input, let's say
 *				234 that means 2 { A,B,C }, 3 { D,E,F } and 4 { J,K,L } and 27 cases, although we will 
 *				draw the first 9:
 *
 *			Group 1:
 *					A D G
 *					A D H
 *					A D I
 *			Group 2:
 *					A E G
 *					A E H
 *					A E I
 *			Group 3:
 *					A F G
 *					A F H 
 *					A F I (popular band btw, not my taste...but just so you know)
 *
 * 			The trick is... to notice that in each iteration the last 3 digits always stay the same and after
 * 				each group only 1-byte is changed. So an ASCII illustration of this (which I love btw) is the
 * 				following:
 *
 * 			Group 1:
 *			iteration 1: deep peak is 1 (or n-1) and y (which indicates the byte change) is 0:
 *
 *			Pos 0  1  2
 *
 * 				A  D  G << shift last by 1 b
 * 				A  D  H << shift last by 1 b
 * 				A  D  I << etc...
 *				   ^  ^
 *				   |  |
 *				   |  |
 *				   |  |---- remains the same
 *				   |
 *				   |----- change by 1 byte after printing the three, and you get the next group]
 *
 *			Group 2:
 *			iteration 2: deep peak is 1 (or n-1) and y (which indicates the byte change) is 1:
 *			
 *			Pos 0  1  2
 *
 * 				A  E  G << shift last by 1 b
 * 				A  E  H << shift last by 1 b
 * 				A  E  I << etc...
 *				   ^  ^
 *				   |  |
 *				   |  |
 *				   |  |---- remains the same
 *				   |
 *				   |----- change by 1 byte after printing the three, and you get the next group
 *
 * 			Group 3:
 *			iteration 3: deep peak is 1 (or n-1) and y (which indicates the byte change) is 2:
 *			
 *			Pos 0  1  2	
 *
 * 				A  F  G << shift last by 1 b
 * 				A  F  H << shift last by 1 b
 * 				A  F  I << etc...
 *				   ^  ^
 *				   |  |
 *				   |  |
 *				   |  |---- remains the same
 *				   |
 *				   |----- change by 1 byte after printing the three, and you get the next group
 *
 * 			Now the real magic happens, dp is reduced by one and we only shift the next bytes (remember
 * 				that we do only 1 byte change that a time!)
 *
 *			Group 4:
 *			iteration 4: deep peak is 0 (or n-2) and y (which indicates the byte change) is 2, please note that
 *					when dp is 0 we reset b and advance the byte of the initial character by one (1 byte):
 *			
 *			Pos 0  1  2	
 *
 * 				B  D  G << shift last by 1 b
 * 				B  D  H << shift last by 1 b
 * 				B  D  I << etc...
 *				^  ^  ^
 *				|  |  |
 *				|  |  |
 *				|  |  |---- remains the same
 *				|  |
 *				|  |----- we reset y, so we can loop again
 *				|  
 * 				|-----  change by 1 byte after printing the three, and you get the next group
 *
 *
 *			Hope you get it better than me in your project! This is mean as an example on how to go about
 *				this project!
 *
 *        Version:  1.0
 *        Created:  12/02/10 23:33:18
 *       Revision:  none
 *       Compiler:  gcc
 *
 *	  License:  This software is Licensed under the terms and condtions of the 
 *	  				BSD License model
 *
 * =====================================================================================
 */


#include	<ctype.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>

/* our array with strings */
char array[][3] = {
	/* 0 & 1 not used */
	{ 'A', 'B', 'C' }, /* 2 */
	{ 'D', 'E', 'F' }, /* 3 */
	{ 'G', 'H', 'I' }, /* 4 */
	{ 'J', 'K', 'L' }, /* 5 */
	{ 'M', 'N', 'O' }, /* 6 */
	{ 'P', 'R', 'S' }, /* 7 */
	{ 'T', 'U', 'V' }, /* 8 */
	{ 'W', 'X', 'Y' }  /* 9 */
};

char *ptr_array = NULL;

/* line buf */
char l_buf[100];

/* int buf */
int n_buf[100];

/* total */
int n_count = 0;

/* read the number */
int
readNumber();

/* print 3-numbers in Y-axis */
void
printYVariations(char ptr[][3], int currNum);

/* create the base array */
int
constructArray(char *p, int *pos);

/* recursive function to print all combs */
int 
printCharacterCombinations(char ptr[][3], int dp, int *i, int y);

/* 
 * ===  FUNCTION  ======================================================================
 *
 *         Name:  main
 *
 *  Description: pew pew, we are! 
 *
 * =====================================================================================
 */
int
main ( int argc, char *argv[] )
{
	int i = 0;
	
	/* get number */
	(void)fprintf(stdout, "\nPlease enter a number: ");
	while((n_count = readNumber()) == -1)
		{(void)fprintf(stdout, "\nPlease enter corrent number: ");}

	/* print our number */
	while(i < n_count)
		{printf("\n\t %d) Number : %d\n", i+1, n_buf[i]);i++;}	

	i = 0;
	/* allocate */
	if(!(ptr_array = (char *)calloc(n_count+1, sizeof(char))))
		{return(EXIT_FAILURE);}

	/* print mem address */
	printf("\nAddress start: 0x%d, end: 0x%d\n", (int)&ptr_array, (int)&ptr_array+n_count+1);
	/* construct base array */
	constructArray(ptr_array, &i);
	/* user info */ 
	printf("\nArray contains: %s\n\n", ptr_array);

	/* initialize i */
	i = 0;
	/* print before we start */
	printf("\n--> i is: %d\n\n", i);
	/* print all possible combinations */	
	printCharacterCombinations(array, n_count-2, &i, 0);
	/* free our memory */
	if(ptr_array)
		free(ptr_array);
	/* exit */
	return (EXIT_SUCCESS);
}				
/* ----------  end of function main  ---------- */

/* read the number*/
int
readNumber() {
	/* number count */
	int n_cnt = 0;
	/* buf for conversion */
	char c;	

	/* reset memory */
	memset(l_buf, 0, sizeof(l_buf));
	memset(n_buf, 0, sizeof(n_buf));


	/* get the line */
	if(!fgets(l_buf, sizeof(l_buf), stdin))
		{return(-1);}

	/* seperate numbers */
	while(l_buf[n_cnt] != '\n' && isdigit(l_buf[n_cnt]) != 0)
	{
		/* create local copy */
		c = l_buf[n_cnt] + '\0';
		/* one of the limited number of cases that atoi 
				is actually useful */
		n_buf[n_cnt] = atoi(&c);
		/*  error */
		if(n_buf[n_cnt] == 0 || n_buf[n_cnt] == 1)
			{return(-1);}
		n_cnt++;
	}

	/* check if we have a number */
	if(l_buf[n_cnt] == '\n')
		return(n_cnt);
	else
		return(-1);
}

/* fill out string array */
int
constructArray(char *p, int *pos) {
	/* reached end of string */
	if(*pos==n_count)
		return(1);

	/* check what numbers we have */	
	switch(n_buf[*pos]) {
		case 2:
			{strncat(p, &array[0][0], 1); break;}  
		case 3:
			{strncat(p, &array[1][0], 1); break;}
		case 4:
			{strncat(p, &array[2][0], 1); break;}
		case 5:
			{strncat(p, &array[3][0], 1); break;}
		case 6:
			{strncat(p, &array[4][0], 1); break;}
		case 7:
			{strncat(p, &array[5][0], 1); break;}
		case 8:
			{strncat(p, &array[6][0], 1); break;}
		case 9:
			{strncat(p, &array[7][0], 1); break;}
		default:
			{ break; }
		}
	/* advance */
	(*pos)++;
	constructArray(p, pos);

	return(1);

}

/* print */
int
printVariations(char ptr[][3], char *p) {

	char *c = p;
	int index = n_buf[n_count-1] - 2;
	/* done? */
	c += n_count-1;	

	*c = array[index][0];	
	fprintf(stdout, "\n\tValid Unique word: %s", ptr_array);

	*c = array[index][1];
	fprintf(stdout, "\n\tValid Unique word: %s", ptr_array);

	*c = array[index][2];
	fprintf(stdout, "\n\tValid Unique word: %s\n", ptr_array);

	/* done */
	return(1);
}

/* print possible combinations */
int 
printCharacterCombinations(char ptr[][3], int dp, int *i, int y) {

	char *p = ptr_array,
		 *t = ptr_array;

	/* we done? */
	if(*i == 3)
		{return(1);}

	/* reset our depth peek */
	if(dp == 0) {
		
		if(y!=0 || n_count == 2) 
			printVariations(ptr, p);
			
		/* advance it */
		(*i)++;

		/* print info while swapping */
		printf("\n\n Buf info:\n     n_buf[0]: %d i: %d y: %d\n", n_buf[0], *i, y);
		printf("\n --> Contents of array: %s Swaping: %c ", p, array[(n_buf[0]-2)][*i]);
		*p = array[n_buf[0]-2][*i];	
		printf("with %c\n\n", array[n_buf[0]-2][*i]);
		/* reset dp */
		dp = n_count-2;

	} else {
		printf("\n\t Info:\n\t\t dp is %d, i is: %d y is: %d\n", dp, *i, y);
			
		t += dp;
		printf("\nLocation is : %d, %d swapping letter %c", n_buf[dp]-2, y, *t);

		*t = array[n_buf[dp]-2][y];

		printf(" with %c\n\n", *t);

		y++;
		if(y == 3)
			{dp--;y=0;}	
		printVariations(ptr, p);
	
	}

	/* recurse */	
	return(printCharacterCombinations(ptr, dp, i, y));	
}

