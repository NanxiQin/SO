#include <stdio.h>	//printf
#include <stdlib.h> //exit
#include <dirent.h> //directorio
#include <unistd.h> //getopt,fork
#include <string.h>
#include <err.h>
#include <errno.h>
#include <sys/wait.h> //fork
#include <sys/stat.h> //lstat
// #include <sys/types.h>
// #include <fcntl.h>

struct options
{
	char *progname;
	int recurse;
};

struct options opt;

void usage(void)
{
	printf("%s [options] [dirname]\n\n", opt.progname);
	printf("lists the contents of dirname (default .)\n");
	printf("options:\n"
		   "\t-h:\tshow this help\n"
		   "\t-R:\trecursive\n");
}

/* apartado b */
void list_dir(char *name)
{
	struct dirent *de;
	DIR *d = opendir(name);
	if (d == NULL)
	{
		exit(EXIT_FAILURE);
	}
	while ((de = readdir(d)) != NULL) // se recorren los contenidos que hay en el directorio
	{
		printf("%s\n", de->d_name);
	}
	closedir(d);
}

/* apartado c */
void process_recurse(char *dirname, char *name)
{
	pid_t pid;
	char path[PATH_MAX];
}

/* apartado c */
void list_dir_recurse(char *name)
{
	DIR *d = opendir(name);
	if (d == NULL)
		exit(EXIT_FAILURE);

	struct dirent *de;
	pid_t pid;
	while ((de = readdir(d)) != NULL) // recorre todos los ficheros del directorio actual
	{								  // se recorren los contenidos que hay en el directorio
		printf("%s\n", de->d_name);

		if (de->d_type == DT_DIR && strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0)
		{
			char nameBuffer[1024];
			nameBuffer[0] = '\0';
			// va concatenando path
			strcat(nameBuffer, name);
			strcat(nameBuffer, "/");
			strcat(nameBuffer, de->d_name);

			pid = fork();

			if (pid == 0)//si es hijo
			{
				printf("%s:\n", nameBuffer);
				list_dir_recurse(nameBuffer);
				return; // no tiene que seguir procesando lo que falta del while si es hijo
			}
		}
	}

	while (wait(NULL) != -1);
	closedir(d);
}

int main(int argc, char **argv)
{
	char *dirname = ".";
	int o;
	opt.progname = argv[0];
	opt.recurse = 0;

	/* Apartado a: procesar opciones con getopt */

	int ord;

	while ((ord = getopt(argc, argv, "hR")) != -1)
	{
		switch (ord)
		{
		case 'h':
			usage();
			exit(EXIT_SUCCESS);
			break;
		case 'R':
			opt.recurse = 1;
			break;
		default:
			exit(EXIT_FAILURE);
		}
	}

	/* There should be one extra argument */
	if (optind < argc)
	{
		dirname = argv[optind];
	}

	/********************************************/

	if (opt.recurse)
	{
		printf("%s:\n", dirname);
		list_dir_recurse(dirname);
	}
	else
		list_dir(dirname);

	return 0;
}
