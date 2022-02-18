#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char *argv[]){

	if(argc<2){
		return 0;
	}
	
	int pid;
	int status;
	int forked=0;
	
	for(int i=1; i<argc; i++){
		
		pid= fork();
		
		if(pid==-1){
			printf("Errore\n");
			exit(-1);
		}
		
		if(pid==0){
			printf("\nSono il figlio %d\n", i);
			execlp("ls","ls", argv[i], NULL);
			
			//eseguito solo se fallisce
			exit(-1);
		}
		forked+=1;	
		
	}
	
	for(int i=0; i<forked; i++){
		wait(&status);
		printf("Sono il padre\n");
	}
}
