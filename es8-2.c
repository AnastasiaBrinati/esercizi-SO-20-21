#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>

#define SIZE 40

sem_t *sem_W[1];
sem_t *sem_R[1];


void producer(int key){

        int ds = shmget(key, SIZE, 0);
        void *addr = shmat(ds, NULL, 0);

	while(1){
		
		sem_wait(sem_W);

                scanf("%[^\n]", (char *)addr);
		
		sem_post(sem_R);

	}


}

void consumer(int key){

        int ds = shmget(key, SIZE, 0);
        char *addr = (char *)shmat(ds, NULL, 0);

	while(1){
		
		sem_wait(sem_R);

                printf("\nEcco il msg del produttore: %s", addr);

                sem_post(sem_W);

	}

}

void main(int argc, char *argv[]){

        int key = 3435;
        int ds = shmget(key, SIZE, IPC_CREAT|IPC_EXCL|0666);

	
	//named
	sem_W = sem_open("W", O_CREAT, 0666, 1);
	sem_R = sem_open("R", O_CREAT, 0666, 0);	


	int prod = fork();

        if (prod == -1){
                printf("fork on producer error\n");
                exit(-1);
        }

	 if (prod == 0){
                printf("ciao sono il producer\n");
                producer(key);
         }
	
	else{
        	printf("\nciao sono il consumer\n");
                consumer(key);
        }
}
