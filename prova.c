#include <string.h>
#include <stdio.h>
#include <stdlib.h>


void main(int argc, char *argv[]){

	char str[1024];
	char *buff[2];
	strcpy(str, argv[1]);

	int j = 0;
	printf("Ecco la stringa data in input: %s\n", str);

	while(str[j]!='\0'){
                
		printf("str[%d] : %c\n", j, str[j]);

		if(str[j]=='a'){
                        *(buff[1]+j) = ' ';
                }
                else{
                        *(buff[1]+j) = str[j];
                }
                j++;
                
                //my_string = my_string+1;
        }

	printf("Ecco la stringa risultante: %s", buff[1]);


}
