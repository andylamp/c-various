/*
 * =====================================================================================
 *
 *       Filename:  p3.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/19/10 23:53:49
 *       Revision:  none
 *       Compiler:  gcc
 *
 *	  License:  This software is Licensed under the terms and condtions of 
 *	  				the BSD License model
 *
 * =====================================================================================
 */



#include	<stdlib.h>
#include	<stdio.h>
#include 	<string.h>
#include	<errno.h>

/* max len */
#define TOOL_NAME_MAX 100
/* max line len */
#define LINE_MAX 200

/* tool struct */
typedef struct atool {
	/* tool code */
	int t_code;
	/* tool name */
	char t_name[TOOL_NAME_MAX];
	/* quantity */
	int t_quantity;
	/* unit price */
	int t_price;
	/* next */
	struct atool *next;
} tool;

/* list head */
tool *head = NULL;

/* forward declarations */

/* --- LIST --- */

/* list add */
tool *
listAdd(tool **l_head, tool *t_node) {
	/* add to list */	
	t_node->next = *l_head;
	if(t_node->next == NULL)
		{printf("\neinai null");}
	*l_head = t_node;

	/* return */
	return(*l_head);
} 

/* search */
tool **
listSearch(tool **l_head, int code) {
	tool **trav = l_head;
	/* base case */
	if(!trav)
		return(NULL);
	/* search, search */
	while(*trav) {
		if((*trav)->t_code == code)
			{return(trav);}
		else
			{trav = &(*trav)->next;}
	}	

	return(NULL);
}

/* list remove */
tool *
listRemove(tool **l_head, int code) {
	tool *t = NULL,
		 **tmp = NULL;
	/* Search */
	tmp = listSearch(l_head, code);
	/* free it */
	if(tmp && *tmp) {	
		t = (*tmp);
		*tmp = ((*tmp)->next);
		free(t);		
	}else {
		/* no nodes foud */
		printf("\n\n--> No tools found using code: %d", code);
	}
	/* return */
	return(*l_head);
}


/* list clear */
tool *
listClear(tool *l_head) {
	tool *t = NULL;//l_head->next;
	while(t) 
	{l_head = l_head->next; free(t); t = l_head;}

	/* null it, just in case */
	return(NULL);
}


/* --- LIST --- */

/* draw menu */
int
drawMenu(int *choice);

/* get input from the user */
int
getInput();

/* get string input */
char *
getSInput();

/* read file */
int
readFile(tool **l_head);

/* list contents */
int
listContents(tool **l_head);

/* insert Tool */
int
insertTool(tool **l_head);

/* update Tool */
int
updateTool(tool **l_head);

/* store to file  */
int
storeToFile(tool **l_head);

/* delete tool */
int
deleteTool(tool **l_head);

/* 
 * ===  FUNCTION  ======================================================================
 *
 *         Name:  main
 *
 *  Description: Toolbox...oh...my Toolbox! 
 *
 * =====================================================================================
 */
int
main ( int argc, char *argv[] )
{
	/* our choice */
	int choice = 0;

	/* initial choice */
	(void)drawMenu(&choice);

	/* main loop */
	while(choice != 7) {
		switch(choice) {
			case 1: {
				(void)readFile(&head);	
				break;
			}

			case 2: {
				(void)listContents(&head);
				break;
			}

			case 3: {
				(void)insertTool(&head);
				break;
			}

			case 4: {
				(void)deleteTool(&head);
				break;
			}

			case 5: {
				(void)updateTool(&head);
				break;
			}

			case 6: {
				(void)storeToFile(&head);
				break;
			}
			default: {
				/* exit */
				break;
			}
		}

		/* choose again */
		(void)drawMenu(&choice);
	}

	/* remove the list */
	if(head)
		{head = listClear(head);}

	/* exit */
	return EXIT_SUCCESS;
}				
/* ----------  end of function main  ---------- */

