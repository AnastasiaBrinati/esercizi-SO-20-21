//esame 17.10.2019
//creo tanti thread quanti sono i parametri di ingresso
//poi il main thread prende constantemente stringhe e a ciclo li passa ai thread che poi stampano su terminale le stringhe passate
 




#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <semaphore.h>
struct parametro
{
	char path[128];
	int i;
}typedef parametro;
sem_t l[10];
sem_t s[10];
char buffer[1024][1024];
void * thread(void * param){
	parametro * p=(parametro *) param;
	printf("io sono il numero %d con path: %s\n",p->i,p->path);
	while(1){
		printf("vado in pausa per il token %d\n",p->i );
		sem_wait(s+(p->i));
		printf("posso leggere: %s\n",buffer[p->i]);
		sem_post(l+(p->i));
	}
	pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
	if (argc<2)
	{
		return 0;
	}
	for (int i = 0; i < argc; i++)
	{
		if(sem_init(l+i,0,1)==-1) return 0;
		if(sem_init(s+i,0,0)==-1) return 0;
	}
	pthread_t tid[argc];
	parametro p[argc];
	for(int i = 1; i < argc; i++)
	{
		memcpy(p[i-1].path,argv[i],strlen(argv[i])+1);
		p[i-1].i=i-1;
		pthread_create(tid+i-1,NULL,&thread,p+i-1);
		
	}
	int i=0;
	char * terminazione="quit\0";
	char * stringa="";
	while(strcmp(stringa,terminazione))
	{
		printf("%d inserire stringa: \n",i%(argc-1));
		scanf("%ms",&stringa);
		sem_wait(l+(i%(argc-1)));
		memcpy(buffer[i%argc],stringa,strlen(stringa));
		sem_post(s+(i%(argc-1)));
		i++;
	}	
/*	for (int i = 0; i < argc; i++)
	{
		pthread_join(tid[i],NULL);
	}
*/	return 0;
}
