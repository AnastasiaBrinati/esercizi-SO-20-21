#include<stdio.h>
#include<stdlib.h>

int main(){

	char c;
	char *msg = &c;
	//msg = malloc(1024);

	scanf("%ms", &msg);
	printf("%s", msg);
}

