#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
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
#include <sys/shm.h>

#define PAGE_SIZE 4096

int sem_w;
int sem_r;

int *values;

void reader(int i, key_t key){

	struct sembuf oper;

	while(1){

	//	int ds_shm = shmget(key, 0, 0);

                oper.sem_num = 0;
                oper.sem_op = -1;
                oper.sem_flg = SEM_UNDO;

                semop(sem_r, &oper, 1);

       //       void *addr = shmat(ds_shm, NULL, SHM_EXEC);
		
		printf("Sono il processo %d, inserire un intero:", i);
		int val;
		scanf("%d", val);
		*values = val;
  		
  		oper.sem_num = 0;
                oper.sem_op = 1;
                oper.sem_flg = SEM_UNDO;

                semop(sem_w, &oper, 1);

//              shmdt(addr);

	}

}

void main(int argc, char *argv[]){

	if(argc < 3){
		return;
	}

	int N = strtol(argv[2], NULL, 10);

	FILE *file;
	int fd;

	key_t key = 1357;
//	int ds_shm = shmget(key, sizeof(int), IPC_CREAT | 0666);
//	void *addr = shmat(ds_shm, NULL, SHM_EXEC);
	
	values = mmap(NULL, PAGE_SIZE, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_SHARED, 0, 0);

	sem_w = semget(IPC_PRIVATE,1,IPC_CREAT|0666);
	semctl(sem_w,1,IPC_RMID,NULL);
	sem_w = semget(IPC_PRIVATE,1,IPC_CREAT|0666);

	sem_r = semget(IPC_PRIVATE,1,IPC_CREAT|0666);
	semctl(sem_r,1,IPC_RMID,NULL);
	sem_r = semget(IPC_PRIVATE,1,IPC_CREAT|0666);

	semctl(sem_w, 0, SETVAL, 0);
	semctl(sem_r, 0, SETVAL, 1);
	
	fd = open(argv[1], O_CREAT | O_RDONLY | O_WRONLY | O_TRUNC,0666);
	file = fdopen(fd,"w+");

	pid_t pid[N];
	for(int i=0; i<N; i++){
		pid[i] = fork();

		if(pid[i]==-1){
		}

		if(pid[i]==0){
			reader(i, key);
		}
	
	}
	struct sembuf oper;

	while(1){
		
//		int ds_shm = shmget(key, 0, 0);
	        
		oper.sem_num = 0;
		oper.sem_op = -1;
		oper.sem_flg = SEM_UNDO;

		semop(sem_w, &oper, 1);
		
//		void *addr = shmat(ds_shm, NULL, SHM_RDONLY);
		
		fprintf(file,"%d ", *values);
		//if(ret == -1 && errno == EINTR){}
		fflush(file);		
		
		oper.sem_num = 0;
		oper.sem_op = 1;
		oper.sem_flg = SEM_UNDO;

		semop(sem_r, &oper, 1);
		
///		shmdt(addr);	
	}

	
	return;
}
