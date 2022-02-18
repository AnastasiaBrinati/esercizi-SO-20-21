#include <unistd.h>
#include <stdlib.h>

#DEFINE N 4

int main(int argc, char *argv[]){

	//char buff[1024];
	int status;
	
	int pid[N];
	int fd[N];
	

	int file_name = open(prova, O_CREAT | O_RDWR);
	
	if(file_name == -1){
		printf("error");
		return;
	}
	
	for(int i=0; i<N; i++)
	{
	
		pid[i]=fork();
		
		if(pid[i] == -1){
			printf("ERRORE NELLA FORK");
			exit(-1);
		}
		if(pid[i] == 0){
			fd[i] = dup(file_name);
			//buff = i*getpid();
			write(fd[i], getpid(), 24);
			exit(0);
		}
		
	}
	
	for(int i=0; i<N; i++){
	
		wait(&status);
	
	}
	
	//cosa e' fcntl
	
	close(file_name);
	
	//in windows e' lockFile, non bloccante, quindi per bloccarlo fare un while di attesa
}
