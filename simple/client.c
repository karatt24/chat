#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/ipc.h>

struct msgbuf{
        long mtype;
        int num;
        char message[256];
};

int main(){

        key_t key;
	int fd, i=0,m;
        struct msgbuf bufs;
        struct msgbuf bufr;

        key = ftok("/",'m');
        fd = msgget(key, IPC_CREAT | 0666);

        while(1){
                printf("1 - Send Message;\n");
                printf("2 - Recive Nessage;\n");
                printf("3 - Quit.\n");
                scanf("%d", &m);
                switch(m){
                        case 1:
                                bufs.mtype = 1;
                                bufs.num = i;
                                scanf("%s", bufs.message);
                                i++;
                                if(msgsnd(fd, &bufs, sizeof(struct msgbuf), 0)){
                                        perror(msgsnd);
                                        return 1;
                                }
                                break;
                        case 2:
                                if (msgrcv(fd,&bufr,sizeof(struct msgbuf),1L,0) < 0){
                                        perror(msgrcv);
                                        return 1;
                                }
				printf("MESSAGE %d: %s\n", bufr.num,bufr.message);
                                break;
                        case 3:
                                exit(0);
                                break;
                        default:
                                exit(0);
                }
        }


}