/* draw the menu */
int
drawMenu(int *choice) {

	/* draw menu */
	(void)fprintf(stdout, "\n1) Read from file");
	(void)fprintf(stdout, "\n2) List contents");
	(void)fprintf(stdout, "\n3) Insert a tool");
	(void)fprintf(stdout, "\n4) Delete a tool");
	(void)fprintf(stdout, "\n5) Update a tool");
	(void)fprintf(stdout, "\n6) Store to file");
	(void)fprintf(stdout, "\n7) Exit\n");

	(void)fprintf(stdout, "\n\tEnter a choice (1-7): "); 
	/* ensure we get what we want */
  	while((*choice = getInput()) == -1 || 
			*choice < 1 || *choice > 7) {
		(void)fprintf(stdout, "\n\n\tPlease try again (1-7): "); 
	}

	/* return, choice value */
	return(*choice);
}

/* get input */
int
getInput() {
	/* l buf */
	char l_buf[100],
		 *p = NULL;
	/* our result */
	int res = 0;

	/* memset, as always */
	memset(l_buf, 0, sizeof(l_buf));

	/* get the line */
	if(!fgets(l_buf, sizeof(l_buf), stdin))
		{return(-1);}

	/* we need 1 token */
	if(!(p = strtok(l_buf, " ")))
		{return(-1);} 

	/* see if we got a number */
	if((res = strtol(p, NULL, 0)) == 0)
		{return(-1);}

	/* finally we got it! */
	return(res);
}

/* get input */
char *
getSInput() {
	/* l buf */
	char l_buf[100],
		 *p = NULL;

	/* memset, as always */
	memset(l_buf, 0, sizeof(l_buf));

	/* get the line */
	if(!fgets(l_buf, sizeof(l_buf), stdin))
		{return(NULL);}

	/* we need 1 token */
	if(!(p = strtok(l_buf, " ")))
		{printf("String in: %s", p);return(NULL);} 
	/* return it */
	printf("\nString in: %s", p);
	/* finally we got it! */
	return(p);
}

/* parse the file */
int
parseFile(FILE *fp) {	

	/* each file line buf */
	char l_buf[LINE_MAX],
		 *p = NULL;

	/* the node we want to add */
	tool *t_node = NULL;

	/* counter, for line tokens */
	int i = 0;

	/* zero it out */
	memset(l_buf, 0, sizeof(l_buf));
	
	/* empty list for the new file */
	head = listClear(head);
	
	/* read each line */
	while(fgets(l_buf, LINE_MAX, fp) != NULL) {
		/* we got a line, break it apart */

		printf("\n\n--> We read: %s", l_buf);
		/* allocate a node */
		t_node = (tool *)calloc(1, sizeof(tool));
		t_node->next = NULL;

		/* zero it */
		memset(t_node->t_name, 0, sizeof(t_node->t_name));
		/* loop through the possible results */
		for(p = strtok(l_buf, " "); p; 
				p = strtok(NULL, " "), i++) {
			/* check what we got */
			printf("\nToken: %s", p);
			switch(i) {
				case 0: {
					/* assign it */
					t_node->t_code = strtol(p, NULL, 0);	
					break;
				}
				case 1: {
					/* copy string */
					strncat((t_node->t_name), p, strlen(p));
					break;
				}
				case 2: {
					/* quantity */
					t_node->t_quantity = strtol(p, NULL, 0);
					break;
				}
				case 3: {
					/* unit price */
					t_node->t_price = strtol(p, NULL, 0);
					break;
				} default: { ; } 
			}
			if(i > 4)
				/* error in file */
				{fclose(fp);if(t_node) free(t_node);return(-1);}
		}
		i = 0;

		printf("\n\nCode: %d\nName: %s\nQuantity: %d\nPrice : %d\n", 
			t_node->t_code, t_node->t_name, t_node->t_quantity, t_node->t_price);

		memset(l_buf, 0, sizeof(l_buf));
		/* add to tool list */
		listAdd(&head, t_node);	
	} 

	/* we done? */
	return(1);
}


