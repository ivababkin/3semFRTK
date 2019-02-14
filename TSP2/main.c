#include<sys/types.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdio.h>
#include<errno.h>
#include<unistd.h>


#define FIELDSIZE 10
#define BUF 5

int NumberOfShips = 2;


void renderField(char ** myfield, char ** hisfield)
{
    printf("\033[3;1H");
    puts("My field              His field");
    for(int i = 0; i < FIELDSIZE; i++)
    {
        printf("%d", i);
        for(int j = 0; j < FIELDSIZE; j++)
        {
            printf("%c", myfield[i][j]);
        }

        printf("           ");
        printf("%d", i);
        for(int j = 0; j < FIELDSIZE; j++)
        {
            printf("%c", hisfield[i][j]);
        }

        printf("\n");
    }
    printf(" ");
    for(int j = 0; j < FIELDSIZE; j++)
        printf("%c", j + 65);

    printf("            ");

    for(int j = 0; j < FIELDSIZE; j++)
        printf("%c", j + 65);
    printf("\n");
    printf("_____________________________________________\033[50D");

}

char ** genegateField()
{
    char ** field = (char **)malloc(FIELDSIZE * sizeof(char*));

    for(int i = 0; i < FIELDSIZE; i++)
    {
        field[i] = (char *)malloc(FIELDSIZE * sizeof(char));
        for(int j = 0; j < FIELDSIZE; j++)
            field[i][j] = '~';
    }

    return field;
}

void shot(char **myfield, char **hisfield, int fd1)
{
    char sndstr[BUF];
    char recvstr[BUF];
    printf("\033[15;1H");
    printf("____________________________________________\033[50D");
    printf("your shot in format A1 or J9: ");
    scanf("%s", sndstr);
    write(fd1, sndstr, BUF);
    sleep(1);
    read(fd1, recvstr, BUF);

    if(strcmp(recvstr, "YES") == 0)
    {
        hisfield[(int)(sndstr[1]) - 48][(int)(sndstr[0]) - 65] = 'X';
        renderField(myfield, hisfield);
    }

    if(strcmp(recvstr, "NO") == 0)
    {
        hisfield[(int)(sndstr[1]) - 48][(int)(sndstr[0]) - 65] = '*';
        renderField(myfield, hisfield);
    }

    if(strcmp(recvstr, "END") == 0)
    {
        printf("\e[1;34mWE ARE THE CHAMPIONS\e[0m   ");
        exit(1);
    }
    printf("____________________________________________\033[50D");
    printf("2: ");
    puts(recvstr);
}



void defence(char **myfield,char **hisfield, int fd1)
{

    char str[BUF];
    printf("\033[15;1H");
    //printf("____________________________________________\033[50D");

    read(fd1, str, BUF);
    printf("BAD GUY: %s\n", str);
    /*read(fd1, str, BUF);
    printf("BAD GUY: %s\n", str);
    read(fd1, str, BUF);
    printf("BAD GUY: %s\n", str);
    printf("666666666666666666\n");
*/
    if(myfield[(int)(str[1]) - 48][(int)(str[0]) - 65] == 'B')
    {
        // printf("==myfield[%d][%d] = %c\n",(int)(str[1]) - 48,(int)(str[0]) - 65, myfield[(int)(str[1]) - 48][(int)(str[0]) - 65]);
        myfield[(int)(str[1]) - 48][(int)(str[0]) - 65] = 'X';
        str[0] = 'Y';
        str[1] = 'E';
        str[2] = 'S';
        str[3] = '\0';

        NumberOfShips --;

        if(NumberOfShips == 0)
        {
            printf("LOOOOOOOOOOOOOSE\n");
            str[0] = 'E';
            str[1] = 'N';
            str[2] = 'D';
            str[3] = '\0';
            write(fd1, str, BUF);
            exit(1);
        }

        write(fd1, str, BUF);

        renderField(myfield, hisfield);

    }
    else
    {
        myfield[(int)(str[1]) - 48][(int)(str[0]) - 65] = '*';
        renderField(myfield, hisfield);
        str[0] = 'N';
        str[1] = 'O';
        write(fd1, str, BUF);
    }

}



