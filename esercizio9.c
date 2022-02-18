#include <unistd.h>

int main(int argc, char *argv[]){

	char buff[1024];
	//char buffout[1024];
	//int status;
	//int tid;
	char *line;
	char *c;

	int file_r = open(provaLettura, O_CREAT | O_RDONLY);
	int file_w = open(provaScrittura, O_CREAT | O_RDWR);
	
	//int f = fopen();
	//fsync(fileno(f));
	
	
	if(file_r == -1){
		printf("error");
		return;
	}
	
	if(file_w == -1){
		printf("error");
		return;
	}
	
	
	
	while(fscanf(file_r, "%s[^\n]", buff) != EOF){
	
		c = strtok(buff, " ");
	}
	
	
	

}
