#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>



#define SIZE (10)

long *v;

sem_t * sem_free_slot, * sem_available_item;

void * producer(void){
       
	printf("ready to produce\n");

retry:

	sem_wait(&(sem_free_slot[0]));
	
	printf("");
	scanf("%l", &v);

	sem_post(&(sem_available_item[0]));

	goto retry;


}

void * consumer(void){

	long value;

	printf("ready to consume\n");

retry:
	sem_wait(&(sem_available_item[0]));
	value = v[0];

	printf("consumer got value %d\n",value);

	sem_post(&(sem_free_slot[0]));
	
	goto retry;
}

int main(int argc, char** argv){

	int prod, cons;
	int i;

	sem_free_slot = (sem_t*)mmap(NULL,sizeof(sem_t),PROT_READ|PROT_WRITE,MAP_ANONYMOUS|MAP_SHARED,0,0);
        if (sem_free_slot == NULL){
                printf("mmap error on free slot semaphores\n");
                exit(-1);

        }       

	if((sem_init(&(sem_free_slot[0]),1,1) == -1)){
			printf("cannot init free slot semaphore at iteration %d\n",i);
			exit(0);
	}	

	

	sem_available_item = (sem_t*)mmap(NULL,sizeof(sem_t),PROT_READ|PROT_WRITE,MAP_ANONYMOUS|MAP_SHARED,0,0);
        if (sem_available_item == NULL){
                printf("mmap error on available item semaphores\n");
                exit(-1);

        }       
	if(sem_init(&(sem_available_item[0]),1,0) == -1){
		printf("cannot init available item semaphores at iteration %d\n",i);
		exit(0);
	}
	

	v = (long*)mmap(NULL,sizeof(long),PROT_READ|PROT_WRITE,MAP_ANONYMOUS|MAP_SHARED,0,0);
        if (v == NULL){
                printf("mmap error\n");
                exit(-1);

        }       
	
	prod = fork();
	if (prod == -1){
		printf("fork on producer error\n");
		exit(-1);

	}	

	if (prod == 0){
		producer();
	}


	cons = fork();
	if (cons == -1){
		printf("fork on consumer error\n");
		exit(-1);

	}	

	if (cons == 0){
		consumer();
	}

	wait(NULL);
	exit(0);

}


