#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

#define SIZE 5

int sem_r;
int sem_w;

char *buff;

int fd_diretto;
int fd_inverso;

void *thread_diretto(void *arg){

	char *str = (char *)arg;
	strcat(str, "_diretto.txt");
	
	fd_diretto = open(str, O_CREAT | O_RDWR | O_TRUNC, 0666);
	FILE *file_diretto = fdopen(fd_diretto, "w+");
	
	struct sembuf oper;
	
	while(1){
	
		oper.sem_num = 0;
		oper.sem_op = -1;
		oper.sem_flg = SEM_UNDO;
		
		semop(sem_r, &oper, 1);
		
		fprintf(file_diretto, "%s ", buff);
		fflush(file_diretto);
		
		oper.sem_num = 0;
		oper.sem_op = 1;
		oper.sem_flg = SEM_UNDO;
		
		semop(sem_w, &oper, 1);
		
	}

	
	
}

void *thread_inverso(void *arg){

	char *str = (char *)arg;
	strcat(str, "_inverso.txt");
	
	char *buff_appoggio;
	
	fd_inverso = open(str, O_CREAT | O_RDWR | O_TRUNC, 0666);
	FILE *file_inverso = fdopen(fd_inverso, "w+");
	
	struct sembuf oper;
	
	while(1){
		
		buff_appoggio = (char *)malloc(SIZE*4096);
		
		lseek(fd_inverso, 0, SEEK_SET);
		fscanf(file_inverso, "%s", buff_appoggio);
		lseek(fd_inverso, 0, SEEK_SET);
	
		oper.sem_num = 1;
		oper.sem_op = -1;
		oper.sem_flg = SEM_UNDO;
		
		semop(sem_r, &oper, 1);
		
		fprintf(file_inverso, "%s ", buff);
		fflush(file_inverso);
		//inutile: lseek(fd_inverso, 6, SEEK_SET);
		fprintf(file_inverso, "%s", buff_appoggio);
		fflush(file_inverso);
		free(buff_appoggio);
		
		oper.sem_num = 0;
		oper.sem_op = 1;
		oper.sem_flg = SEM_UNDO;
		
		semop(sem_w, &oper, 1);
		
	}

	
	
	
}

void *thread_handler(){

	char c1;
	char c2;


}

void handler(int signo, siginfo_t *a, void *b){

	pthread_t tid;
	pthread_create(&tid, NULL, thread_handler, NULL);
	
}



int main(int argc, char *argv[]){

	if(argc<2){
		printf("usage: gcc prog.c string");
		return 0;
	}	
	
	sigset_t set;
	struct sigaction act;
	
	sigfillset(&set);
	act.sa_sigaction = handler;
	act.sa_mask = set;
	act.sa_flags = 0;
	
	sigaction(SIGINT, &act, NULL);
	
	char S1[1024];
	char S2[1024];
	char c = *argv[1];
	int i=1;
	while(c!='\0'){
		S1[i-1] = c;
		S2[i-1] = c;
		c = *(argv[1]+i);
		i++;
	}
		
	
	sem_w = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | 0666);
	semctl(sem_w, 0, SETVAL, 2);
	
	sem_r = semget(IPC_PRIVATE, 2, IPC_CREAT | IPC_EXCL | 0666);
	semctl(sem_r, 0, SETVAL, 0);
	semctl(sem_r, 1, SETVAL, 0);
	
	buff = (char *)malloc(sizeof(char)*SIZE);
	
	pthread_t tid[2];
	
	if(pthread_create(&tid[0], NULL, thread_diretto, (void *)S1)==-1){
		printf("\nthread_error\n");
		return 0;
	}
	
	if(pthread_create(&tid[1], NULL, thread_inverso, (void *)S2)==-1){
		printf("\nthread_error\n");
		return 0;
	}	
	
	
	struct sembuf oper;
	
	while(1){
	
		oper.sem_num = 0;
		oper.sem_op = -2;
		oper.sem_flg = SEM_UNDO;
		
		semop(sem_w, &oper, 1);
		
		printf("\n[main thread]\n");
		for(int i=0; i<SIZE; i++){ 
		
			c = getchar();
			if(c!='\n'){
				buff[i] = c;
			}
			else{
				i--;
			}
		
		}
		fflush(stdout);
		
		oper.sem_num = 0;
		oper.sem_op = 1;
		oper.sem_flg = SEM_UNDO;
		
		semop(sem_r, &oper, 1);
		
		oper.sem_num = 1;
		oper.sem_op = 1;
		oper.sem_flg = SEM_UNDO;
		
		semop(sem_r, &oper, 1);
	
	}

	
}
