#include <unistd.h>

#DEFINE N 4
#DEFINE BUFF_SIZE 2048


void consumatore(int key){
	
	char = buff_output;
	
	//perch[ esiste gia'
	int md = shmget(key, BUFF_SIZE, 0);
	
	buff_output = shmat(md, NULL, 0);
	printf(buff_output);
	
	shmdt(buff_output);
	
}

void produttore(int key){
	

	char *buff_input;
	int md;

	
	md = shmget(key, BUFF_SIZE, 0666);
	//write buff_input on mem
	buff_input = +shmat(md, NULL, 0);
	
	printf("Inserisci stringa per il mio successore\n");
	scanf("%[^\n]", &buff_input);
	
	
	shmdt(buff_input);
	
	
}

int main(int argc, char *argv[]){

	int key = 32
	
	int pid=fork();
	
	if(pid==0){
	
		produttore(key);
	
	}
	
	wait();
	consumatore(key);
	
	

	
}
