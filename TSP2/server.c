#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#include <stdlib.h>

#define BUF 5



int main()
{
    int sockfd, newsockfd1, newsockfd2;
    int clilen;
    int n;
    char line[1000];

    struct sockaddr_in servaddr, cliaddr;

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("cant create socket\n");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(51000);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
    {
        printf("cant bind socket\n");
        close(sockfd);
        exit(1);
    }

    if (listen(sockfd, 5) < 0)
    {
        printf("cant listen socket\n");
        close(sockfd);
        exit(1);
    }

    //while(1)
    {
        clilen = sizeof(cliaddr);
        if((newsockfd1 = accept(sockfd, (struct sockaddr *) &cliaddr, &clilen)) < 0)
        {
            printf("cant make a link1\n");
            close(sockfd);
            exit(1);
        }
        if((newsockfd2 = accept(sockfd, (struct sockaddr *) &cliaddr, &clilen)) < 0)
        {
            printf("cant make a link2\n");
            close(sockfd);
            exit(1);
        }

        /*if((n = read(newsockfd1, line, 999)) > 0)
        {
            printf("line1 = %s", line);
            char massage[] = "you are first";
            if((n = write(newsockfd1, massage, 20)) < 0)
            {
                printf("cant write\n");
                close(sockfd);
                exit(1);
            }
        }

        if((n = read(newsockfd2, line, 999)) > 0)
        {
            printf("line2 = %s", line);
            char massage[] = "you are second";
            if((n = write(newsockfd2, massage, 20)) < 0)
            {
                printf("cant write\n");
                close(sockfd);
                exit(1);
            }
        }*/
        char massage[] = "111";
        if((write(newsockfd1, massage, BUF)) < 0)
        {
            printf("cant write\n");
            close(sockfd);
            exit(1);
        }

        massage[0] = '2';
        massage[1] = '2';
        massage[2] = '2';


        if((write(newsockfd2, massage, BUF)) < 0)
        {
            printf("cant write\n");
            close(sockfd);
            exit(1);
        }
    }

    printf("start conversation\n");

    while (1)
    {
        if((read(newsockfd1, line, BUF)) > 0)
        {
            printf("line = %s\n", line);

            if((write(newsockfd2, line, BUF)) < 0)
            {
                printf("cant write\n");
                close(sockfd);
                exit(1);
            }
        }

        if((read(newsockfd2, line, BUF)) > 0)
        {
            printf("line2 = %s\n", line);

            if((write(newsockfd1, line, BUF)) < 0)
            {
                printf("cant write\n");
                close(sockfd);
                exit(1);
            }
        }

        if((read(newsockfd2, line, BUF)) > 0)
        {
            printf("line2 = %s\n", line);

            if((write(newsockfd1, line, BUF)) < 0)
            {
                printf("cant write\n");
                close(sockfd);
                exit(1);
            }
        }

        if((read(newsockfd1, line, BUF)) > 0)
        {
            printf("line = %s\n", line);

            if((write(newsockfd2, line, BUF)) < 0)
            {
                printf("cant write\n");
                close(sockfd);
                exit(1);
            }
        }


    }

    close(newsockfd1);
    close(newsockfd2);
    close(sockfd);

    return 0;
}
