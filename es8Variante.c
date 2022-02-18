
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <semaphore.h>

#define SIZE 4096

sem_t sem_W[1];
sem_t sem_R[1];

void producer(int key){

        int ds = shmget(key, SIZE, 0);
        char *addr = (char *)shmat(ds, NULL, 0);

	while(1){
		
		printf("attendo il semaforo per la scrittura\n");

		sem_wait(sem_W);

		printf("ho atteso il semaforo per la scrittura\n");
                scanf("\nLasciare un msg per il consumatore: %[^\n]", addr);
		
		sem_post(sem_R);
		printf("\nHo rilasciato il semaforo per la lettura\n");

	}


}

void consumer(int key){

        int ds = shmget(key, SIZE, 0);
        char *addr = (char *)shmat(ds, NULL, 0);


	while(1){
		
		printf("attendo il semaforo per la lettura\n");

		sem_wait(sem_R);
		
		printf("ho atteso il semaforo per la lettura\n");

                printf("\nEcco il msg del produttore: %s", addr);

                sem_post(sem_W);

	}

}

void main(int argc, char *argv[]){

        int key = 3435;
        int ds = shmget(key, SIZE, IPC_CREAT|IPC_EXCL|0666);


        if(sem_init(sem_W, 1, 1)==-1){
			printf("errore init\n");
	}
 	
	if(sem_init(sem_R, 1, 0)==-1){
			printf("errore init\n");
	}


	int prod = fork();

        if (prod == -1){
                printf("fork on producer error\n");
                exit(-1);
        }

        if (prod == 0){
                printf("ciao sono il producer\n");
                producer(key);
                exit(1);
        }

        int cons = fork();

        if (cons == -1){
                printf("fork on consumer error\n");
                exit(-1);
        }

        if (cons == 0){
                printf("ciao sono il consumer\n");
                consumer(key);
                exit(1);
        }

        int status;
        for(int i=0; i<2; i++){ wait(&status); }

}
