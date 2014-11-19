/* mastermind "clone" or w/e */

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>
#include <limits.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>

/* limits of n, defined at compile time */
#define MIN_N 3
#define MAX_N 5

#define MIN_LEN 1
#define MAX_LEN 1

/* greetings... grouped up */
void
greeting_msg() {
   printf("!! A Mastermind clone welcomes you\n"
          "\n -- Initialized with allowed (min,max) length: (%d, %d)\n",
          MIN_N, MAX_N);
}

/* print num */
void
print_num(int **num, const int *nlen) {
   int i = 0;
   printf("\n Number is: ");
   for(i = 0; i < *nlen; i++) 
      {printf(" %d ", (*num)[i]);}
   printf("\n");
}

/* parse n */
void
parse_n(int *n) {
   size_t llen = 0;
   ssize_t alen = 0;
   char *buf = NULL;
   extern int errno;

   /* parse until needed */
   while(alen < (MIN_LEN+1) || alen > (MAX_LEN+1)) {
      printf("\n ** Please select valid n: "); 
      alen = getline(&buf, &llen, stdin);
   }
   /* assign n */
   if(n != NULL) {
      errno = 0;
      *n = strtol(buf, NULL, 10);
      /* check for conversion errors */
      if(errno == EINVAL || 
         errno == ERANGE ||
         /* check for range errors as well */
         *n < MIN_N || *n > MAX_N)
         {*n = -1;}
   }
   /* free up used resources */
   free(buf);
}

/* parse true/false choice */
int
parse_tf(char *msg, int *def) {
   size_t llen = 0;
   ssize_t alen = 0;
   int ret = 0;
   char *buf = NULL;
   /* parse until needed */
   while(alen != 2) {
      if(msg == NULL) 
         {printf("\n ** Select choice (y/n): ");}
      else 
         {printf("\n %s", msg);}
      alen = getline(&buf, &llen, stdin);
   }

   /* check what we got */
   if( buf[0] == 'y' || buf[0] == 'Y') 
      {ret = 1;} 
   else if( buf[0] == 'n' || buf[0] == 'N') 
      {ret = 0;} 
   else {
      printf("\n # Invalid option, reverting to default... (%s)\n", 
            *def ? "Y" : "N" ); 
      ret = *def;
   }
   /* free up resources */
   if(buf) {free(buf);}
   /* return the selection */
   return(ret);
}

/* allow for repeats */
int
allow_repeats() 
   {int def = 1; return(parse_tf("Allow repeats (y/n): ", &def));}

/* allow for repeats */
int
print_yn() 
   {int def = 0; return(parse_tf("Print number (y/n): ", &def));}

/* generate the number */
void
gen_num(int **num, int **tnum, const int *nlen, const int *reps) {
   int i = 0,
       j = 0,
       tn = 0;

   /* seed rand */
   srand(time(NULL));

   /* allocate required memory */
   if((*num = calloc(1, (*nlen)*sizeof(int))) == NULL)
         {return;}
   if((*tnum = calloc(1, (*nlen)*sizeof(int))) == NULL)
         {return;}

   /* generate the number */
   for(i = 0; i < *nlen; i++) {
      tn = (rand() % 9);
      /* special care for no-repeats option */ 
      if(*reps == 0) {
         while(1) {
            for(j = 0; j < i; j++) {
               /* have we got a repeat? */
               if((*num)[j] == tn) 
                  {tn = (rand() % 9); break;}
            }
            /* we don't have rep */
            if(j == i) {break;}
         }
      }

      (*num)[i] = tn;
   }
   /* print number if needed */
   if(print_yn()) {print_num(num, nlen);}
}