/* read file */
int
readFile(tool **l_head) {
	/* file pointer */
	FILE *fp = NULL;
	/* actual file buf */
	char fa[100];

	/* inform the user to type */	
	(void)fprintf(stdout, "\n\tPlease enter a filename: ");	
	/* zero out memory */
	(void)memset(fa, 0, sizeof(fa));
	/* get input */
	scanf("%99s", fa);

	printf("\nOpening file: %s", fa);
	/* try to open file */
	if((fp = fopen(fa, "r")) == NULL)
		{(void)fprintf(stdout, "\n\nError, wrong file or permission denied\n"); 	
		printf("\n\nDebug Info --> Errno Val: %d\n\n", errno);
		/* hax */
		fgets(fa, sizeof(fa), stdin);
		return(-1);}

	/* hax */
	fgets(fa, sizeof(fa), stdin);
	/* parse file */
	if(parseFile(fp) < 0)
		{fclose(fp);return(-1);}

	/* close fp */
	fclose(fp);
	
	return(1);
}


/* update file */
int
storeToFile(tool **l_head) {
	/* file pointer */
	FILE *fp = NULL;
	/* actual file buf */
	char fa[LINE_MAX];
	tool *trav = head;
	/* inform the user to type */	
	(void)fprintf(stdout, "\n\tPlease enter a filename: ");	
	/* zero out memory */
	(void)memset(fa, 0, sizeof(fa));
	/* get input */
	scanf("%99s", fa);

	(void)fprintf(stdout, "\nOpening file: %s", fa);
	if((fp = fopen(fa, "w+")) == NULL)
		{(void)fprintf(stdout, "\n\nError, wrong file or permission denied\n"); 	
		(void)fprintf(stdout, "\n\nDebug Info --> Errno Val: %d\n\n", errno);
		/* hax */
		fgets(fa, sizeof(fa), stdin);
		return(-1);}

	/* hax */
	fgets(fa, sizeof(fa), stdin);

	/* write file */
	while(trav) {
		memset(fa, 0, sizeof(fa));
		/* construct write string */
		snprintf(fa, sizeof(fa), "%d %s %d %d\n", 
			trav->t_code, trav->t_name, trav->t_quantity, trav->t_price);
		/* write it */

		/* NOTE: bored to write...code to ensure correct write, although
  				most times it will be just fine if not all; but deal with it */
		fwrite(fa, strlen(fa), sizeof(char), fp);
		/* advance trav */
		trav = trav->next; 
	}
	/* close fp */
	fclose(fp);
	
	return(1);
}

/* insert tool */
int
insertTool(tool **l_head) {

	/* get a new tool */
	tool *n_t = NULL;	
	int res = 0;

	char *p = NULL;

	/* allocate it */
	if(!(n_t = (tool *)calloc(1, sizeof(tool))))
		return(-1);
	/* be safe */	
	n_t->next = NULL;
	/* get data */
		
	(void)fprintf(stdout, "\nEnter a tool name: ");
		while(!(p = getSInput()))
			{(void)fprintf(stdout, "\nTry again: ");}

	/* update it */
	memset(n_t->t_name, 0, sizeof(n_t->t_name));
	/* append it, DON'T use strncpy or even worse strcpy */
	strncat(n_t->t_name, p, strlen(p));

	(void)fprintf(stdout, "\nEnter a tool code: ");
		while((res = getInput()) < 0 || res < 0) 
			{(void)fprintf(stdout, "\nTry again: ");}
	/* put it */
	n_t->t_code = res;

	(void)fprintf(stdout, "\nEnter a tool quantity: ");
		while((res = getInput()) < 0 || res < 0) 
			{(void)fprintf(stdout, "\nTry again: ");}
	/* put it */
	n_t->t_quantity = res;

	(void)fprintf(stdout, "\nEnter a tool unit price: ");
		while((res = getInput()) < 0 || res < 0) 
			{(void)fprintf(stdout, "\nTry again: ");}
	/* put it */
	n_t->t_price = res;

	/* add it to list */
	listAdd(l_head, n_t);

	/* we done? */
	return(1);
}

