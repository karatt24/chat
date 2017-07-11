#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>

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

struct list_client{
	char name[256];
	long id;

	struct list_client *next;
	struct list_client *prev;

};

struct list_client *root;

pthread_mutex_t mut;

int dueue(int m, int n){
	key_t key;
	key = ftok("/", m);
	if(n == 0)
		return msgget(key, IPC_CREAT | 0666);
	return msgget(key, 0);
}

void controll_clients(int *fd){

	int res, err;
	struct msglist buf;
	while(1){
                sleep(0.5);
                res = msgrcv(fd,&buf,sizeof(struct msgbuf),1L, IPC_NOWAIT);
                if (res < 0){
        	        err = errno;
                        if(err == ENOMSG)
                                continue;
                        perror(msgrcv);
                        return 1;
		}
		pthread_mutex_trylock(&mut);
			struct list_client *tmp;
			tmp = malloc(sizeof(struct list_client));
			tmp->
		pthread_mutex_unlock(&mut);
        }
}

int main(){

        pid_t pid;
        int *fd, dev_null, res, err;
	struct list_client *root;
	struct msglist buf_list;
	pthread_mutex_init(&mut, NULL);
	pthread_t tid[2];
	fd = malloc(sizeof(int)*2);
        pid = fork();
        if (pid == 0){
                setsid();
                chdir("/");
                dev_null = open("/dev/null", O_RDWR,0666);
                dup2(dev_null, 0);
                dup2(dev_null, 1);

		fd[0] = queue('m', 0);
		fd[1] = queue('n', 0);
		pthread_create(&tid[0], NULL, controll_clients, (void*)fd);
		while(1){
			sleep(0.5);

		}


	}
}
