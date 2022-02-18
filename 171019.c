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

#define SIZE 4096

int sem_r;
int sem_w;

char **buff;
int N;
int *fd;

typedef struct _thread_args{
	int me;
	int my_file;
}thread_args;

void *thread_func(void *arg){

	thread_args *my_args = (thread_args *)arg;

	int fd = my_args->my_file;
	FILE *file = fdopen(fd, "w+");
	int i = my_args->me;
	struct sembuf oper;

//	printf("\nSono il thread: %d, scrivo sul file %s\n", i, my_args->my_string);
	
	while(1){
		oper.sem_num = i;
		oper.sem_op = -1;
		oper.sem_flg = SEM_UNDO;

		semop(sem_r, &oper, 1);

		//printf("\nSono il thread: %d, ho ricevuto %s", i, buff[i]);
		//fflush(stdout);

		fprintf(file, "%s \n", buff[i]);
		fflush(file);

		oper.sem_num = i;
        	oper.sem_op = 1;
        	oper.sem_flg = SEM_UNDO;

        	semop(sem_w, &oper, 1);
	}

	close(fd);

}

void *handler(){
	
	FILE *file_output = fopen("output.txt", "w");
	char *line;
	FILE *file[N];

	int i=0;

	for(int i=0; i<N; i++){

                file[i] = fdopen(fd[i], "r+");

	}

	while(fscanf(file[i], "%m[^\n]", &line)!=EOF){
	   
		fgetc(file[i]);
		//strtok();
			printf("%s", line);
			fprintf(file_output, "%s \n", line);
	
	
			free(line);
		
	 	i = (i+1)%N;
	}

	for(int i=0; i<N; i++){
		fclose(file[i]);
	}
}

int main(int argc, char *argv[]){

	if(argc<2){
		printf("usage: gcc 171019.c file1 .. fileN\n");
		return 0;
	}

	N = argc-1;
	sigset_t set;
	struct sigaction act;

	printf("argc: %d, N: %d\n",argc, N);
	

	fd = (int *)malloc(N*sizeof(int));
	for(int i=0; i<N; i++){
                fd[i] = open(argv[i+1], O_CREAT | O_RDWR | O_TRUNC, 0666);
        }

	sem_r = semget(IPC_PRIVATE, N, IPC_CREAT | 0666);
	for(int i=0; i<N; i++){
		semctl(sem_r, i, SETVAL, 0);
	}

	sem_w = semget(IPC_PRIVATE, N, IPC_CREAT | 0666);
        for(int i=0; i<N; i++){
                semctl(sem_w, i, SETVAL, 1);
        }

	buff = (char **)malloc(N*SIZE);	

	sigfillset(&set);
	act.sa_sigaction = handler;
	act.sa_mask = set;
	act.sa_flags = 0;

	sigaction(SIGINT, &act, NULL);

	pthread_t tid[N];
	for(int i=0; i<N; i++){
		thread_args *args = (thread_args *)malloc(sizeof(thread_args));

		args->me = i;
		args->my_file = fd[i];
		pthread_create(&tid[i], NULL, thread_func, (void *)args);
	}

	struct sembuf oper;
	int i = 0;
	while(1){

		char *str;

		oper.sem_num = i;
	        oper.sem_op = -1;
        	oper.sem_flg = SEM_UNDO;

        	semop(sem_w, &oper, 1);

		printf("\n[MAIN THREAD] inserisci stringa: ");
		scanf("%ms", &str);
		buff[i] = str;
//		fflush(stdin);

		oper.sem_num = i;
                oper.sem_op = 1;
                oper.sem_flg = SEM_UNDO;

                semop(sem_r, &oper, 1);

		i = (i+1)%N;
	}

	return 0;
}
