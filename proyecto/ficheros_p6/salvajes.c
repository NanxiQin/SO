#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/mman.h>
#define NUMITER 8
sem_t *empty; 
sem_t *full; 
sem_t *eating;
int *ration;
int getServingsFromPot(void)
{
	sem_wait(eating);
	--(*ration);
	if(*ration<=0){
	printf("%d\n",*ration);
	  sem_post(empty);
	  sem_wait(full);
	}
	sem_post(eating);
}

void eat(void)
{
	unsigned long id = (unsigned long) getpid();
	printf("Savage %lu eating\n", id);
	sleep(rand() % 5);
}

void savages(void)
{
	for (int i = 0; i < NUMITER; i++){
		getServingsFromPot();
		eat();
	}
}

int main(int argc, char *argv[])
{
	int shd;
	shd = shm_open("/BUFFER", O_RDWR, 0);
	ration =  (int* )mmap(NULL,sizeof(int),PROT_READ|PROT_WRITE, MAP_SHARED, shd, 0);
	empty = sem_open("/EMPTY", 0);
	full = sem_open("/FULL",0 );
	eating = sem_open("/EATING", 0);
	
	savages();
	
	munmap(ration, sizeof(int));
	//shm_unlink("/BUFFER");
	sem_close(full);
	sem_close(empty);

	//sem_unlink("/EATING");
	return 0;
}
