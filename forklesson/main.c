#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#define LENGTH_OF_STRING 20


void Split (char* string, char* delimeters, char*** tokens, int* tokensCount)//оно портит исходную строку, но так, вроде, можно
{
    int count = 0;
    char * ptrstr;
    char ** doubleptr = *tokens;
    ptrstr = strtok (string, delimeters);
    *doubleptr = ptrstr;

    while (ptrstr != NULL)
    {
       (doubleptr)++;
       ptrstr = strtok (NULL, delimeters);
       *doubleptr = ptrstr;
       count++;
    }
    doubleptr++;
    doubleptr = NULL;
    *tokensCount = count;

}

int compare(const void * s1, const void * s2)
{
    int res = ((**((char**)s1)) - (**((char**)s2)));
    return res;
}




int main()
{
    pid_t a;
    pid_t pid = 2;


    char delimeters[] = {" '\n'"};



    FILE * f = fopen("/home/ivan/Рабочий стол/фртк/all/forklast/forklast/text", "r");


    int NumberOfStrings = 0;
    int TimeBeforeKill = 0;
    fscanf(f, "%d\n", &NumberOfStrings);
    fscanf(f, "%d\n", &TimeBeforeKill);

    pid_t ProccesPids[NumberOfStrings];


    char ** strings;
    strings = (char**)calloc(NumberOfStrings, sizeof(char *));


    for (int i = 0; i < NumberOfStrings; i++)
    {
        strings[i] = malloc(LENGTH_OF_STRING * sizeof(char));
    }

    for (int i = 0; i < NumberOfStrings; i++)
    {
        fgets(strings[i], LENGTH_OF_STRING, f);
    }

    qsort(strings, NumberOfStrings, sizeof(char *), compare);

    for (int i = 0; i < NumberOfStrings; i++)
    {
        printf("strings[%d] = %s\n", i, strings[i]);
    }

    for(int i = -1; i < NumberOfStrings; i++)
    {

        if (pid == 0)
        {
            char** tokens = (char**)calloc(10, sizeof(char*));
            int tokensCount = 0;
            Split (strings[i], delimeters, &tokens, &tokensCount);
            execvp(tokens[1], tokens + 1);
            printf("EXECVP FAILED\n");
            exit(0);
        }
        else
        {
            if(i < NumberOfStrings - 1)
            {
                sleep(((int)strings[i + 1][0])-48);
                pid = fork();

                if(pid != 0)
                  ProccesPids[i + 1] = pid;
            }
        }
    }


    if (pid != 0)
    {
        for (int i = 0; i < NumberOfStrings; i++)
        {
            sleep(TimeBeforeKill - ((int)(strings[NumberOfStrings - 1][0]) - 48) + (((int)(strings[i][0])) - 48));
            kill(ProccesPids[i], 1);
            printf("prosses %d was killed\n", ProccesPids[i]);
        }

    }

    fclose(f);
    return 0;
}
