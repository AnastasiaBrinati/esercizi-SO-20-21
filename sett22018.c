#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define SIZE 5

char *buff;
int *fd;

char *command;

int sem_r;
int sem_w;

void *thread_func(void *arg){
	
	int i = (int)arg;

    while(1){

	struct sembuf oper;

	oper.sem_num = i;
	oper.sem_op = -1;
	oper.sem_flg = SEM_UNDO;

	semop(sem_r, &oper, 1);
	
	dprintf(*(fd+i), "%s", *buff);   	
	
	oper.sem_num = 0;
        oper.sem_op = 1;
        oper.sem_flg = SEM_UNDO;

        semop(sem_w, &oper, 1);
	
    }

}

void handler(){
	
	for(int i=0; i<N; i++){
		system(command, fd[i]);
	}

}

int main(int argc, char *argv[]){
	
	if(argc < 2){
		return;
	}

	int N = argc-1;
	sem_w = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL);
	semctl(sem_w, 0, IPC_RMID, 1);
	sem_w = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL);

	sem_r = semget(IPC_PRIVATE, N, IPC_CREAT | IPC_EXCL);
        semctl(sem_r, 0, IPC_RMID, 1);
        sem_r = semget(IPC_PRIVATE, N, IPC_CREAT | IPC_EXCL);


	command = "cat %s \n";
	signal(SIGINT, handler);


	//oppure
	//sigset_t set;
	//struct sigatcion act;
	//
	//act.handler = handler; !
	//act.set = set; !
	//act.!
	//
	//sigaction(SIGINT, &act, NULL);
	//
	//
	
	buff = (char *)malloc(SIZE);
	fd = (int *)malloc(sizeof(int)*N);

	pthread_t tid;
	for(int i=0; i<N; i++){
		*fd = open(argv[i+1], O_CREAT | O_RDWR, 0666);
		fd = fd+1;
		pthread_create(&tid, NULL, thread_func, (void *)i); 
	}	
		


	int i=1;
	while(1){
		
		
		char c;
		char *str;
		printf("sono il main thread: \n");
		for(int j=0; j<5; j++){
			scanf("%c", c);
			strcpy(str, c);
		}
		fflush(STDIN);
		
		struct sembuf oper;

       		oper.sem_num = 1;
       		oper.sem_op = -1;
        	oper.sem_flg = SEM_UNDO;
	
	        semop(sem_w, &oper, 1);

		*buff = str;		

		oper.sem_num = i;
                oper.sem_op = 1;
                oper.sem_flg = SEM_UNDO;

                semop(sem_r, &oper, 1);

		i = (i+1)%N;

	}
}
