#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#define LAST_MESSAGE 100
#define MESS_LEN 100

int main(void)
{
    int msqid;
    char pathname[]="keyfile";
    key_t  key;
    int len;
    int pid;
    int Wait;

    struct mymsgbuf
    {
       long mtype;
       int a;
       int b;
       int res;
       int pid;
    } clientbuf;

    int sizeOfMsg = sizeof(struct mymsgbuf) - sizeof(long);

    key = ftok(pathname, 0);

    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
       printf("Can\'t get msqid\n");
       exit(-1);
    }

    while (1)
    {
       if ((len = msgrcv(msqid, (struct msgbuf *) &clientbuf, sizeOfMsg, 0, 0)) < 0){
         printf("Can\'t receive message from queue\n");
         exit(-1);
       }

       pid = fork();
       //wait(&Wait);
       if(pid != 0)
       {
           clientbuf.res = clientbuf.a * clientbuf.b;
           printf("server %d * %d = %d\n", clientbuf.a, clientbuf.b, clientbuf.res);


           if (msgsnd(msqid, (struct msgbuf *) &clientbuf, sizeOfMsg, 0) < 0)
           {
               printf("Can\'t send message to queue\n");
               //msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
               exit(-1);
           }

       }

    }
    printf("--------end--------\n");
    return 0;
}
