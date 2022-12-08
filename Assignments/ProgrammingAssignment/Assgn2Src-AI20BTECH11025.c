#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define MAX_ARR_SIZE 50000

// Global array
int globarr[MAX_ARR_SIZE];

// Struct for argument for each thread
struct item
{
    int start; // Start number for each thread
    int itemsize; // Number of integers assigned to each thread
    int threadnum; // Index of the thread
};

// Function to find all the perfect numbers in the given range of start number - (start number + itemsize)
void* find_perfect_num(void* arg)
{
    int p = ((struct item *)arg)->start;
    int r = p + ((struct item *)arg)->itemsize;
    for(int num = p; num < r; num++)
    {
        int sum = 0;
        for(int i = 1; i < num; i++)
        {
            if(num%i == 0) sum += i;
        }
        if(sum == num)
        {
            globarr[num] = 1;
        }
    }


    // Writing to the output log file
    char threadfile[20] = "OutFile";
    char threadnumchar[5];
    sprintf(threadnumchar, "%d", ((struct item *)arg)->threadnum);
    strcat(threadfile, threadnumchar);
    strcat(threadfile, ".log");
    FILE *fp;
    fp  = fopen(threadfile, "w");
    
    for(int num = p; num < r; num++)
    {
        fprintf(fp, "%d ", num);
        if(globarr[num] == 1) fprintf(fp, "%s", "Is a perfect number\n");
        else fprintf(fp, "%s", "Not a perfect number\n");
    }
    
    fclose(fp);
    
    return NULL;
}

int main()
{
    // n = number, k = num of threads
    int n, k;

    // Input file to be named as 'input.txt'
    char filename[20] = "input.txt";
  
    // printf("Enter the file name:");
    // scanf("%s", &filename);
    
    FILE *ptr = fopen(filename, "r");
    if(ptr == NULL)
    {
        printf("FILE DOES NOT EXISTS\n");
        return EXIT_FAILURE;
    }

    // Reading the file
    fscanf(ptr, "%d", &n);
    fscanf(ptr, "%d", &k);

    // Creating an array of threads
    pthread_t threads_array[k];
  
    // Creating an array of arguments for the threads
    struct item *arguments[k];
  
    // Allocating size to the arguments
    for(int i = 0; i < k; i++)
    {
        arguments[i] = malloc(sizeof(struct item)); 
    }

    // Partitioning integers 1-N among k threads
    // The first integer and the number of integers for each thread is put in the argument struct
    for(int i = 0; i < k; i++)
    {
        arguments[i]->start = ((ceil(n/(k*1.0))) * i) + 1;
        arguments[i]->itemsize = (ceil(n/(k*1.0)));
        if(i == k-1 && n%k > 0) arguments[i]->itemsize = n - (ceil(n/(k*1.0)))*(k-1);
        arguments[i]->threadnum = i+1;

        // Looping through and creating multiple threads
        pthread_create(&threads_array[i], NULL, find_perfect_num, arguments[i]);
    }
  
    //Joining all the threads after they get completed
    for(int i = 0; i < k; i++)
    {
        pthread_join(threads_array[i], NULL);
    }
  
    // Writing the output to a log file
    char threadfile[20] = "OutMain";
    strcat(threadfile, ".log");
    FILE *fp;
    fp  = fopen(threadfile, "w");

    for(int i = 0; i < k; i++)
    {
        fprintf(fp, "%s", "Thread ");
        char threadnumchar[5];
        sprintf(threadnumchar, "%d", arguments[i]->threadnum);
        fprintf(fp, "%s: ", threadnumchar);
        int p = arguments[i]->start;
        int r = arguments[i]->itemsize + p;
        for(int num = p; num < r; num++)
        {
            if(globarr[num] == 1) fprintf(fp, "%d ", num);
        }
        fprintf(fp, "\n");
    }
    
    fclose(fp);
}