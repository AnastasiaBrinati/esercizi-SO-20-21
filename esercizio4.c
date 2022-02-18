#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


//buffer allocato nell'heap
char *buffer_dinamico;

int main(int argc, char *argv[]){

	char *buff;
	int pid;
	int status;
	
	printf("Stringa: ");
	scanf("%ms[^\n]", &buff);
	
	pid = fork();
	
	if(pid==-1){
		printf("Errore nella fork()\n");
		exit(-1);
	}
	
	if(pid==0){
		printf("\nSono il figlio: %s, con pid: %d\n", buff, getpid());
		exit(0);
	}
	else{	
		wait(&status);
		printf("Sono il padre: %s, con pid: %d\n", buff, getpid());		
	}
	return 0;
}
