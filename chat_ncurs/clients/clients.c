#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>



struct msglist{
        long mtype;
        char name[256];
        long id;
};

struct msgchat{
        long mtype;
        char name[256];
        char message[2048];
};




pthread_mutex_t mut=PTHREAD_MUTEX_INITIALIZER;

int queue(int m, int n){
        key_t key;
        key = ftok("/", m);
        if(n == 0)
                return msgget(key, IPC_CREAT | 0666);
        return msgget(key, 0);
}

struct list_client{
        long mtype;
        char name[100][256];
};

struct list_client list;
char name[256];

void connect(int *fd){

	struct msglist *buf, *ans;
	struct list_client *tmp, *list_buf=NULL;
	long id;
	int res, err;
	pid_t pid;
	char *name;

	name = malloc(256);
	buf=malloc(sizeof(struct msglist));
	ans=malloc(sizeof(struct msglist));
	pid=getpid();
	id=(long)pid;
	printf("\nInter your name: ");
        scanf("%s", name);

	while(strcmp(ans->name, "OK")){
		res = 0;
		snprintf(buf->name, 256, "%s", name);
		buf->id=id;
		buf->mtype=1L;
		msgsnd(fd[0], buf, sizeof(struct msglist), 0);
		msgrcv(fd[0], ans, sizeof(struct msglist), 10L, 0);
        /*        if (res < 0){
                        err = errno;
                        if(err == ENOMSG)
                                continue;
                        perror("msgrcv");
                        return;
                }else{
*/		if(strcmp(ans->name, "OK"))
			printf("Connect - OK!");
	//	}
	}
}

void resive(int *fd){
	struct list_client buf;
	struct msgchat mess;
	long my_id;
	int i=0;
	pid_t pid;
	pid=getpid();
	my_id=(long)pid;

	msgrcv(fd[2], &buf, sizeof(struct list_client), my_id, 0);
	while(1){
	    	printf("---------------clients---------------\n");
    		while(strcmp(buf.name[i], "end")){
	            printf("%s", buf.name[i]);
        	    i++;
       		}
       		printf("\n----------------chat-----------------\n");
        	msgrcv(fd[1], &mess, sizeof(struct msgchat), my_id, 0);
		printf("%s: %s", mess.name, mess.message);
	}
}

int main(){

	int i=0, *fd;
	long my_id;
	char *message;
	struct msgchat send;
	pid_t pid;
	pthread_t tid[2];
	message = malloc(2048);
	fd = malloc(sizeof(int)*2);
	fd[0] = queue('m', 1);
	fd[1] = queue('n', 1);
	fd[2] = queue('w', 1);
	pid=getpid();
	my_id=(long)pid;
//	pthread_create(&tid[0], NULL, (void *)connect, (void*)fd);
	connect(fd);
	pthread_create(&tid[1], NULL, (void *)resive, (void*)fd);
	while(strcmp(message, "/quit") != 0){
		gets(message);
		send.mtype = my_id;
		snprintf(send.name, 256, "%s", name);
		snprintf(send.message, 2048, "%s", message);
		msgsnd(fd[1], &send, sizeof(struct msgchat), 0);
	}
}
