#include <string.h>
#include <stdio.h>
#include <stdlib.h>
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

int sem_a;
int sem_output;

char **buff;
char *file_name = "output.txt";
char *command;

typedef struct _thread_args{
	int me;
	char *my_string;
}thread_args;

void *OUTPUT(void *arg){

	int N = (int)arg;
	int fd = open(file_name, O_CREAT | O_TRUNC | O_RDWR, 0666);

	struct sembuf oper;

        oper.sem_num = N+1;
        oper.sem_op = -1;
        oper.sem_flg = SEM_UNDO;

	printf("[OUTPUT thread, sto scrivendo la stringa %s", *(buff+N));

	semop(sem_a, &oper, 1);

	dprintf(fd, "%s \n", *(buff+N));
	fflush(fd);
	
	oper.sem_num = N;
        oper.sem_op = 1;
        oper.sem_flg = SEM_UNDO;

        semop(sem_a, &oper, 1);

}

void *thread_func(void *arg){

	thread_args *args = (thread_args *)arg;
	
	int i = args->me +1;


	struct sembuf oper;

        oper.sem_num = i;
        oper.sem_op = -1;
        oper.sem_flg = SEM_UNDO;

	//serve un altro semaforo su cui attendere per scrivere su buff+i

	printf("[THREAD %d], sto scrivendo sulla stringa %s",i, *(buff+i-1));
        semop(sem_a, &oper, 1);

	int j = 0;
	char *str;
	char *my_string;
	strcpy(my_string, args->my_string);
	strcpy(str, *(buff+i-1));
	strcpy(*(buff+i), "");
	while(str!=NULL){
		if(str==my_string){
			strcat(*(buff+i)+j, " ");	
		}
		else{
			strcat(*(buff+i)+j, str);
		}
		j++;
		str = str+1;
		my_string = my_string+1;
	}
	
//segnalo al prossimo
	oper.sem_num = i+1;
        oper.sem_op = 1;
        oper.sem_flg = SEM_UNDO;

        semop(sem_a, &oper, 1);
//segnalo al precedente
	oper.sem_num = i-1;
        oper.sem_op = 1;
        oper.sem_flg = SEM_UNDO;

        semop(sem_a, &oper, 1);

}

void *handler(){
	system("cat output.txt");
}

int main(int argc, char *argv[]){

	if(argc<2){
		printf("usage: gcc sett2019.c s1 .. sn\n");
		return 0;
	}
	

	int N = argc-1;
	buff = (char **)malloc((N+1)*sizeof(char *));
	
	/*
	for(int i=0; i<N+1; i++){
		*buff = (char *)malloc(SIZE);
        }
	*/

	//strcpy(command, "cat %s", file_name);
	signal(SIGINT, handler);

	sem_a = semget(IPC_PRIVATE, N+2, IPC_CREAT | IPC_EXCL);
        semctl(sem_a, 0, SETVAL, 1);
	//semctl(sem_a, 1, SETVAL, 1);
	for(int i=1; i<N+2; i++){
		semctl(sem_a, i, SETVAL, 0);
	}
//	sem_output = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL);
//	semctl(sem_output, 0, SETVAL, 1);

	thread_args *args = (thread_args *)malloc(sizeof(thread_args));
	pthread_t tid;
	for(int i=0; i<N; i++){
		args->me = i;
		args->my_string = argv[i+1];
		pthread_create(&tid, NULL, thread_func, (void *)args);
	}


	pthread_t out;
	pthread_create(&out, NULL, OUTPUT, (void *)N);

	while(1){
		
		struct sembuf oper;

		oper.sem_num = 0;
		oper.sem_op = -1;
		oper.sem_flg = SEM_UNDO;

		semop(sem_a, &oper, 1);
	
		char *str;
		printf("[MAIN thread], inserisci una stringa:");
		scanf("%ms", &str);
		//		fflush(STDIN);

		*buff = str;

		oper.sem_num = 1;
                oper.sem_op = 1;
                oper.sem_flg = SEM_UNDO;

                semop(sem_a, &oper, 1);


	}

	return 0;
}
