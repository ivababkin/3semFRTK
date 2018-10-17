#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int     fd1, fd2, result;

    size_t  size = 0;
    char    resstring[14];
    char    name1[] = "aaa.fifo";
    char    name2[] = "bbb.fifo";
    int i, k;
    printf("choose '1' or '2'");
    scanf("%d", &i);
    char * str = (char*)calloc(50, sizeof(char));
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

    printf("i = %d\n", i);



    size = 1;
    if (i == 1)// ////////////////////////
    {
        pid = fork();


        if (pid == 0)
        {
            fd2 = open(name2, O_RDONLY);
            while(1)
            {
                size = read(fd2, str, 50);
                if (size < 0)
                {
                    printf("O NEET");
                }
                printf("2: ");
                puts(str);
                if(strcmp(str, "kill") == 0)
                    exit(0);

            }
        }
        else
        {
            fd1 = open(name1, O_WRONLY);
            while(1)
            {
                scanf("%s", str);
                size = write(fd1, str, 50);
                if(strcmp(str, "kill") == 0)
                    exit(0);
            }
        }
    }

    if (i == 2)// /////////////////////////////////
    {
        pid = fork();


        if (pid == 0)
        {
            fd1 = open(name1, O_RDONLY);
            while(1)
            {
                size = read(fd1, str, 50);
                if(size < 0)
                {
                    printf("O NEEET");
                }
                printf("1: ");
                puts(str);
                if(strcmp(str, "kill") == 0)
                    exit(0);
            }
        }
        else
        {
            fd2 = open(name2, O_WRONLY);
            while (1)
            {
                scanf("%s", str);
                size = write(fd2, str, 50);
                if(strcmp(str, "kill") == 0)
                    exit(0);
            }
        }
    }

    return 0;
}
