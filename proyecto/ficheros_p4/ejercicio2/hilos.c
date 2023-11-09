#include <stdlib.h>
#include <stdio.h>
#include <pthread.h> //hilos

#define NTH 45
struct prop //variables para el hilo
{
	int n;
	char prio;
};

void *thread_usuario(void *arg) //función del hilo
{
	struct prop *p = (struct prop *)(arg); //casting al struct
	printf("Thread %d with id %ld prio %c\n", p->n, (long)pthread_self(), p->prio);
	free(arg);
	pthread_exit(0);
}

int main(int argc, char *argv[])
{
	pthread_attr_t attr; //atributo
	pthread_attr_init(&attr); //inicialización por defecto de los atributos para el hilo
	
	pthread_t threads[NTH]; //hilos
	struct prop *args[NTH]; //aray estático de punteros a un struct
	
	for (int i= 0; i < NTH; ++i)
	{
		args[i] = malloc(sizeof(struct prop)); //reservar mem dinamica para el strcut
		args[i]->n = i;
		//printf("%d \n", i); debug
		if (i % 2 == 0)
			args[i]->prio = 'P';
		else
			args[i]->prio = 'N';
		pthread_create(&threads[i], &attr, thread_usuario, args[i]);
	}

	for (int i = 0; i < NTH; ++i)
	{ // procesar en orden de creación
		pthread_join(threads[i], NULL);
	}
	return 0;
}
