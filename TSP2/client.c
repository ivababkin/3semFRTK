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

int NumberOfShips = 5;



void scanTurn(char * str)
{
    while(1)
    {

        printf("\033[16;1H");
        scanf("%s", str);

        if(str[2] != NULL)
        {
            printf("\033[15;1H");
            printf("_____________________________________________\n");
            printf("\033[15;1H");
            printf("Oh it's so big\n");
            printf("\033[16;1H");
            printf("_____________________________________________\n");
            continue;
        }
        char mass1Letter[] = "ABCDEFGHIJ";

        char del[3];
        del[0] = str[0];
        del[1] = '\0';
        char * res = strstr(mass1Letter, del);
        if(res == NULL)
        {
            printf("\033[15;1H");
            printf("_____________________________________________\n");
            printf("\033[15;1H");
            printf("FORMAT: a big letter from A to J and a digit\n");
            printf("\033[16;1H");
            printf("_____________________________________________\n");

            continue;
        }


        char mass2Letter[] = "0123456789";

        del[0] = str[1];
        del[1] = '\0';
        res = strstr(mass2Letter, del);
        if(res == NULL)
        {
            printf("\033[15;1H");
            printf("_____________________________________________\n");
            printf("\033[15;1H");
            printf("FORMAT:a digit from 0 to 9\n");
            printf("\033[16;1H");
            printf("_____________________________________________\n");

            continue;
        }
        break;
    }
    printf("\033[15;1H");
    printf("_____________________________________________\n");
    printf("\033[15;1H");

}




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
    printf("_____________________________________________\033[50D");
    printf("your shot in format A1 or J9: ");    
    scanTurn(sndstr);
    write(fd1, sndstr, BUF);
    sleep(1);
    read(fd1, recvstr, BUF);

    printf("\033[15;1H");
    printf("_____________________________________________\033[50D");
    printf("BAD GUY: ");
    puts(recvstr);

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
        printf("\033[15;1H");
        printf("_____________________________________________\033[50D");
        printf("\033[16;1H");
        printf("_____________________________________________\033[50D");
        printf("\033[16;1H");
        printf("\e[1;34mWE ARE THE CHAMPIONS\e[0m   ");
        exit(1);
    }
}



void defence(char **myfield,char **hisfield, int fd1)
{

    char str[BUF];
    printf("\033[15;1H");
    printf("_____________________________________________\033[50D");

    read(fd1, str, BUF);

    if(myfield[(int)(str[1]) - 48][(int)(str[0]) - 65] == 'B')
    {
        myfield[(int)(str[1]) - 48][(int)(str[0]) - 65] = 'X';
        str[0] = 'Y';
        str[1] = 'E';
        str[2] = 'S';
        str[3] = '\0';

        NumberOfShips --;

        if(NumberOfShips == 0)
        {            
            printf("\033[15;1H");
            printf("_____________________________________________\033[50D");
            printf("\033[16;1H");
            printf("_____________________________________________\033[50D");
            printf("\033[16;1H");
            printf("\e[1;34mLOOOOOOOOOOOOOSE\e[0m   ");
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
    field[(int)(turn[1]) - 48][(int)(turn[0]) - 65] = 'B';
}

void placeships(char ** myfield, char ** hisfield)
{
    for(int i = 0; i < NumberOfShips; i++)
    {
        char turn[5];        
        printf("\033[15;1H");
        printf("your ship in format A1 or J9:");        
        printf("\033[16;1H");
        scanTurn(turn);
        addShip(turn, myfield);
        renderField(myfield, hisfield);
    }
    printf("\033[15;1H");
    printf("wait for your slow opponent");
    sleep(1);
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

    printf("\033[15;1H");
    printf("_____________________________________________\033[50D");
    printf("\033[16;1H");
    printf("_____________________________________________\033[50D");


    sleep(1);

    renderField(myfield, hisfield);

    placeships(myfield, hisfield);


    char * str = (char*)calloc(BUF, sizeof(char));





    if (numberOfPlayer == 1)//
    {
        while(1)
        {
            shot(myfield, hisfield, fd1);
            sleep(1);
            defence(myfield, hisfield, fd1);
        }

    }

    if (numberOfPlayer == 2)//
    {
        while(1)
        {
            defence(myfield, hisfield, fd1);
            sleep(1);
            shot(myfield, hisfield, fd1);
        }

    }


    return 0;
}
