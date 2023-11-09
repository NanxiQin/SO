#include <stdio.h>
#include <stdlib.h>
#include <signal.h> //señales
#include <unistd.h>	  //getopt,fork,sleep
#include <sys/wait.h> //fork

/*programa que temporiza la ejecución de un proceso hijo */

void signal_handler(int signum) //PROCESAR SEÑALES
{
	if (signum == SIGALRM) //SI ES DE TIPO ALARMA
	{
		printf("Child has killed\n");
		kill(0, SIGKILL); //mandar señar de kill al proceso hijo (0)
	}
}

int main(int argc, char **argv)
{
	pid_t pid;
	pid = fork();

	if (pid == 0)
	{
		execvp(argv[1], argv); //ejecuta otro programa cuyo nombre se pasa como arg
		// return 0;
	}
	else if (pid > 1) //si es padre
	{
		signal(SIGALRM, signal_handler); // Register signal handler
		alarm(5);						 // Scheduled alarm after 2 seconds
		wait(NULL); //necesario para que el proceso hijo termine con la alarma si es infinito
		printf("Child has finished\n"); //si no se había interrumpido por la alarma
	}
	return 0;
}
