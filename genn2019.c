#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <fcntl.h>
#include <string.h>

#define SIZE 4096

typedef struct _thread_args{
	int i;
	char *str;
} thread_args;


int sem_r;
int sem_w;

char *buff;

void *thread_func(void *arg){

	thread_args *targs = (thread_args *) arg;	
        int i = targs->i;	

	struct sembuf oper;

	while(1){

	oper.sem_num = i;
        oper.sem_op = -1;
        oper.sem_flg = SEM_UNDO;

        semop(sem_r, &oper, 1);


	if(strcmp(targs->str, buff)==0){
		int len = strlen(buff);
		strcpy(buff, "*");
		for(int i=1; i<len; i++){
			strcat(buff, "*");
		}
	}
	
	//printf("\nthread %d, ecco %s", i, buff);

	oper.sem_num = 0;
        oper.sem_op = 1;
        oper.sem_flg = SEM_UNDO;

        semop(sem_w, &oper, 1);

	}
}


char command[1024];

void printer(int signo){

	system(command);

	return;
}

int main(int argc, char *argv[]){

	if(argc<3){
		printf("usage: gcc genn2019.c file_name s1 .. s\n");
		return 0;
	}

	int N = argc-2;

	int fd = open(argv[1], O_CREAT | O_TRUNC | O_RDWR, 0666);
	FILE *file = fdopen(fd, "w+");
	
	buff = (char *)malloc(SIZE);
	
	sem_w = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL);
	semctl(sem_w, 0, IPC_RMID, 1);
	sem_w = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL);
	
	sem_r = semget(IPC_PRIVATE, N, IPC_CREAT | IPC_EXCL);
        semctl(sem_r, 0, IPC_RMID, 1);
        sem_r = semget(IPC_PRIVATE, N, IPC_CREAT | IPC_EXCL);

	semctl(sem_w, 0, SETVAL, 0);
	for(int i=0; i<N; i++){
		semctl(sem_r, i, SETVAL, 0);
	}

	sprintf(command,"cat %s",argv[1]);
//	signal(SIGINT, printer);

	
	pthread_t tid;
	for(int i=0; i<N; i++){
		thread_args *arg = (thread_args *)malloc(sizeof(thread_args));
		arg->i = i;
		arg->str = argv[i+2];
		pthread_create(&tid, NULL, thread_func, (void *)arg);
	}

	while(1){
		
		//char *buff;
		printf("\nsono il main thread, inserire una stringa: ");
		scanf("%s", buff);
		//fflush(STDIN);
		
		struct sembuf oper; 

		for(int i=0; i<N; i++){
	
			printf("\nSto segnalando al thread %d", i);
			oper.sem_num = i;
			oper.sem_op = 1;
			oper.sem_flg = SEM_UNDO;
			
			semop(sem_r, &oper, 1);
		}
		
		oper.sem_num = 0;
                oper.sem_op = -N;
                oper.sem_flg = SEM_UNDO;

                semop(sem_w, &oper, 1);


		fprintf(file, "%s \n", buff);
                fflush(file);
		
	}
	
	return 0;
}
