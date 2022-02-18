#include <unistd.h>

#DEFINE N 4
#DEFINE BUFF_SIZE 2048

#DEFINE S 5


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
	addr = +shmat(md, NULL, 0);
	
	printf("Inserisci stringa per il mio successore\n");
	scanf("%[^\n]", &buff_input);
	
	
	shmdt(buff_input);
	
	
}

int main(int argc, char *argv[]){

	if(argc < 2){
		return;
	}
	
	int pid[N];
	char *md[N];
	int key[N];
	void *buff[N];
	
	for(int i=0; i<N; i++){
	
		key = 2246+i
		pid[i]=fork()
		if(pif[i] == 0){
		
			md[i] = shmget(key[i], NULL, O_CREAT | 06666);
			buff[i] = shmat(md[i], NULL, 0)
			strcpy(buff[i], i);
			printf("Ecco l'int %d, %d\n", buff[i], i);
			shdmt(buff[i]);
		}
		
	}
	
	

	
}
