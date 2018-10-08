#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void Split (char* string, char* delimeters, char*** tokens, int* tokensCount)
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
    // FIXIT: числа 2, 10, 20, 30 нужно вынести в именованные константы, из названия которых ясно, зачем они нужны
    pid_t a;
    pid_t pid = 2;
    char delimeters[] = {" "};

    char ** mass[10];

    for (int i = 0; i < 10; i++)
    {
        mass[i] = malloc(20 * sizeof(char));
    }
    
    // FIXIT: полный путь до файла со списком команд в коде быть не должно.
    // Можете задать путь до файла с командами через аргументы командной строки:
    // https://stackoverflow.com/questions/1593288/qtcreator-and-command-line-arguments
    FILE * f = fopen("/home/ivan/Рабочий стол/фртк/visokosny/umnozhenie/forklesson/text", "r");


    for (int i = 0; i < 10; i++)
    {
        fgets(mass[i], 30, f);
    }


    // FIXIT: число команд для запуска нужно из файла со списком команд взять. "хардкодить" не нужно.
    for(int i = 0; i < 5; i++)
    {
        if (pid == 0)
        {
            printf("parent of child %d\n", getppid());
            printf("    child %d\n", getpid());
            char** tokens = (char**)calloc(10, sizeof(char*));
            int tokensCount = 0;
            // Почему i - 1? От нуля же i идет...
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
            // Если не разберетесь, то на семинаре проговорим, что именно требуется сделать, чтобы "убивать" всех дольше timeout'а.
            // Эту ветку else'а можно пока закомментировать совсем. Пока добейтесь, чтобы ветка if (pid == 0) работала корректно. 
            
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