void addShip(char * turn, char **field)
{
    //printf("[==]myfield[%d][%d] = %c\n",(int)(turn[1]) - 48,(int)(turn[0]) - 65, field[(int)(turn[1]) - 48][(int)(turn[0]) - 65]);

    field[(int)(turn[1]) - 48][(int)(turn[0]) - 65] = 'B';
    //printf("[==]myfield[%d][%d] = %c\n",(int)(turn[1]) - 48,(int)(turn[0]) - 65, field[(int)(turn[1]) - 48][(int)(turn[0]) - 65]);

}

void placeships(char ** myfield, char ** hisfield)
{
    for(int i = 0; i < NumberOfShips; i++)
    {
        char turn[5];
        printf("your ship in format A1 or J9:");
        scanf("%s", turn);
        //printf("turn = %s\n", turn);
        addShip(turn, myfield);
        renderField(myfield, hisfield);
    }
}


int main(int argc, char * argv[])
{
    printf("\033[2;2H");
    printf("\e[1;34mSEA FIGHT\e[0m   ");
    printf("Be carefule, there is no foolproves:(\n");
    printf("\033]2;BATTLE\007 ");

    char ** myfield = genegateField();
    char ** hisfield = genegateField();

    int numberOfPlayer;

    int fd1;
    int n;
    int i;
    char sendline[5], recvline[5];
    struct sockaddr_in servaddr;
    if(argc != 2)
    {
        printf("no IP found\n");
        exit(1);
    }
    bzero(sendline, 5);
    bzero(recvline, 5);

    if((fd1 = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("cant create socket\n");
        exit(-1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(51000);
    if(inet_aton(argv[1], &servaddr.sin_addr) == 0)
    {
        printf("invalid IP\n");
        close(fd1);
        exit(1);
    }

    if(connect(fd1, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
    {
        printf("cant connect socket\n");
        close(fd1);
        exit(1);
    }


    /*printf("print 'R', if ypu are readyy for the battle: ");
    fflush(stdin);
    fgets(sendline, 1000, stdin);
    if ((n = write(sockfd, sendline, strlen(sendline) + 1)) < 0)
    {
        printf("cant write\n");
        close(sockfd);
        exit(1);
    }

*/
    if ((read(fd1, recvline, BUF)) < 0)
    {
        printf("cant read\n");
        close(fd1);
        exit(1);
    }

    if(strcmp(recvline, "111") == 0)
    {
        numberOfPlayer = 1;
    }


    if(strcmp(recvline, "222") == 0)
    {
        numberOfPlayer = 2;
    }

    printf("%d", numberOfPlayer);






    printf("-0000------0000---\n");
    printf("--0----0----0----0--\n");
    printf("--0---------0----0--\n");
    printf("--0---00----0----0--\n");
    printf("--0----0----0----0--\n");
    printf("---0000------0000---\n");
    sleep(1);

    renderField(myfield, hisfield);

    placeships(myfield, hisfield);



    char    name1[] = "aaa.fifo";
    char    name2[] = "bbb.fifo";
    char * str = (char*)calloc(BUF, sizeof(char));


    /*int k;
    if(k = mknod(name1, S_IFIFO | 0666, 0) < 0)
    {
        printf("Can't create FIFO1\n");
        exit(-1);
    }
    printf("k = %d\n", k);


    if(k = mknod(name2, S_IFIFO | 0666, 0) < 0)
    {
        printf("Can't create FIFO2\n");
        exit(-1);
    }*/




    if (numberOfPlayer == 1)//
    {
        //int fd1 = open(name2, O_RDWR);
        //int fd1 = open(name1, O_WRONLY);
        while(1)
        {
            shot(myfield, hisfield, fd1);
            sleep(2);
            defence(myfield, hisfield, fd1);
        }

    }

    if (numberOfPlayer == 2)//
    {

        //int fd1 = open(name2, O_RDWR);
        //int fd1 = open(name1, O_RDONLY);
        while(1)
        {
            //printf("defence\n");
            defence(myfield, hisfield, fd1);
            //printf("end defence\n");
            sleep(1);
            shot(myfield, hisfield, fd1);
        }

    }


    return 0;
}
