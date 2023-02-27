#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

// For Correctness Testing 4
void printct4(char *, int);
void fillct4(char *, int, char);

int main(void)
{
	
	//int *ptr[4096];
	int *p, *q;
	char *o, *l, *t, *u;
	int i;
	
	// -----------------------------------------------------
	// Correctness Testing 1
	// tests if malloc allocates and reserves space
	// uses and frees p
	printf("Correctness Testing 1: \n\n");
	p = (int*) malloc(sizeof(int) * 5);
	
	for(i = 0; i < 5; i++)
	{
            	p[i] = i + 1;
        }
        
        // Print the elements of the array
        printf("The elements of the array are: ");
        for (i = 0; i < 5; ++i) {
            printf("%d, ", p[i]);
        }
	free(p);
	printf("\n------------------------------------------\n");
	
	// -------------------------------------------------------------------------
	// Correctness testing # 2 when successful malloc returns pointer to an 
	// object not overlaping to any other allocated object 
	// uses p and q and then frees both
	
	printf("Correctness Testing 2: \n\n");
	p = (int*) malloc(sizeof(int)*10);
	q = (int*) malloc(sizeof(int)*10);
	
	// for loop goes through p sets all values of p to 1 and q to 2
	for(i = 0; i < 10; i++)
	{	p[i] = 1;  q[i] = 2;	}
	
	// free p
	free(p);
	p = (int*) malloc(sizeof(int)*12); // remallocs p into a bigger size;
	
	
	for(i = 0; i < 12; i++)
	{	p[i] = 3;	}
	
	for(i = 0; i < 10; i++)
	{
		printf("%d:%d ", i, q[i]);
	}
	printf("\n");
	
	for(i = 0; i < 12; i++)
	{
		printf("%d:%d ", i, p[i]);
	}
	puts("\n");
	
	
	// testing if the freed memory from free(p) can be used to allocate memory.
	int *firstchunk = (int*) malloc(sizeof(int)*10); // if it works should take the place of the first freed chunk
	for(i = 0; i < 10; i++) { firstchunk[i] = 4; }
	for(i = 0; i < 35; i++) { printf("%d:%d ", i, firstchunk[i]); } // this goes past the allocated space of the first chunk to see if its the first chunk
	puts("\n"); // at indexes 12-> 34 it has memory stored.
	
	free(p);
	free(q);
	free(firstchunk);
	
	// -----------------------------------------------------------
	// Correctness Testing 3
	// Testing if freecoalescing works
	// uses p q l t, p and q are boreders while q and t are going to be used to split 200 bytes
	// both are gonna be freed, then q malloced again for the whole space.
	
	printf("Correctness Testing 3: \n\n");
	
	// total metadata would be 12
	u = (char*) malloc(1941); // -4 -> to account for matadata
	o = (char*) malloc(99);
	t = (char*) malloc(99);
	l = (char*) malloc(1941); // -5 -> to account for meta data
	
	free(o);
	free(t);
	
	o = (char*) malloc(250);
	free(u);
	free(l);
	free(o);
	
	
	printf("\n------------------------------------------\n");
	
	
	printf("Correctness Testing 4: \n\n");
	// doing the correctness test # 3 on the assignment writeup
	u = (char*) malloc(200); 
	o = (char*) malloc(300);
	l = (char*) malloc(500); 
	t = (char*) malloc(1000);
	
	fillct4(u, 200, 'u'); // fillmalloc correctness testing 4
	fillct4(o, 300, 'o');
	fillct4(l, 500, 'l');
	fillct4(t, 1000, 't');
	
	printct4(u, 200); // print correctness testing 4
	printct4(o, 300);
	printct4(l, 500);
	printct4(t, 1000);
	
	printf("\n------------------------------------------\n");
	
	// error testing 1
	printf("\nError Testing 1: \n\n");
	int x;
	free(&x);
	// sends error because address isnt a malloced address
	printf("\n------------------------------------------\n");
	
	// error testing 2
	printf("\nError Testing 2: \n\n");
	p = (int*)malloc(sizeof(int)*2);
	free(p + 1);
	// sends an error because invalid pointer
	printf("\n------------------------------------------\n");
	
	// error testing 3
	printf("\nError Testing 3: \n\n");
	p = (int*)malloc(sizeof(int)*100);
	q = p;
	free(p);
	free(q);
	// sends an error message because tries freeing a ptr that points to a spot in memory that was already freed
	printf("\n------------------------------------------\n");
	
	return EXIT_SUCCESS;
}

void printct4(char *ptr, int n)
{
	int i;
	for(i = 0; i < n; i++)
	{
		printf("%c ", ptr[i]);
	}
	printf("\n\n");
	return;
}
void fillct4(char *ptr, int n, char letter)
{
	int i;
	
	for(i = 0; i < n; i++)
	{
		ptr[i] = letter;
	}
}

