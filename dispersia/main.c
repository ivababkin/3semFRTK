#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MNOGO 100000000

struct OnePart
{
    int * data;
    int begin;
    int end;
    int sum;
    int middle;
    float dispersia;
};

typedef struct OnePart onepart;

void *mythread(void *dummy)
{
    pthread_t mythid = pthread_self();

    onepart part;
    part = *(onepart*)dummy;

    int summ = 0;
    for (int i = part.begin; i <= part.end; i++)
    {
        summ = summ + part.data[i];
    }

    part.sum = summ;

    //printf("==part.begin = %d\n", part.begin);
    //printf("==part.end = %d\n", part.end);
    //printf("==part.data = %d\n", part.data);
    //printf("==part.sum = %d\n", part.sum);

    //printf("==part.end - part.begin = %d\n", part.end - part.begin);
    *(onepart*)dummy = part;
    return NULL;
}

void *mythreadcountdispersia(void *dummy)
{
    pthread_t mythid = pthread_self();

    onepart part;
    part = *(onepart*)dummy;

    int dispersia = 0;
    for (int i = part.begin; i <= part.end; i++)
    {
        part.dispersia = (float)((part.data[i] - part.middle) * (part.data[i] - part.middle)) / (float)(part.end - part.begin + 1);
    }

    //printf("==part.begin = %d\n", part.begin);
    //printf("==part.end = %d\n", part.end);
    //printf("==part.data = %d\n", part.data);
    //printf("==part.sum = %d\n", part.sum);
    //printf("==part.dispersia = %f\n", part.dispersia);

    *(onepart*)dummy = part;
    return NULL;
}




int main()
{
    float middle = 0;
    float sum = 0;

    int * mass = (int*) malloc(MNOGO * sizeof(int));
    for (int i = 0; i < MNOGO; i++)
    {
        mass[i] = -10 + rand()%20;
        //printf("%d  ", mass[i]);
    }
    printf("\n");

    int numberofthr = 0;

    printf("input numberofthreads\n");
    scanf("%d", &numberofthr);

    onepart * parts = malloc(numberofthr * sizeof(onepart));
    int lengthofonepart =MNOGO/numberofthr;

    for (int i = 0; i < numberofthr; i++)
    {
        parts[i].begin = i * lengthofonepart;
        parts[i].end = (i + 1) * lengthofonepart - 1;
        parts[i].data = mass;
    }
    parts[numberofthr - 1].end = MNOGO - 1;

    pthread_t * thids = malloc(numberofthr * sizeof(pthread_t));


    clock_t begin = clock();

    for (int i = 0; i < numberofthr; i++)
    {
        int res = pthread_create( &thids[i], (pthread_attr_t *)NULL, mythread, &parts[i]);
    }
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("time_spent = %lf", time_spent * 1000);

    for (int i = 0; i < numberofthr; i++)
    {
        pthread_join(thids[i], (void **)NULL);
    }



    /*for (int i = 0; i < numberofthr; i++)
    {
        printf("parts[%d].begin = %d\n", i, parts[i].begin);
        printf("parts[%d].end = %d\n", i, parts[i].end);
        printf("parts[%d].data = %d\n", i, parts[i].data);
        printf("parts[%d].sum = %d\n", i, parts[i].sum);
    }*/

    for (int i = 0; i < numberofthr; i++)
    {
        sum = sum + parts[i].sum;
    }
    middle = sum / MNOGO;

    printf("\n");
    //printf("sum = %f\n", sum);
    //printf("middle = %f\n", middle);

    for (int i = 0; i < numberofthr; i++)
    {
        parts[i].middle = middle;
    }

    float sumdispersia = 0;

    for (int i = 0; i < numberofthr; i++)
    {
        int res = pthread_create( &thids[i], (pthread_attr_t *)NULL, mythreadcountdispersia, &parts[i]);
    }

    for (int i = 0; i < numberofthr; i++)
    {
        pthread_join(thids[i], (void **)NULL);
    }

    for (int i = 0; i < numberofthr; i++)
    {
        sumdispersia = sumdispersia + parts[i].dispersia;
    }

    printf("          sumdispersia = %f", sumdispersia);

    return 0;
}

/*int msec = 0, trigger = 10;
clock_t before = clock();

do {

    Do something to busy the CPU just here while you drink a coffee
    Be sure this code will not take more than `trigger` ms


  clock_t difference = clock() - before;
  msec = difference * 1000 / CLOCKS_PER_SEC;
  iterations++;
} while ( msec < trigger );

printf("Time taken %d seconds %d milliseconds (%d iterations)\n",
  msec/1000, msec%1000, iterations);
*/


