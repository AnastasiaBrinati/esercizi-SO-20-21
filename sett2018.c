#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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

#define SIZE 4096

char *values;
int fd;

int sem_a;
int sem_b;

void *thread_funcA(void *arg){
	
	int i = (int)arg;
	struct sembuf oper;

	oper.sem_num = 0;
	oper.sem_op = -1;
	oper.sem_flg = SEM_UNDO;

	semop(sem_a, &oper, 1);
	
	char *val;
	printf("Sono il thread A %d, leggo:", getpid());
	scanf("%s", val);
	*values = val;

	oper.sem_num = i;
        oper.sem_op = 1;
        oper.sem_flg = SEM_UNDO;

        semop(sem_b, &oper, 1);

}

void *thread_funcB(void *arg){

	int i = (int)arg;
	struct sembuf oper;
	
	oper.sem_num = i;
        oper.sem_op = -1;
        oper.sem_flg = SEM_UNDO;

        semop(sem_b, &oper, 1);

	printf("Sono il thread B %d, ho ricevuto: %s", getpid(), *values);	
	dprintf(fd, "%s \n", *values);

	//oppure fai: FILE* file = fdopen(fd, ...); fprintf(file, ...);
	
	oper.sem_num = 0;
        oper.sem_op = 1;
        oper.sem_flg = SEM_UNDO;

        semop(sem_b, &oper, 1);


}

int main(int argc, char *argv[]){
	
	if(argc<3){
		return;
	}

	int N = atoi(argv[2]);
	char *file_name = argv[1];

	fd = open(file_name, O_CREAT | O_RDWR, 0666);
	values = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0);

	sem_a = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
	semctl(sem_a, 0, IPC_RMID, 1);
	sem_a = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);


	sem_b = semget(IPC_PRIVATE, N, IPC_CREAT | 0666);
        semctl(sem_a, 0, IPC_RMID, 1);
        sem_b = semget(IPC_PRIVATE, N, IPC_CREAT | 0666);

	semctl(sem_a, 0, SETVAL, 1);
	for(int i=0; i<N; i++){
		semctl(sem_b, i, SETVAL, 0);
	}

	pthread_t tidA[N];
	for(int i=0; i<N; i++){
		pthread_create(&tidA[i], NULL, thread_funcA,(void*)i);
	}

	int pid = fork();
	if(pid==0){
		
		pthread_t tidB[N];
	        for(int i=0; i<N; i++){
        	        pthread_create(&tidB[i], NULL, thread_funcB, (void *)i);
        	}


	}

}