/* list contents */
int
listContents(tool **l_head) {
	tool *trav = *l_head;

	(void)fprintf(stdout, "\nPrinting Tools: \n\n");
	while(trav) {
		/* print tools */
		(void)fprintf(stdout, "\nPrinting Tool: %s\n", trav->t_name);
		(void)fprintf(stdout, "\n\tTool Code: %d\n", trav->t_code);
		(void)fprintf(stdout, "\n\tTool Quantity: %d\n", trav->t_quantity);
		(void)fprintf(stdout, "\n\tTool Unit Price: %d\n", trav->t_price);
		
		/* advance */
		trav = trav->next;
	} 
	return(1);
}

/* update tool */
int
updateTool(tool **l_head) {

	tool **t = NULL;

	char n_buf[TOOL_NAME_MAX],
		 *p = NULL;

	int code = 0,
		res = 0;

	/* as usual reset memory */
	memset(n_buf, 0, sizeof(n_buf));

	(void)fprintf(stdout, "\nPlease enter the code of the tool you want to update: ");

	/* get input */	
	while((code = getInput()) < 0)
		(void)fprintf(stdout, "\nPlease try again: ");	

	/* search */
	t = listSearch(l_head, code);

	if(!t)	
		{(void)fprintf(stdout, "\nNo tool found using code: %d\n", code);return(NULL);}

	/* we found */
	(void)fprintf(stdout, "\nNode found with following data: \n");

	/* print tool */
	(void)fprintf(stdout, "\n\t 1)Tool Name: %s\n", (*t)->t_name);
	(void)fprintf(stdout, "\n\t2) Tool Code: %d\n", (*t)->t_code);
	(void)fprintf(stdout, "\n\t3) Tool Quantity: %d\n", (*t)->t_quantity);
	(void)fprintf(stdout, "\n\t4) Tool Unit Price: %d\n", (*t)->t_price);
	
	(void)fprintf(stdout, "\n\tEnter data number you want to update (1-4): ");

	/* printf what we want to update */
	while((code = getInput()) < 0 || code < 1 || code > 4)
		{(void)fprintf(stdout, "\nPlease try again (1-4): ");}		

	/* switch, actually bigger code but saves cycles */
	switch(code) {
		case 1: {
				/* name */
				(void)fprintf(stdout, "\nEnter new Tool name: ");
				/* get it */
				while(!(p = getSInput()))
					{(void)fprintf(stdout, "\nTry again: ");}
				/* zero memory */
				memset((*t)->t_name, 0, TOOL_NAME_MAX);
				/* append */
				strncat((*t)->t_name, p, strlen(p));	
				break;
			}

		case 2: {
				/* Code */
				(void)fprintf(stdout, "\nEnter new Tool Code: ");
				while((res = getInput()) < 0 || res < 0) 
					{(void)fprintf(stdout, "\nTry again: ");}
				/* update */ 
				(*t)->t_code = res;
				break;
			}

		case 3: {
				/* Quantity */
				(void)fprintf(stdout, "\nEnter new quantity: ");
				while((res = getInput()) < 0 || res < 0) 
					{(void)fprintf(stdout, "\nTry again: ");}
				/* update */ 
				(*t)->t_quantity = res;
				break;
			}

		case 4: {
				/* Price */
				(void)fprintf(stdout, "\nEnter new price: ");
				while((res = getInput()) < 0 || res < 0) 
					{(void)fprintf(stdout, "\nTry again: ");}
				/* update */ 
				(*t)->t_price = res;	
				break;
			}
		default: { ; }
	}

	/* we done? */
	return(1);
}

/* delete tool */
int
deleteTool(tool **l_head) {
	/* our code to search */	
	int code = 0;

	fprintf(stdout, "\nPlease enter the code for the node to delete: ");

	while((code = getInput()) < 0)
		fprintf(stdout, "\nPlease try again: ");
	
	if(listRemove(l_head, code))
		(void)fprintf(stdout, "\nNode Successfully removed!\n");
	else
		(void)fprintf(stdout, "\nNode to remove wasn't found\n");

	return(1);
}
