#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"


#define MEMSIZE 4096
#define METASIZE 3

static char memory[MEMSIZE];
static void collRight(int);

void *mymalloc(size_t size, char *file, int line)
{
	
	int i; // ptr for spot in memory
	// malloc(0) error
	if(size == 0) // when size = 0, malloc(0) -> s a NULL pointer
	{
		printf("Error Tried to allocate 0 bytes of memory (%s:l%d)\n", file, line);
		return NULL;
	}
	
	// if asked to do more than alloted size of memory instant errors, returns -1
	if(size > MEMSIZE - METASIZE) // size > 4096 - 3 // fails and returns error
	{ 
		printf("Failed to find space in memory, (%s:l%d)\n", file, line);
	       	return NULL;
	}
	
	i = 0;
	
	// traverses from 0 -> 4092 (since metadata is 3 bytes and needs 1 more for data)
	while(i < MEMSIZE - METASIZE - 2) 
	{
		void *ptr;
		int end, rsize, nextBlock, blocksize;
		int free = (int)memory[i];
		int quotient = (int) memory[i+1];
		int remainder = (int) memory[i+2];
		
		// adds 256 to quotient if negative. this is because char is ranged from -128 to 127 
		// because char goes from 0->127 then -128 -> -1 (0->127 then 128->255)
		if(quotient < 0){	quotient += 256; 	}
		if(remainder < 0){ 	remainder += 256;	}
		blocksize = quotient * 255 + remainder;	
		
		
		if(blocksize == 0) // So that if its the first time for malloc 
		{
			blocksize = MEMSIZE - METASIZE; // block size 4096-3 = 4093 = 4093 // total data size 
			end = MEMSIZE; // index at the last data of the block
			// start index of next block
			nextBlock = i + METASIZE + size; // index for next block
			
			if(end - nextBlock >= 4) // if there is space to make another Block it will otherwise it will otherwise it adds to this used Block
			{
				memory[i+1] = size / 255; // quotient
				memory[i+2] = size % 255; // remainder
				memory[i] = 1; // sets Block to not free
				rsize = blocksize-size-METASIZE; // remaining size
				
				memory[nextBlock] = 0;
				memory[nextBlock+1] = rsize/255;
		       	memory[nextBlock+2] = rsize%255;
				ptr = &memory[i+METASIZE];
				
				return ptr; // returning void ptr of the start of the data 	
			}
			
			else  // if there is only less than 4 bytes remaining in the Block it gets added into the allocated Block
			{
				size = blocksize;
				
				memory[i] = 1; // sets Block to not free
				memory[i+1] = size / 255; // quotient
				memory[i+2] = size % 255; // remainder
				ptr = &memory[i+METASIZE];
				
			}
			
		}

		// Normal run through so it would check this current block
		if(free == 1) // not free
		{
			i += (blocksize + METASIZE); // 
			
		}
		else
		{
		
		
			if(blocksize < size)
			{
				i += (blocksize+METASIZE);
				
				
			}
			else
			{
			
				end = i + METASIZE + blocksize; // index at the last data of the block
				// start index of next block
				nextBlock = i + METASIZE + size;
				if(end - nextBlock >= 4) // if there is space to make another Block it will otherwise it will otherwise it adds to this used Block
				{
					memory[i] = 1; // sets Block to not free
					memory[i+1] = size / 255; // quotient
					memory[i+2] = size % 255; // remainder
					rsize = blocksize-size -METASIZE; // needed size
					
					memory[nextBlock] = 0;
					memory[nextBlock+1] = rsize/255;
			       	memory[nextBlock+2] = rsize%255;
					ptr = &memory[i+METASIZE];
					
					return ptr; // returning void ptr of the start of the data 	
				}	
				else
				{
					size = blocksize; 
					
					memory[i+1] = size / 255; // quotient
					memory[i+2] = size % 255; // remainder
					memory[i] = 1; // sets Block to not free
					ptr = &memory[i+METASIZE];
					
					return ptr; // returning void ptr 	
				}
						    
			}
			
		}
	}
	
	printf("Not enough space in memory(%s:l%d)\n", file, line);
       	return NULL;
}



void myfree(void *ptr, char *file, int line)
{
	// address
	// invalid pointer
	// double free 
	int i = 0;
	int nextblock;
	//void *start = ptr-METASIZE; // find start index from Metadata
	
	// NULL pointer
	if(ptr == NULL) return;

	if(&memory[3] == ptr) // if first block is ptr
	{
		if (memory[i] == 0)
		{	
			printf("Invalid Pointer (%s:l%d)\n", file, line);
			return;
		}
		memory[i] = 0; // sets to free
		
		collRight(i);
		return;
	}
	
	while(i < MEMSIZE - METASIZE - 2)
	{
		 
		int quotient = (int) memory[i+1];
		int remainder = (int) memory[i+2];
		if(quotient < 0){	quotient += 256; 	}
		if(remainder < 0){ 	remainder += 256;	}
		
		int blocksize = quotient * 255 + remainder;
		nextblock = i + blocksize + METASIZE; // index of next block
		if(nextblock > MEMSIZE)
		{
			printf("Invalid Pointer huh%d(%s:l%d)\n",i, file, line);
			return;
		}
		else if (&memory[nextblock + METASIZE] == ptr)
		{
			if(memory[nextblock] == 1)
			{
				memory[nextblock] = 0;
				
				collRight(nextblock);
				if(memory[i] == 0)
					collRight(i);
				return;
			}
			else 
			{
				printf("Invalid Pointer t(%s:l%d)\n", file, line);
				return;
			}
		}
		else
		{
			i = nextblock;
			//printf("skip\n");
			
		}
		
	}
	// needto coalese	
		
	printf("Invalid Pointer a(%s:l%d)\n", file, line);
}	

static void collRight(int index)
{
	int nextblock, blocksize, prevblocksize, totalsize;
	int quotient = (int) memory[index+1];
	int remainder = (int) memory[index+2];
	if(quotient < 0){	quotient += 256; 	}
	if(remainder < 0){ 	remainder += 256;	}
	blocksize = quotient * 255 + remainder;
	prevblocksize = blocksize + METASIZE;
	
	nextblock = index + blocksize + METASIZE; // index of the start of metadata for next block
	if(nextblock < MEMSIZE - METASIZE - 2 && memory[nextblock] == 0)
	{
		quotient = (int) memory[nextblock+1];
		remainder = (int) memory[nextblock+2];
		if(quotient < 0){	quotient += 256; 	}
		if(remainder < 0){ 	remainder += 256;	}
		blocksize = quotient * 255 + remainder;
		totalsize = prevblocksize + blocksize;
		
		memory[index+1] = totalsize/255;
		memory[index+2] = totalsize%255;
		
	}
	return;
}