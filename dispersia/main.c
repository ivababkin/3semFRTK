#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MNOGO 1000

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

    printf("==part.begin = %d\n", part.begin);
    printf("==part.end = %d\n", part.end);
    printf("==part.data = %d\n", part.data);
    printf("==part.sum = %d\n", part.sum);

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

    printf("==part.begin = %d\n", part.begin);
    printf("==part.end = %d\n", part.end);
    printf("==part.data = %d\n", part.data);
    printf("==part.sum = %d\n", part.sum);
    printf("==part.dispersia = %f\n", part.dispersia);

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
        printf("%d  ", mass[i]);
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

    for (int i = 0; i < numberofthr; i++)
    {
        int res = pthread_create( &thids[i], (pthread_attr_t *)NULL, mythread, &parts[i]);
    }



    for (int i = 0; i < numberofthr; i++)
    {
        pthread_join(thids[i], (void **)NULL);
    }



    for (int i = 0; i < numberofthr; i++)
    {
        printf("parts[%d].begin = %d\n", i, parts[i].begin);
        printf("parts[%d].end = %d\n", i, parts[i].end);
        printf("parts[%d].data = %d\n", i, parts[i].data);
        printf("parts[%d].sum = %d\n", i, parts[i].sum);
    }

    for (int i = 0; i < numberofthr; i++)
    {
        sum = sum + parts[i].sum;
    }
    middle = sum / MNOGO;

    printf("\n");
    printf("sum = %f\n", sum);
    printf("middle = %f\n", middle);

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






/*#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int mass[3] = {0, 0, 0};

void *mythread(void *dummy)

{

    pthread_t mythid;

    mythid = pthread_self();

    int *i = (int*)dummy;

    for(int j = 0; j < 10000000; j++)
    {
        mass[*i]++;
    }

    for(int j = 0; j < 10000000; j++)
    {
        mass[*i + 1]++;
    }



   return NULL;
}

int main()
{
   pthread_t thid1, thid2, mythid;
   int       result;

   int a;
   int b;

   a = 0;
   b = 1;

   result = pthread_create( &thid1, (pthread_attr_t *)NULL, mythread, &a);


   printf("Thread created, thid = %lu\n", thid1);

   result = pthread_create( &thid2, (pthread_attr_t *)NULL, mythread, &b);


   printf("Thread created, thid = %lu\n", thid2);

   mythid = pthread_self();



   pthread_join(thid1, (void **)NULL);
   pthread_join(thid2, (void **)NULL);
   printf("mass[0] = %d\n", mass[0]);
   printf("mass[1] = %d\n", mass[1]);
   printf("mass[2] = %d\n", mass[2]);


   return 0;
}
*/