/* print required feedback */
int
print_feedback(const int *cor, const int *cor_p, const int *nlen) {
   if(*cor_p == *nlen)
      {printf("\nGreat, you found the secrent number!\n"); return(*cor_p);}
   else if(*cor_p > (*nlen)/2)
      {printf("\nMore than half found and in the right place!"); return(0);}
   else if(*cor >= (*nlen)/2) 
      {printf("\nMore than half found, not in the right place though"); return(0);}
   else //(*cor < (*nlen)/2) 
      {printf("\nLess than half found, not so good guess"); return(0);}
}

/* parse guess number for each iteration */
int
parse_num(int *cnt, char **buf, const int *num, int *tnum, 
          ssize_t *alen, size_t *llen, const int *nlen, const int *reps) {
   int i = 0, 
       j = 0, 
       p = 0,
       def = 1, 
       ret = 0,
       cor = 0,
       pcor = 0,
       cor_p = 0;
   extern int errno;
   char c; 
   *alen = 0;
   printf("\nEnter the number to guess: ");
   if(((*alen = getline(buf, llen, stdin)) == -1) || (((*alen)-1) != *nlen))
      {printf("\n** Error reading line (maybe mis-match in length?), try again"); return(ret);}
   else
      {(*cnt)++; printf("\n Parsed line %d and we read %ld characters\n", *cnt, ((*alen)-1));}

   /* now parse the number */ 
   for(i = 0; i < *nlen; i++) {
      errno = 0;
      c = (*buf)[i];
      tnum[i] = strtol(&c, NULL, 10);
      /* check for conversion errors */
      if(errno == EINVAL || 
         errno == ERANGE ||
         /* check for range errors as well */
         tnum[i] < 0 || tnum[i] > 9)
         {return(-1);}
      /* calculate the percentages as we go */
      pcor = cor; c = 0;
      for(j = 0, p = 0; j < *nlen; j++, p++) {
         /* for parameter calculation */
         if(num[j] == tnum[i]) 
            {if(cor == pcor) {cor++;} if(i == j) {cor_p++;}}
         /* for repeats */
         if(*reps == 0 && p <= i && tnum[p] == tnum[i]) {c++;} 
      } 
      /* check if we found repeats */
      if((*reps == 0) && (c > 1)) 
         {printf("\n Invalid number, repeats found c: %d, try again\n", c); return(ret);}
   }
   /* print the number */
   print_num(&tnum, nlen);
   /* print the required feedback */
   ret = print_feedback(&cor, &cor_p, nlen);
   /* check if we need to continue */
   if(ret == 0) {ret = !parse_tf("Continue (y/n): ", &def);}

   return(ret);
}

/* the guessing routine */
int
guess(int *num, int *tnum, const int *nlen, const int *reps) {
   int cnt = 0, ret = 0;
   char *buf = NULL,
        **tbuf = &buf;

   size_t llen = 0;
   ssize_t alen = 0;

   /* get the guess */
   while((ret = parse_num(&cnt, tbuf, num, tnum, &alen, &llen, nlen, reps)) == 0) 
      {;}
   printf("\nPlayer %s the secret number, used %d attempts", 
         ret > 1 ? "found" : "didn't find", cnt); 
   printf("\nSecret number is: ");
   print_num(&num, nlen);
   printf("\nYour number is: ");
   print_num(&tnum, nlen);
   if(buf) {free(buf);}
   return(0);
}

/* main stub */
int main(int argv, char **argc) {
   int n = -1,
       reps = 0,
       *num = NULL,
       *tnum = NULL,
       **ptnum = &tnum,
       **pnum = &num;
   greeting_msg();
   /* loop until we get a valid value */
   while(n == -1) {parse_n(&n);}
   printf(" !! Selected n is: %d\n", n);
   /* check the repeats option */
   reps = allow_repeats();
   printf("\n!! Repeats allowed: %s\n", reps ? "YES" : "NO");
   /* generate the number */
   gen_num(pnum, ptnum, &n, &reps);
   /* guess number */
   guess(num, tnum, &n, &reps);

   if(num) {free(num);}
   if(tnum) {free(tnum);}
   return(EXIT_SUCCESS);
}

