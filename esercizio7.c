#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void thread_function1(){

//non potevo passare direttamente il valore allocando
//dinamicamente la stringa nel main
	
	char *pointer1 = string;
	printf("Ciao sono il thread1\n");
	scanf("%m[^\n]", pointer);
	
	pthread_exit((void*) string);
	
}

void thread_function2(char *string){

	char *pointer2 = string;
	printf("Ciao sono il thread2\n");
	printf("Ecco la stringa:\n", );
	
	pthread_exit((void*) 0);
}

void main(int argc, char *argv[]){
	
	pthread_t tid1;
	pthread_t tid2;
	char *string;
	
	int v[2];
	
		v[0] = pthread_create(&tid1, NULL,
			thread_function1,
			NULL);
			
		
	
		if (v[0]) {
      		printf("cannot create thread for error %d\n", i);
     		exit(-1);
      		}
      		
      		if(pthread_join(tid1, &string)){
 			return 1;
      		}
      		
      		sleep(1000);
      		
      		v[1] = pthread_create(&tid2, NULL,
			thread_function2,
			string);
	
		if (v[1]) {
      		printf("cannot create thread for error %d\n", i);
     		exit(-1);
      		}	
	}
	
	free(string);
		
}
