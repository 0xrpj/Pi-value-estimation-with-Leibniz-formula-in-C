/*
    Author: Roshan Parajuli
    Application: Pi value estimation with Leibniz formula
*/

#include <stdio.h>
#include <math.h>
#include <pthread.h>

double sum = 0;
int threads;
pthread_mutex_t mutex;

//structure for the starting and ending position of iterations
struct iter
{
    int start;
    int end;
};

void *Leibniz(void *vars)
{
    struct iter *values = (struct iter *)vars;
    int start = values->start;
    int end = values->end;

    int i;
    double local_sum = 0;

    for (i = start; i <= end; i++)
    {
        local_sum += (pow(-1, i) / ((2 * i) + 1));
    }

    //Prevention of race condition
    pthread_mutex_lock(&mutex);
    sum += local_sum;
    pthread_mutex_unlock(&mutex);

    pthread_exit(0);
}

void main()
{
    long long iterations;

    system("COLOR A");
    printf(" How many iterations do you want? \t Note: More iterations results to more accurate pi value estimation!\n\n ");
    scanf("%lld", &iterations);

    printf("\n\n Enter the number of threads: ");
    scanf("%d", &threads);

    //Slicing the iterations value to equally distribute between all threads.
    int sliceList[threads];
    int remainder = iterations % threads;

    int i;
    for (i = 0; i < threads; i++)
    {
        sliceList[i] = iterations / threads;
    }

    int j;
    for (j = 0; j < remainder; j++)
    {
        sliceList[j] += 1;
    }

    int startList[threads];
    int endList[threads];

    int l;

    for (l = 0; l < threads; l++)
    {
        if (l == 0)
            startList[l] = 0;
        else
            startList[l] = endList[l - 1] + 1;

        endList[l] = startList[l] + sliceList[l] - 1;
    }

    //Making an array of structures.
    struct iter arr[threads];

    int k;
    for (k = 0; k < threads; k++)
    {
        arr[k].start = startList[k];
        arr[k].end = endList[k];
    }

    //Initializing mutex for its usage in the multithreaded function.
    pthread_mutex_init(&mutex, NULL);

    pthread_t threadId[threads];

    //Creating threads and joining them to let every thread finish its job.

    int m;
    for (m = 0; m < threads; m++)
    {
        pthread_create(&threadId[m], NULL, Leibniz, &arr[m]);
    }

    int n;
    for (n = 0; n < threads; n++)
    {
        pthread_join(threadId[n], NULL);
    }

    printf("\n The approximate value of pi is calculated to be %.10f\n", 4 * sum);
}
