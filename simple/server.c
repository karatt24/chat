#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>


struct msgbuf{
	long mtype;
	int num;
	char message[256];
};

struct msglist{
        long mtype;
        char name[256];
        long id;
};

int main(){

	int i=1, m;
	int fd;
	key_t key;
	struct msglist bufs;
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
				bufs.mtype = 12L;
				scanf("%s", bufs.name);
				i++;
				if(msgsnd(fd, &bufs, sizeof(struct msglist), 0)){
					perror(msgsnd);
					return 1;
				}
				break;
			case 2:
				if (msgrcv(fd, &bufr, sizeof(struct msgbuf), 1L, 0) < 0){
					perror(msgrcv);
					return 1;
				}
				printf ("MESSAGE %d: %s\n", bufr.num, bufr.message);
				break;
			case 3:
				exit(0);
				break;
			default:
				exit(0);
		}
	}

}
