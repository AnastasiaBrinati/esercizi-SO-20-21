#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[]){
	
	int i;
	int len_buff = strlen(argv[1]);
	char buffer_stack[len_buff];
	
	if(argc<2){
		printf("Chiamare una stringa insieme al nome 							dell'eseguibile\n");
		return 1;
	}
	
	for(i=0; i<len_buff; i++){
		buffer_stack[i] = *(argv[1]+i);	
	}
	//memcpy equivalente
	
	for(i=0; i<len_buff; i++){
		printf("Char nello stack: %c \n", *(buffer_stack+i));
		//printf("Ecco la stringa: %s \n", buffer_stack);
	}
	
	for(i=0; i<len_buff; i++){
		buffer_stack[i] = *(argv[1]+len_buff-i-1);
	}
	
	for(i=0; i<len_buff; i++){
		printf("Char nello stack al contrario: %c \n", 				*(buffer_stack+i));
	}
	//oppure printf("Ecco la stringa: %s \n", buffer_stack);
	
	return 0;	
}
