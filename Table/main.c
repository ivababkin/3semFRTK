#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define TABLE_LIMIT 2
#define MAX_LENGTH_OF_TYPE 10
#define MAX_LENGTH_OF_STRING 15
#define BUF 20

typedef struct Thing thing;

int MaxNumberOfTypes = 10;
int MaxNumberOfThings = 20;


struct Thing
{
    char * type;
    int timeOfCleaning;
    int timeOfDrying;
};

int semaforCreate()
{
    int semid;
    char filename[] = "/home/ivan/Рабочий стол/фртк/all/Table/Table/keyfile";
    key_t key;

    key = ftok(filename, 0);

    if((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0){
        printf("Can\'t create semaphore set\n");
        exit(-1);
    }

    return semid;
}

void operate(int semid, int op)
{
    struct sembuf mybuf;
    mybuf.sem_op  = 0;
    mybuf.sem_op  = op;
    mybuf.sem_num  = 0;
    mybuf.sem_flg = 0;
    if(semop(semid, &mybuf, 1) < 0)
    {
        printf("Can\'t wait for condition\n");
        exit(-1);
    }
}

void lockSem(int semid)
{
    operate(semid, -1);
}

void unlockSem(int semid)
{
    operate(semid, 1);
}

thing * cleateMassOfThingsAndFillTimesOfCleaning()
{
    char sep[2] = " ";
    FILE * fTimesOfCleaning = fopen("/home/ivan/Рабочий стол/фртк/all/Table/Table/timesofcleaning", "r");
    thing * massOfThings = malloc(MaxNumberOfTypes * sizeof(thing));


    for(int i = 0; i < MaxNumberOfTypes; i++)
    {
        char * strtemp = malloc(MAX_LENGTH_OF_STRING * sizeof(char));
        fgets(strtemp, MAX_LENGTH_OF_STRING, fTimesOfCleaning);
        strtok(strtemp, sep);
        massOfThings[i].type = strtemp;
        strtok(NULL, sep);
        massOfThings[i].timeOfCleaning = ((int)(*strtok(NULL, sep)) - 48);
    }
    fclose(fTimesOfCleaning);

    return massOfThings;
}

void deleteMassToClean(char ** massOfThings)
{
    /*for(int i = 0; i < MaxNumberOfThings; i++)
    {
        printf("deletemass[%d] = %s\n", i, massOfThings[i]);
        if(massOfThings[i] != NULL)
            //free((char *)massOfThings[i]);
    }*/
    if (massOfThings != NULL)
        free(massOfThings);
}

void FillTimesOfDrying(thing * massOfThings)
{
    char sep[2] = " ";
    FILE * fTimesOfDrying = fopen("/home/ivan/Рабочий стол/фртк/all/Table/Table/timesofdrying", "r");

    char strtemp[MAX_LENGTH_OF_STRING];
    for(int i = 0; i < MaxNumberOfTypes; i++)
    {
        fgets(strtemp, MAX_LENGTH_OF_STRING, fTimesOfDrying);
        char * typeTemp = strtok(strtemp, sep);
        strtok(NULL, sep);
        int timeOfDryingTemp = (int)(*strtok(NULL, sep));
        for(int i = 0; i < MaxNumberOfTypes; i++)
        {
            if(strcmp(massOfThings[i].type, typeTemp) == 0)
            {
                massOfThings[i].timeOfDrying = timeOfDryingTemp - 48;
            }

        }
    }
    fclose(fTimesOfDrying);
}

void dry(thing * massOfThings, char * typeTemp)
{
    int timeOfDryingTemp = 0;
    for(int i = 0; i < MaxNumberOfTypes; i++)
    {
        if(strcmp(massOfThings[i].type, typeTemp) == 0)
        {
             timeOfDryingTemp = massOfThings[i].timeOfDrying;
        }
    }
    sleep(timeOfDryingTemp);    
    printf("dryed %s %d sec\n", typeTemp, timeOfDryingTemp);
}

void clean(thing * massOfThings, char * typeTemp)
{
    int timeOfCleaningTemp = 0;
    for(int i = 0; i < MaxNumberOfTypes; i++)
    {
        if(strcmp(massOfThings[i].type, typeTemp) == 0)
        {
             timeOfCleaningTemp = massOfThings[i].timeOfCleaning;
        }
    }

    sleep(timeOfCleaningTemp);
    printf("cleaned %s %d sec\n", typeTemp, timeOfCleaningTemp);
}

char ** createMassToClean()
{
    char sep[2] = " ";
    FILE * fNumber = fopen("/home/ivan/Рабочий стол/фртк/all/Table/Table/number", "r");
    char ** massOfThingsToClean = malloc(MaxNumberOfThings * sizeof(char*));

    int count = 0;
    while(count < MaxNumberOfThings)
    {
        char * strtemp = malloc(MAX_LENGTH_OF_STRING * sizeof(char));
        if(fgets(strtemp, MAX_LENGTH_OF_STRING, fNumber) == NULL)
        {
            fclose(fNumber);
            return massOfThingsToClean;
        }
        strtok(strtemp, sep);
        massOfThingsToClean[count] = strtemp;
        strtok(NULL, sep);
        int i = ((int)(*strtok(NULL, sep)) - 48);

        for (int k = 1; k <= i; k++)
        {
            count += 1;
            massOfThingsToClean[count] = strtemp;
        }
    }
    fclose(fNumber);
    return massOfThingsToClean;
}

void childClean(thing * myMassOfThings, int fd, int * thingsOnTheTable, int semid)
{
    char ** massOfThingsToClean;
    massOfThingsToClean = createMassToClean();

    printf("\n");
    printf("queue to be cleaned\n");
    for(int i = 0; i < MaxNumberOfThings; i++)
    {
        printf("%s\n", massOfThingsToClean[i]);
    }
    printf("\n");

    int i = 0;

    unlockSem(semid);

    while(i < MaxNumberOfThings)
    {
        if ((*thingsOnTheTable) <= TABLE_LIMIT)
        {
            clean(myMassOfThings, massOfThingsToClean[i]);
            lockSem(semid);
            write(fd, massOfThingsToClean[i], BUF);
            printf("put %s\n", massOfThingsToClean[i]);
            (*thingsOnTheTable)++;
            unlockSem(semid);
            printf("       ThingsOnTheTable %d\n", *thingsOnTheTable);
            i++;
        }
    }
    char endMassege[] = "all";
    write(fd, endMassege, BUF);
    printf("have cleaned all things\n");
    deleteMassToClean(massOfThingsToClean);
}

void perentDry(thing * myMassOfThings, int fd, int* thingsOnTheTable, int semid)
{
    char takenThing[MAX_LENGTH_OF_TYPE];
    char endMassege[] = "all";
    while(strcmp(takenThing, endMassege) != 0)
    {

        if(read(fd, takenThing, BUF) < 0)
            printf("read crashed");

        if(strcmp(takenThing, endMassege) == 0)
        {
            printf("have dryed all things\n");
            exit(-1);
        }

        printf("took %s\n", takenThing);


        lockSem(semid);
        (*thingsOnTheTable)--;
        unlockSem(semid);
        dry(myMassOfThings, takenThing);
    }
    printf("finish\n");
}

int * createShearedMemory()
{
    int * ptrRes;
    int key;

    if((key = ftok("keyfile", 1)) < 0)
    {
        printf("Can\'t generate key\n");
        exit(-1);
    }

    int shmid;
    if((shmid = shmget(key, sizeof(int), 0666|IPC_CREAT)) < 0)
    {
            printf("Can\'t create shared memory\n");
            exit(-1);
    }

    if((ptrRes = (int *)shmat(shmid, NULL, 0)) == (int *)(-1))
    {
        printf("Can't attach shared memory\n");
        exit(-1);
    }
    *ptrRes = 0;
    return ptrRes;
}

void getNumberOfTypes(int * number)
{
    FILE * fTimesOfCleaning = fopen("/home/ivan/Рабочий стол/фртк/all/Table/Table/timesofcleaning", "r");
    int res = 0;

    char strtemp[MAX_LENGTH_OF_STRING];
    for(int i = 0; i < *number; i++)
    {
        if(fgets(strtemp, MAX_LENGTH_OF_STRING, fTimesOfCleaning) != NULL)
            res++;
    }
    fclose(fTimesOfCleaning);
    *number = res;
}

void getMaxNumberOfThings(int * number)
{
    char sep[] = " ";
    int res = 0;
    FILE * fNumber = fopen("/home/ivan/Рабочий стол/фртк/all/Table/Table/number", "r");

    char strtemp[MAX_LENGTH_OF_STRING];
    for(int i = 0; i < *number; i++)
    {
        if(fgets(strtemp, MAX_LENGTH_OF_STRING, fNumber) == NULL)
        {
            fclose(fNumber);
            break;
        }
        strtok(strtemp, sep);
        strtok(NULL, sep);
        res += ((int)(*strtok(NULL, sep)) - 48);
    }
    *number = res;
    fclose(fNumber);

}

int main()
{
    getNumberOfTypes(&MaxNumberOfTypes);
    printf("MaxNumberOfTypes = %d\n", MaxNumberOfTypes);

    getMaxNumberOfThings(&MaxNumberOfThings);
    printf("MaxNumberOfThings = %d\n", MaxNumberOfThings);


    thing * myMassOfThings = cleateMassOfThingsAndFillTimesOfCleaning();
    FillTimesOfDrying(myMassOfThings);

    printf("FILE:\n");
    for(int i = 0; i < MaxNumberOfTypes; i++)
    {
        printf("[%d]%s clean:%d dry:%d \n", i, myMassOfThings[i].type, myMassOfThings[i].timeOfCleaning,
               myMassOfThings[i].timeOfDrying);
    }


    int fd[2];
    int pid = -1;

    if(pipe(fd) < 0)
    {
        printf("Can\'t open pipe\n");
        exit(-1);
    }
    int* ptrRes = createShearedMemory();
    int semid = semaforCreate();

    pid = fork();
    if (pid == 0)
    {//родитель вытирает
        perentDry(myMassOfThings, fd[0], ptrRes, semid);
    }
    else
    {//ребенок моет
        childClean(myMassOfThings, fd[1], ptrRes, semid);
    }

    //free(myMassOfThings);//щто?


    close(fd[0]);
    close(fd[1]);

    return 0;
}
