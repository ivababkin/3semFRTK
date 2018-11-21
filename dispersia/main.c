#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define DIGITS 100000000

struct OnePart
{
    int * data;
    int begin;
    int end;
    int sum;
    int average;
    float dispersion;//не деленная на количество элементов
};

typedef struct OnePart OnePart;

void *ThreadCountAverage(void *AcceptedPart)
{    
    OnePart ThisPart;
    ThisPart = *(OnePart*)AcceptedPart;

    int sum = 0;
    for (int i = ThisPart.begin; i <= ThisPart.end; i++)
    {
        sum = sum + ThisPart.data[i];
    }

    ThisPart.sum = sum;

    //printf("==ThisPart.begin = %d\n", ThisPart.begin);
    //printf("==ThisPart.end = %d\n", ThisPart.end);
    //printf("==ThisPart.data = %d\n", ThisPart.data);
    //printf("==ThisPart.sum = %d\n", ThisPart.sum);

    //printf("==ThisPart.end - ThisPart.begin = %d\n", ThisPart.end - ThisPart.begin);
    *(OnePart*)AcceptedPart = ThisPart;
    return NULL;
}

void *MyThreadCountDispersion(void *AcceptedPart)
{
    pthread_t mythid = pthread_self();

    OnePart ThisPart;
    ThisPart = *(OnePart*)AcceptedPart;

    int dispersion = 0;
    for (int i = ThisPart.begin; i <= ThisPart.end; i++)
    {
        ThisPart.dispersion = ThisPart.dispersion + (float)((ThisPart.data[i] - ThisPart.average) * (ThisPart.data[i] - ThisPart.average));
    }

    //printf("==ThisPart.begin = %d\n", ThisPart.begin);
    //printf("==ThisPart.end = %d\n", ThisPart.end);
    //printf("==ThisPart.data = %d\n", ThisPart.data);
    //printf("==ThisPart.sum = %d\n", ThisPart.sum);
    //printf("==ThisPart.dispersion = %f\n", ThisPart.dispersion);

    *(OnePart*)AcceptedPart = ThisPart;
    return NULL;
}




int main()
{
    float average = 0;
    float sum = 0;

    int * mass = (int*) malloc(DIGITS * sizeof(int));
    for (int i = 0; i < DIGITS; i++)
    {
        mass[i] = -1000 + rand()%2000;
        //printf("%d  ", mass[i]);
    }
    printf("\n");

    int NumberOfThr = 0;

    printf("input number of threads\n");
    scanf("%d", &NumberOfThr);

    OnePart * Parts = malloc(NumberOfThr * sizeof(OnePart));
    int LengthOfOnePart =DIGITS/NumberOfThr;

    for (int i = 0; i < NumberOfThr; i++)
    {
        Parts[i].begin = i * LengthOfOnePart;
        Parts[i].end = (i + 1) * LengthOfOnePart - 1;
        Parts[i].data = mass;
    }
    Parts[NumberOfThr - 1].end = DIGITS - 1;

    pthread_t * thids = malloc(NumberOfThr * sizeof(pthread_t));


    double start = time(NULL);

    for(int r = 0; r < 40; r++)
    {

        for (int i = 0; i < NumberOfThr; i++)
        {
            int res = pthread_create( &thids[i], (pthread_attr_t *)NULL, ThreadCountAverage, &Parts[i]);
        }

        for (int i = 0; i < NumberOfThr; i++)
        {
            pthread_join(thids[i], (void **)NULL);
        }
    }
    double end = time(NULL);

    printf("Took %lf seconds to execute it\n", difftime(end, start) / 40);


    /*for (int i = 0; i < NumberOfThr; i++)
    {
        printf("Parts[%d].begin = %d\n", i, Parts[i].begin);
        printf("Parts[%d].end = %d\n", i, Parts[i].end);
        printf("Parts[%d].data = %d\n", i, Parts[i].data);
        printf("Parts[%d].sum = %d\n", i, Parts[i].sum);
    }*/

    for (int i = 0; i < NumberOfThr; i++)
    {
        sum = sum + Parts[i].sum;
    }
    average = sum / DIGITS;

    printf("\n");
    //printf("sum = %f\n", sum);
    //printf("average = %f\n", average);

    for (int i = 0; i < NumberOfThr; i++)
    {
        Parts[i].average = average;
    }

    float sumdispersion = 0;

    for (int i = 0; i < NumberOfThr; i++)
    {
        int res = pthread_create( &thids[i], (pthread_attr_t *)NULL, MyThreadCountDispersion, &Parts[i]);
    }

    for (int i = 0; i < NumberOfThr; i++)
    {
        pthread_join(thids[i], (void **)NULL);
    }

    for (int i = 0; i < NumberOfThr; i++)
    {
        sumdispersion = sumdispersion + Parts[i].dispersion;
    }

    sumdispersion = sumdispersion / DIGITS;

    printf("          sumdispersion = %f", sumdispersion);
    printf("          average = %f\n", average);

    free(mass);
    return 0;
}
