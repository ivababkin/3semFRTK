#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUF 50

int main()
{
    int     fd1, fd2, result;

    size_t  size = 0;
    char    resstring[14];
    char    name1[] = "aaa.fifo";
    char    name2[] = "bbb.fifo";
    int numberOfConsole, k;
    printf("choose numberOfConsole '1' or '2'");
    scanf("%d", &numberOfConsole);
    char * str = (char*)calloc(BUF, sizeof(char));
    pid_t pid = -1;


    /*if(k = mknod(name1, S_IFIFO | 0666, 0) < 0)
    {
        printf("Can't create FIFO1\n");
        exit(-1);
    }
    printf("k = %d\n", k);


    if(k = mknod(name2, S_IFIFO | 0666, 0) < 0)
    {
        printf("Can't create FIFO2\n");
        exit(-1);
    }

    printf("k = %d\n", k);
*/

    printf("numberOfConsole = %d\n", numberOfConsole);



    if (numberOfConsole == 1)// ////////////////////////
    {
        pid = fork();


        if (pid == 0)
        {
            fd2 = open(name2, O_RDONLY);
            while(1)
            {
                read(fd2, str, BUF);
                if (size < 0)
                {
                    printf("O NEET");
                }
                printf("2: ");
                puts(str);

            }
        }
        else
        {
            fd1 = open(name1, O_WRONLY);
            while(1)
            {
                scanf("%s", str);
                write(fd1, str, BUF);
            }
        }
    }

    if (numberOfConsole == 2)// /////////////////////////////////
    {
        pid = fork();


        if (pid == 0)
        {
            fd1 = open(name1, O_RDONLY);
            while(1)
            {
                size = read(fd1, str, BUF);
                if(size < 0)
                {
                    printf("O NEEET");
                }
                printf("1: ");
                puts(str);

            }
        }
        else
        {
            fd2 = open(name2, O_WRONLY);
            while (1)
            {
                scanf("%s", str);
                write(fd2, str, BUF);
            }
        }
    }

    return 0;
}
