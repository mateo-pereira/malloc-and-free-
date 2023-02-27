#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include "mymalloc.h"

int main(int argc, char *argv[])
{
    //Stores then immediately frees 120 1 byte chunks
    double average1;
    double total_in_seconds;
    int k;
    int i;
    for (k = 0;k < 50;k++)
    {
        clock_t start = clock();   
        for (i = 0;i < 120;i++)
        {  
            void *ptr = malloc(1);
            free(ptr);
        }

        clock_t end = clock();
        total_in_seconds = (end - start)/(double)CLOCKS_PER_SEC;
        //printf("This took %lf seconds to run for one_Time\n",total_in_seconds);
        average1 += total_in_seconds;
    }
    average1 = average1 / 50;
    printf("The average time for Test_One is %lf seconds\n",average1);
    
    
    //Stores 120 1-byte chunks, then frees them all
    double average2;
    double total_in_seconds2;
    int *pointersOne[120];
    for (k = 0;k < 50;k++)
    {
        clock_t start2 = clock();
        for (i = 0 ;i < 120;i++) 
            pointersOne[i] = malloc(1);

        for (i = 0 ;i < 120;i++)
            free(pointersOne[i]);

        clock_t end2 = clock();
        total_in_seconds2 = (end2 - start2)/(double)CLOCKS_PER_SEC;
        //printf("This took %lf seconds to run for two_Time\n",total_in_seconds2);
        average2 += total_in_seconds2;
    }
    average2 = average2 / 50;
    printf("The average time for Test_Two is %lf seconds\n",average2);

    //Randomly chooses between allocating a 1-byte chunk and deallocating a 1-byte chunk
    double average3;
    double total_in_seconds3;
    int *pointersTwo[120];
    for (k = 0;k < 50;k++)
    {
        clock_t start3 = clock();
        int mallocCount = 0;
        int freeCount = 0;
        int ptrCount = 0;
        int random;
        while (mallocCount < 120)
        {
            random = rand(); //Random num gen from 1-10

            if (random > 5) //If the number is 5 or over create a pointer in the array
            {
                pointersTwo[ptrCount] = malloc(1);
                mallocCount++;
                ptrCount++;
            }
            if (random <= 5) //If num is 5 or under deallocate, figure out how to pick a random num from 0 to count
            {
                if (freeCount < mallocCount)
                {
                    free(pointersTwo[ptrCount]);
                    freeCount++;
                    ptrCount--;
                }  
            }
        }
        for (i = 0;i < ptrCount;i++) //Frees the rest of the pointers
            free(pointersTwo[i]);

        clock_t end3 = clock();
        total_in_seconds3 = (end3 - start3)/(double)CLOCKS_PER_SEC;
        //printf("This took %lf seconds to run for Test_Three\n",total_in_seconds3);
        average3 += total_in_seconds3;
    }
    average3 = average3 / 50;
    printf("The average time for Test_Three is %lf seconds\n",average3);
    

    //Stress test #1 (make sure coalescing works multiple times)
    double average4;
    double total_in_seconds4;
    int *pointersThree[10];
    for (k = 0;k < 50;k++)
    {
        clock_t start4 = clock();
        for (i = 0;i < 10;i++)
            pointersThree[i] = malloc(1);

        free(pointersThree[8]);
        free(pointersThree[9]);
        pointersThree[8] = malloc(2);

        clock_t end4 = clock();
        total_in_seconds4 = (end4 - start4)/(double)CLOCKS_PER_SEC;
        //printf("This took %lf seconds to run for four_Time\n",total_in_seconds4);
        average4 += total_in_seconds4;
    }
    average4 = average4 / 50;
    printf("The average time for Test_Four is %lf seconds\n",average4);
    for (i = 0;i<8;i++) //frees up the remaining memory in pointersThree
        free(pointersThree[i]);

    //Stress test #2 (Fills up the entire memory to half capacity then frees it)
    double average5;
    double total_in_seconds5;
    int *pointersFour[128];
    for (k = 0;k < 50;k++)
    {
        clock_t start5 = clock();
        for (i = 0;i<128;i++)
            pointersFour[i] = malloc(29); //29 because 29 + 3 = 32, 32 * 128 = 4096

        for (i = 0;i<128;i++)
            free(pointersFour[i]);
        
        clock_t end5 = clock();
        total_in_seconds5 = (end5 - start5)/(double)CLOCKS_PER_SEC;
        //printf("This took %lf seconds to run for Five_Time\n",total_in_seconds5);
        average5 += total_in_seconds5;
    }
    average5 = average5 / 50;
    printf("The average time for Test_Five is %lf seconds\n",average5);
}

