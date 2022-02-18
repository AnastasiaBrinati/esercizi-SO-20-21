#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>




void *inverter(FILE *file){

	FILE *file_asterisco = fopen("asterisco.txt", "w+");
	char *line;
	char *token;
	char *nekot;
	char c;
	int M;
	
	while(fscanf(file, "%m[^\n]", &line)!=EOF){
		fgetc(file);
		
		
		token = strtok(line, " ");
		
		while(token!=NULL){
		strcpy(nekot, token);
		
		M = strlen(nekot);
		
		for(int i=0; i<M/2; i++){
		
			c = nekot[i];
			nekot[i] = nekot[M-1];
			nekot[M-i] = c;
					
		}
		
		for(int i=0; i<M; i++){
			
			if(token[i] != nekot[i]){
				token[i] = '*';
			}	
			
		}
		
		fprintf(file_asterisco, "%s ", token);
		fflush(file_asterisco);
		
		token = strtok(NULL, " ");
		}
		
		free(line);
	}
	
	fclose(file);
	fclose(file_asterisco);
	
	

}

int main(int argc, char *argv[]){

	if(argc<3){
		printf("usage: gcc prog2.c file_name string1 .. stringN\n");
		return 0;
	}
	
	//int fd = open(argv[1], O_CREAT | O_RDWR | O_TRUNC, 0666);
	FILE *file = fopen(argv[1], "w");
	
	int N = argc-2;
	for(int i=0; i<N; i++){
		fprintf(file, "%s ", argv[i+2]);
		
	}
	
	pid_t pid;
	pid = fork();
	
	if(pid==-1){
		printf("fork() error\n");
		return 0;
	}
	if(pid==0){
		inverter(file);
		exit(1);
	}
	else{
		
		int status;
		
		while(1){
			wait(&status);
		}
	}
	
	return 0;
	
}
