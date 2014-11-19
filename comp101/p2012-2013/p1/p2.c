/*
 * =====================================================================================
 *
 *       Filename:  p2.c
 *
 *    Description: This is the recursive motif production. This is quite tricky...
 *                  basically what this is, an inverse tetris (or sort of) pattern
 *                  that copies itself over time; as illustrated in the pdf document
 *                  it follows the following pattern:
 *                  
 *                  Note: for blank-spots I use '-' just to accurately show them.
 *
 *                  1 (2^1):
 *                      *-
 *
 *                  2 (2^2):
 *                      *_
 *                      **
 *                      _*
 *
 *                  3 (2^3):
 *                      *_
 *                      **
 *                      _*
 *                      ****
 *                      __*_
 *                      __**
 *                      ___*
 *
 *                  and so on... well we can see the recursive pattern here, but
 *                  the true clue is that each level is actually the previous level
 *                  idented by a number of blank spots that's equal to the previous 
 *                  level's degree divided by two, so in our example the first level
 *                  has zero, as it's the first... the second is 2^1, the third is 2^2
 *                  and the list goes on... each level is also separated by a line
 *                  that "cuts it in half" has that many dots as the size of the level
 *                  divided by two.
 *
 *                  The other part that has to be noticed is that you can contruct a
 *                  tree like structure that we traverse in sort of an inorder fashion;
 *                  which basically let us traverse the nodes, print the leaves (which
 *                  in our case always is the pattern of size 1, or 2^1 to be excact).
 *                  This is also convenient because when we finish a branch and go up, 
 *                  we print the required level dots (*). The problem was the corrent 
 *                  identation of the levels, that was solved by passing a depth 
 *                  parameter in the function that was global to the tree; this
 *                  variable indicates the actual tree level and is used in conjunction
 *                  with the the pdepth variable which indicates the branch level that 
 *                  we currently are. The values of these two variables are then used to
 *                  deduce the corrent identation required by each level and branch. 
 *                  Unfortunatly we do require quite a lot of hectic cases do accomplish 
 *                  that; this is due to the fact that the tree is not symentric and
 *                  is one sided (right) so these cases are basically required to align 
 *                  the left branches of the tree as the right ones are correct.
 *
 *                  Anyway... grab the code, for your pleasure.
 *
 *        Version:  1.0
 *        Created:  11/11/2012 22:25:34
 *       Revision:  none
 *       Compiler:  gcc 
 *
 * =====================================================================================
 */

/* defines */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/* print the same c, n times */
void
printRepeatingCharacter(ssize_t n, char c) {
    ssize_t i = 0;
    if(n<=0) {return;} /* no point */
    for(i = 0; i < n; i++)
        {printf("%c",c);}
}

/* this draws the pattern... */
void
drawPattern(ssize_t size, ssize_t ident, ssize_t pdepth, ssize_t *depth) {

    if(size == 2) {
        printRepeatingCharacter(*depth, ' ');
        printRepeatingCharacter(size/2, '*');
        printRepeatingCharacter(size/2, ' ');
    } else {
 
        drawPattern(size/2, ident, pdepth, depth); printf("\n");
        /* indicates motif level (final should be N-1 (where N the input number) */
        (*depth)++;
        /* branch depth */
        pdepth++;
        if(size == ident && (*depth)%2 != 0 && (*depth) > 3) {
            printRepeatingCharacter((*depth)-1, ' ');
            printRepeatingCharacter(size/2, '*');
        } else if(size == ident || size*2 == ident) {
            printRepeatingCharacter(*depth-(size/4), ' ');
            printRepeatingCharacter(size/2, '*');
        } else if(ident > 0 && size == ident && *depth%2 == 0) {
           printRepeatingCharacter(ident+pdepth, ' ');           
            printRepeatingCharacter(size/2, '*'); 
        } else if (size%4 == 0 && ident%8 == 0 && *depth%2 != 0) {
            printRepeatingCharacter((*depth)-1, ' ');
            printRepeatingCharacter(size/2, '*');           
        } else if (size == 16 && ident%16 == 0 && *depth%2 == 0 && ident > 0) {
            printRepeatingCharacter((*depth)-4, ' ');
            printRepeatingCharacter(size/2, '*');      
        } else if ( size == 8 && ident > size) {
            printRepeatingCharacter(*depth-2, ' ');
            printRepeatingCharacter(size/2, '*');
        }
        else {
            printRepeatingCharacter(ident/2, ' ');
            printRepeatingCharacter(size/2, '*');	
        }

        printf("\n");
        if(size == ident && *depth%2 != 0) {
            drawPattern(size/2, ((*depth)*2)-3, pdepth, depth);
        }
        else {
            drawPattern((size/2), size/2, pdepth, depth); 
        }

    }
}

/* just.. main */
int
main ( int argc, char *argv[] )
{

    ssize_t depth = 0;
    drawPattern(64,0, 0, &depth);
	printf("\n");
    return EXIT_SUCCESS;
}				
/* ----------  end of function main  ---------- */
