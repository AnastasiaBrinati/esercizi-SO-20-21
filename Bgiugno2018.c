#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <pthread.h>
#include <sys/sem.h>
#include <string.h>
#include <semaphore.h>
#include <unistd.h>

pthread_mutex_t M1;
pthread_mutex_t M2;
pthread_mutex_t M3;

int SIZE = 128;

void main(int argc, char *argv[]){

	if(argc < 3){
		printf("Numero di argomenti non sufficiente\n");
		return;
	}
	
	int N = atoi(argv[2]);
	if(N < 1){
		printf("Valore minore di 1\n");
	}

	pthread_mutex_init(&M1, NULL);
	pthread_mutex_init(&M2, NULL);
	pthread_mutex_init(&M3, NULL);
	//pthread_mutex_lock(&M3);

	int fd = open(argv[1],O_RDWR|O_CREAT,0666 );
	int forked = 0;
	int ds;
	int key;
	ds  = shmget(key, SIZE, IPC_CREAT | 0666);
	
	int pid[N];
	int i;
	for(i=0; i<N; i++){
		
		pid[i] = fork();
		if(pid[i]==-1){
			printf("Errore nel forkare il processo %d\n", i);
		}

		if(pid[i]==0){
		   
		  char *addr;
		  int buff;
		  addr = shmat(ds, 0, SHM_W);

//		   while(1){
			pthread_mutex_lock(&M1);
			
			printf("\nSono il figlio %d\n", i);
			scanf("%d", &buff);

			pthread_mutex_lock(&M2);
			memcpy(addr, buff, sizeof(buff));
			pthread_mutex_unlock(&M3);

			pthread_mutex_unlock(&M1);
			exit(1);
//		   }
		}else{
			forked++;
		}
	}
	
	char *addr;
	char *addr1;
	addr = shmat(ds, 0, SHM_R);
	

	while(forked>0){
		
		pthread_mutex_lock(&M3);
		
		addr1 = addr;
//		strcat(addr1, ' ');

		write(fd, addr1, strlen(addr1));
		
		pthread_mutex_unlock(&M2);
		forked--;
	}
}
