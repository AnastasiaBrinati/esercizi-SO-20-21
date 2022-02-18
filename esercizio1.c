#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int N = 3;
int W = 4;


int main(int argc, char *argv[]){
	
	int pid[N];
	int status;
	struct flock lock1;
	memset(&lock1, 0, sizeof(lock1));
	
	
		lock1.l_type = F_WRLCK;
		lock1.l_whence = SEEK_SET;
		lock1.l_start = 0;
		lock1.l_len = 0;
		lock1.l_pid = 0;
	
	
	int fd = open("prova.txt", O_WRONLY | O_RDONLY | O_CREAT | O_TRUNC, 0666);
	
	for(int i=0; i<N; i++){
	
		pid[i] = fork();
		if(pid[i] == -1){
			printf("Errore nella fork\n");
			exit(0);
		}
		
		
		if(pid[i] == 0){
		
			char text[10];
			sprintf(text, "%d\n", (int)getpid());
			
			if(fcntl(fd, F_SETLK, &lock1)==-1){
				//error
			}
			else{
				write(fd, text, strlen(text));
				
				lock1.l_type = F_UNLCK;
				
				if(fcntl(fd, F_SETLK, &lock1)==-1){
				//error
				}
			}
			exit(getpid());						
		}
		
	}
	
	for(int i=0; i<N; i++){
		wait(&status);
		printf("Terminato processo con id: %d\n", status>>8);
	}
	
	close(fd);
	
	return;
	
	
}
