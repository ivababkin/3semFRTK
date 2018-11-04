#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// FIXIT: называйте, пожалуйста, константы, объявленные с помощью define в таком стиле: NUMBER_OF_WORDS 
#define numberofwords 20
#define lengthofwords 20


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


int main(int argc, char* argv[], char* envp[])
{
    pid_t a;
    pid_t pid = 2;

    char delimeters[] = {" '\n'"};


    char ** mass;
    mass = (char**)calloc(numberofwords, sizeof(char *));


    // FIXIT: нужно число 10 вынести в именованную константу
    for (int i = 0; i < 10; i++)
    {
        mass[i] = malloc(lengthofwords * sizeof(char));
    }

    // FIXIT: если в коде нужно получить путь до некоторого файла, его нужно передавать через аргументы командной строки
    FILE* f = fopen("/home/ivan/Рабочий стол/фртк/all/forkprog/text", "r");

    int numberofstrings = 0;
    fscanf(f, "%d\n", &numberofstrings);

    printf("===%d===\n", numberofstrings);

    for (int i = 0; i < numberofwords; i++)
    {
        fgets(mass[i], lengthofwords, f);
    }



    for(int i = 0; i < numberofstrings; i++)
    {
        if (pid == 0)
        {
            printf("perent of child %d\n", getppid());
            printf("    child %d\n", getpid());
            char** tokens = (char**)calloc(10, sizeof(char*));
            int tokensCount = 0;
            Split (mass[i - 1], delimeters, &tokens, &tokensCount);
            //printf ("%d\n", tokensCount);
            printf("tokens[0] = %s\n", tokens[0]);
            printf("tokens[1] = %s\n", tokens[1]);
            printf("tokens[2] = %s\n", tokens[2]);
            printf("tokens[3] = %s\n", tokens[3]);

            //printf("%s\n", tokens[3]);
            printf ("--------------------------------------\n");
            //tokens[2] = NULL;
            execvp(tokens[0],tokens);
            printf("EXECVP FAILED\n");
            exit(0);
        }
        else
        {
            //signal (SIGALRM, printf("F\n"));
            //alarm (10);
            
            // FIXIT: на текущий момент ваш код не соответствует условию задачи ... для каждой команды указано, через сколько сек её надо
            // запустить и какой time limit для программ, т.е. входной файл выглядит примерной так:
            // 3 5
            // 2 ls -al
            // 1 pwd
            // 3 ./hello_world
            // Причем, если, например, последняя программа зависла, то её через 5 сек нужно убить
            sleep(1);
            if (i != 0)
                kill(pid, 1);

            // Неясно, почему fork выполняете здесь, а не перед if (pid == 0) 
            pid = fork();
            //wait(&a);
        }
    }

    printf ("parent %d died\n", getpid());
    fclose(f);
    return 0;
}
