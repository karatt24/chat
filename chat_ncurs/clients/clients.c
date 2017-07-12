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

int dueue(int m, int n){
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

	struct msglist buf;
	struct list_client *tmp, *list_buf=NULL;
	long id;
	pid_t pid;
	pid=getpid();
	id=(long)pid;
	sprintf(buf->name, "%s", name);
	buf->id=id;
	buf->mtype=1L;
	msgsnd(fd[0], &buf, sizeof(struct msglist), 0);

}

void send(int *fd){
	struct list_client buf;
	struct msgchat mess;
	int i=0;
	pid_t pid;
	pid=getpid();
        my_id=(long)pid;

	msgrcv(fd[2], &buf, sizeof(struct list_client), my_id, 0);
        while(strcmp(message, "/quit") != 0){
                printf("---------------clients---------------\n");
                while(strcmp(buf->name[i], "end")){
                        printf("%s", buf->name[i]);
                        i++;
                }
                printf("\n----------------chat-----------------\n");
                msgrcv(fd[1], &mess, sizeof(struct msgchat), my_id, 0);
		printf("%s: %s", mess->name, mess->message);
	}
}

int main(){

	int i=0, *fd;
	long my_id;
	char *message;
	struct all_clients buf;
	struct msgchat send;
	message = malloc(2048);
	fd = malloc(sizeof(int)*2);
	fd[0] = dueue('m', 1);
	fd[1] = dueue('n', 1);
	fd[2] = dueue('w', 1);
	pid=getpid();
	my_id=(long)pid;
	printf("\nInter your name: ");
	scanf("%s", name);
	while(strcmp(message, "/quit") != 0){
		gets(message);
		send->mtype = my_id;
		sprintf(send->name, "%s", name);
		sprintf(send->message, "%s", message);
		msgsnd(fd[1], &send, sizeof(struct msgchat), 0);
	}
}
