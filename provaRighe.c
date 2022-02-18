#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void *figlio(FILE *file){

	int fd = open("output.txt", O_CREAT | O_RDWR, 0666);
	FILE *output_file = fdopen(fd, "w");
	char *line;
	char *token;
	int i=0;

	while(fscanf(file, "%m[^\n]", &line)!=EOF){


		fgetc(file);
		printf("[riga %d]: %s\n", i, line);
		
		token = strtok(line, " ");
		while(token != NULL){
			if(strlen(token)>5){
				fprintf(output_file, "%s ", token);
			}

			token=strtok(NULL, " ");
		}
		
		fflush(output_file);
		fprintf(output_file, "%s\n", "");
		fflush(output_file);
		i++;
		free(line);
	}


	fclose(file);	

}

int main(int argc, char *argv[]){

	if(argc<2){
		return 0;
	}

	//int fd = open(argv[1], O_CREAT | O_RDWR, 0666);
	FILE *file = fopen(argv[1], "r");
	
	pid_t pid=fork();
	if(pid==0){
		figlio(file);
	}
	while(1);
	
	return 0;
}
