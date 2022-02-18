#include <unistd.h>

int main(int argc, char *argv[]){

	char buffin[1024];
	char buffout[1024];
	int status;

	int file_name = open(prova, RDWR);
	
	if(file_name == -1){
		printf("error");
		return;
	}
	//i due processi condividono la sessione, quindi 
	//condividono il descrittore, ma anche l'indice di lettura
	
	int pid = fork();
	if(pid == -1){
		printf("error");
		return;	
	}
	
	
	if(pid == 0){
		
		printf("Sono il figlio");
		scanf("%m[^\n]", &buffin);
		write(prova, buffin, 1024);
		//anche se superfluo perche' viene liberato comunque
		free(buffin);
		exit(0);
	
	}

	else{
		wait(&status);
		//prima torno all-inizio:
		lseek(file_name, 0, 1024);
		read(prova, buffout, 1024);
		printf("Ecco cosa ha letto mio figlio: %s",buffout);
	}

}
