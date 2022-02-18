#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/shm.h>

#define SIZE 4096

int sem_W;
int sem_R;

void producer(int key){

	int ds = shmget(key, SIZE, 0);
	char *addr = (char *)shmat(ds, NULL, 0);
	
	struct sembuf oper;
	
//	while(1){

                oper.sem_num = 0;
                oper.sem_op = -1;
                oper.sem_flg = SEM_UNDO;
                semop(sem_W, &oper, 1);

                scanf("\nLasciare un msg per il consumatore: %[^\n]", &addr);

                oper.sem_num = 0;
                oper.sem_op = 1;
                oper.sem_flg = SEM_UNDO;
                semop(sem_R, &oper, 1);

//        }

}

void consumer(int key){

	int ds = shmget(key, SIZE, 0);
	char *addr = (char *)shmat(ds, NULL, 0);
	
	struct sembuf oper;

//	while(1){
		
		oper.sem_num = 0;
        	oper.sem_op = -1;
        	oper.sem_flg = SEM_UNDO;
		semop(sem_R, &oper, 1);
		
		printf("\nEcco il msg del produttore: %s", addr);
		
		oper.sem_num = 0;
		oper.sem_op = 1;
		oper.sem_flg = SEM_UNDO;
		semop(sem_W, &oper, 1);

//	}

}

void main(int argc, char *argv[]){
	
	int key = 3435;
	int sem_key = IPC_PRIVATE;

	int ds = shmget(key, SIZE, IPC_CREAT|IPC_EXCL|0666);
	
	
	sem_W = semget(sem_key,1,IPC_CREAT|0666);
	semctl(sem_W,1,IPC_RMID,NULL);
	sem_W = semget(sem_key,1,IPC_CREAT|0666);

	sem_R = semget(sem_key,1,IPC_CREAT|0666);
	semctl(sem_R,1,IPC_RMID,NULL);
	sem_R = semget(sem_key,1,IPC_CREAT|0666);

	
        if(sem_W == -1){
                printf("semget error\n");
                exit(-1);
        }

        semctl(sem_W,0,SETVAL,1);
       

        if(sem_R == -1){
                printf("semget error\n");
                exit(-1);
        }

       	semctl(sem_R,0,SETVAL,0);



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
	for(int i=0; i<2; i++){	wait(&status); }
	semctl(sem_W,IPC_RMID,0);
	semctl(sem_R,IPC_RMID,0);
}

