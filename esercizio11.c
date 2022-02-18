#include <unistd.h>

#DEFINE N 4
#DEFINE BUFF_SIZE 2048

void thread_fun(char *buff){
	
	void *pointer;
	void *pointer_input;
	char buff_input[2048];
	
	printf("Inserisci stringa per il mio successore\n");
	scanf("%[^\n]", &buff_input);
	
	pointer = malloc(BUFF_SIZE);
	pointer_input = malloc(BUFF_SIZE);
	
}

int main(int argc, char *argv[]){

	int i;
	pthread_t thread;
	char *message;
	
	for(i = 0; i<N; i++){
		if(pthread_create(&thread, NULL, thread_fun, (void *)message) == 0){
			
			//dove il main attende i risultati delle f dei threads
			pthread_join(thread, (void **)&message);	
		}
		
	}
	
	printf(message);
	
	free(message);
	


}
