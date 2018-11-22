#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255

int main(void)
{
    
    int msqid;
    char pathname[]="keyfile";
    key_t  key;
    int i,len;

    struct mymsgbuf
    {
       long mtype;
       int a;
       int b;
       int res;
       int pid;
    } clientbuf;



    /* Create or attach message queue  */
    
    key = ftok(pathname, 0);
    
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
       printf("Can\'t get msqid\n");
       exit(-1);
    }


    /* Send information */
    

    clientbuf.mtype = getpid();
    scanf("%d %d", &clientbuf.a, &clientbuf.b);
    printf("clientbuf.a = %d, clientbuf.b = %d\n", clientbuf.a, clientbuf.b);
    clientbuf.pid = 15;
    clientbuf.res = 0;
    int sizeOfMsg = sizeof(struct mymsgbuf) - sizeof(long);

    if (msgsnd(msqid, (struct msgbuf *) &clientbuf, sizeOfMsg, 0) < 0)
    {
        printf("Can\'t send message to queue\n");
        //msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);

    }

    len = msgrcv(msqid, (struct msgbuf *) &clientbuf, sizeOfMsg, getpid(), 0);
    printf("client %d * %d = %d\n", clientbuf.a, clientbuf.b, clientbuf.res);

    return 0;    
}
