#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


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
    *tokensCount = count;

}


int main(int argc, char* argv[], char* envp[])
{
    pid_t a;
    pid_t pid = 2;
    char delimeters[] = {" "};

    char ** mass[10];

    for (int i = 0; i < 10; i++)
    {
        mass[i] = malloc(20 * sizeof(char));
    }
    FILE * f = fopen("/home/ivan/Рабочий стол/фртк/visokosny/umnozhenie/forklesson/text", "r");


    for (int i = 0; i < 10; i++)
    {
        fgets(mass[i], 30, f);
    }



    for(int i = 0; i < 5; i++)
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
            //printf("%s\n", tokens[3]);
            printf ("--------------------------------------\n");
            //execvp(tokens[0],tokens);
            //printf("EXECVP FAILED\n");
            exit(0);
        }
        else
        {
            //signal (SIGALRM, printf("F\n"));
            //alarm (10);
            sleep(1);
            if (i != 0)
                kill(pid, 1);

            pid = fork();
            //wait(&a);
        }
    }

    printf ("parent %d died\n", getpid());
    fclose(f);
    return 0;
}
