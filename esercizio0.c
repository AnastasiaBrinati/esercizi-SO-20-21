#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[]){
	
	char buff1[56];
	char *buff2;
	
	//char *ptobuff1 = buff1;
	
	printf("Buffer array: ");
	scanf("%ms", buff1);
	printf("\nBuffer pointer: ");
	scanf("%ms", &buff2);
	
	printf("\nEcco stringa nell'array: %s\n", buff1);
	printf("Ecco stringa nel pointer: %s\n", buff2);
	
	return 0;
}
