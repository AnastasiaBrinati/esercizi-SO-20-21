#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//buffer allocato nell'heap
char *buffer_dinamico;

int main(int argc, char *argv[]){
	
	printf("Inserire stringa: ");
	//consumiamo tutto cio' che si trova sullo
	//stndinput fino al terminatore di linea
	scanf("%ms", &buffer_dinamico);
	//perche' [not \n]???
	
	int i;
	//oppure conta man mano in un while != '\O'
	int len_buff = strlen(buffer_dinamico);
	//buffer allocato nello stack
	//ma devo farlo +1 per il terminatore di stringa
	char buffer_stack[len_buff];
	
	
	//era equivalente usare: memcpy(buffer_stack,
	//				 buffer_dinamico, len_buff);
	for(i=0; i<len_buff; i++){
		buffer_stack[i] = *(buffer_dinamico+i);	
	}
	
	//aggiungo il terminatore
	//se avessi usato strcpy sarebbe stato fondamentale
	
	for(i=0; i<len_buff; i++){
		printf("Char nello stack: %c \n", *(buffer_stack+i));
	}
	//puoi anche printare direttamente: printf("stringa: 
	// %s\n", buffer_stack);
	
	free(buffer_dinamico);
	
	return 0;	
}
