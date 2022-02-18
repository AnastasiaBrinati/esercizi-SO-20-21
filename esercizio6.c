#include <stdio.h>
#include <stdlib.h>
#include <string.h>

volatile unsigned_int sentinella = 0;

void thread_function(){
	
	
	printf("Ciao sono il thread\n");
	printf("Sto terminando\n");
	sentinella = 1;
	
}

void main(int argc, char *argv[]){
	
	pthread_t thread;
	int i;
	
	i = pthread_create(&tid, NULL,
			thread_function,
			NULL);
	
	if (i) {
      		printf("cannot create thread for error %d\n", i);
     		exit(-1);
      	}
      	
      	while(1){
      		if(sentinella){
      			printf("Parent thread is terminating\n");
      			break;
      		}
      	}
	
		
}
