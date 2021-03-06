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

struct all_clients{
        long mtype;
        char list[100][256];
};


struct list_client{
	char name[256];
	long id;

	struct list_client *next;
};

struct list_client *root=NULL;

pthread_mutex_t mut=PTHREAD_MUTEX_INITIALIZER;

int queue(int m, int n){
	key_t key;
	key = ftok("/", m);
	if(n == 0)
		return msgget(key, IPC_CREAT | 0666);
	return msgget(key, 0);
}


void controll_clients(int *fd){

	int res, err;
	struct msglist *buf, *ok;
	ok = malloc(sizeof(struct msglist));
	buf = malloc(sizeof(struct msglist));
	root=NULL;
	while(1){
                sleep(0.5);
		res = 0;
                res = msgrcv(fd[0],buf,sizeof(struct msglist),1L, IPC_NOWAIT);
                if (res < 0){
        	        err = errno;
                        if(err == ENOMSG)
                                continue;
                        perror("msgrcv");
                        return;
		}else{
			ok->mtype = 10L;
			snprintf(ok->name, 256, "OK");
			ok->id = 0L;
			msgsnd(fd[0], ok, sizeof(struct msglist), 0);
		}
		pthread_mutex_lock(&mut);
			struct list_client *bufer, *tmp;
			bufer = malloc(sizeof(struct list_client));
			snprintf(bufer->name, 256, "%s", buf->name);
			bufer->id = buf->id;
			bufer->next=NULL;
			if(root == NULL){
				root = bufer;
			}else{
				tmp = root;
				while(tmp->next!=NULL)
					tmp=tmp->next;
				tmp->next = bufer;
				free(bufer);
			}
		pthread_mutex_unlock(&mut);
	}
}

void chat(int *fd){
	long list_id[100];
	int i, res, err, k;
	struct msgchat buf, *send;
	struct all_clients *list;
	struct list_client *tmp=NULL;
	list = malloc(sizeof(struct all_clients));
	send = malloc(sizeof(struct msgchat));
	while(1){
		sleep(0.1);
		i=0;

		pthread_mutex_lock(&mut);
			tmp=root;
			while(tmp!=NULL){
			        list_id[i]=tmp->id;
        			i++;
	        		tmp=tmp->next;
		        }
	        pthread_mutex_unlock(&mut);

		k=i;
		if (root!=NULL)
			for(i=0; i<k; i++){
				res=0;
        			res = msgrcv(fd[1], &buf, sizeof(struct msgchat), list_id[i], IPC_NOWAIT);
		 		if(res < 0){
					err = errno;
					if(err == ENOMSG)
						continue;
                			perror("msgrcv");
                			return;
				}
				pthread_mutex_lock(&mut);
					tmp=root;
					while(tmp != NULL){
						send->mtype = tmp->id;
						snprintf(send->name, 256, "%s", tmp->name);
						snprintf(send->message, 2048, "%s", buf.message);
						msgsnd(fd[1], &send, sizeof(struct msgchat), 0);
						tmp=tmp->next;
					}
				pthread_mutex_unlock(&mut);
			}
	}

}

void clients(int *fd){
	long list_id[100];
	int i, j;
	struct all_clients *send;
	struct list_client *tmp;
	send = malloc(sizeof(struct all_clients));
	while(1){
		i=0;
		sleep(0.1);
		pthread_mutex_lock(&mut);
		tmp=root;
		while(tmp!=NULL){
			list_id[i]=tmp->id;
			strcpy(send->list[i], tmp->name);
			i++;
			tmp=tmp->next;
		}
		if (i < 100 & i>0)
			snprintf(send->list[i+1],4, "end");
		else
			continue;
		j=i+1;
		for(i=0; i < j; i++){
			send->mtype=list_id[i];
			msgsnd(fd[2], &send, sizeof(struct all_clients), 0);
		}
		pthread_mutex_unlock(&mut);
	}
}

int main(){

	pid_t pid;
	int *fd, dev_null, res, err;
	struct msglist buf_list;
	pthread_t tid[3];

        long list_id[100];
        int i, j;
        struct all_clients *send;
        struct list_client *tmp=NULL;
        send = malloc(sizeof(struct all_clients));
	fd = malloc(sizeof(int)*3);
/*	pid = fork();
        if (pid == 0){
                setsid();
                chdir("/");
                dev_null = open("/dev/null", O_RDWR,0666);
                dup2(dev_null, 0);
                dup2(dev_null, 1);
*/
		fd[0] = queue('m', 0);
		fd[1] = queue('n', 0);
		fd[2] = queue('w', 0);
		pthread_create(&tid[0], NULL, (void *)controll_clients, (void*)fd);
		pthread_create(&tid[1], NULL, (void *)chat, (void*)fd);

        while(1){
                i=0;
                sleep(0.1);
                pthread_mutex_lock(&mut);
                tmp=root;
                while(tmp!=NULL){
                        list_id[i]=tmp->id;
                        snprintf(send->list[i],256, "%s",  tmp->name);
                        i++;
                        tmp=tmp->next;
                }
                pthread_mutex_unlock(&mut);
                if (i < 100 & i > 0)
                        snprintf(send->list[i+1], 4, "end");
                else
                        continue;
                j=i+1;
                for(i=0; i < j; i++){
                        send->mtype=list_id[i];
                        msgsnd(fd[2], &send, sizeof(struct all_clients), 0);
                }
	}

}
