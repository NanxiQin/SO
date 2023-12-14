#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/mman.h>

#define M 2
#define MAX_BUFFER 1024 /* tama~no del buffer */
sem_t *full; 
sem_t *empty; 
sem_t *eating;
int finish = 0;
int* ration=0;
void putServingsInPot(int servings)
{
	printf("Cooker%d\n",*ration);
	sem_wait(empty);
	*ration=servings;
	printf("Put Servings\n");
	sem_post(eating);
	sem_post(full);
}

void cook(void)
{
	while(!finish) {
		putServingsInPot(M);
	}
}

void handler(int signo)
{
	if (signo==SIGTERM ||signo==SIGINT )
	finish = 1;
}

int main(int argc, char *argv[])
{

	int shd;
	shd = shm_open("/BUFFER", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
	ftruncate(shd, sizeof(int));
	ration = (int*) mmap(NULL,sizeof(int),PROT_READ|PROT_WRITE, MAP_SHARED, shd, 0);
	full= sem_open("/FULL", O_CREAT, 0700, 0);
	empty= sem_open("/EMPTY", O_CREAT, 0700,1);
	eating = sem_open("/EATING", O_CREAT, 0700,0);
	cook();
	
	munmap(ration, sizeof(int));
	shm_unlink("/BUFFER");
	sem_close(full);
	sem_close(empty);
	sem_unlink("/FULL");
	sem_unlink("/EMPTY");
	sem_unlink("/EATING");
	
	return 0;
}
