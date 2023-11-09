#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h> //FLAGS fichero posix
//#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(void)
{
    int fd1, i, pos;
    char c;
    char buffer[6];
    fd1 = open("output.txt", O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
    for (i = 1; i < 10; i++)
    {
        write(fd1, "00000", 5); //lo realiza cada padre siempre antes del hijo
        pos = lseek(fd1, 0, SEEK_CUR); //guarda la pos en el momento después de escribir el padre
        if (fork() == 0)
        {
            /* Child */
            sprintf(buffer, "%d", i * 11111); //escribir en el buffer con formato int
            lseek(fd1, pos, SEEK_SET); //se asegura que se sitúa justo en la pos después del padre para escribir
            write(fd1, buffer, 5); //escribir en el fichero lo que hay en el buffer (sin el fin de línea)
            close(fd1); //para el hijo hay que cerrar el fichero, ya que este hijo se ha dejado de usar
            exit(0);
        }
        else
        {
            /* Parent */
            while (wait(NULL) != -1); // esperar a su hijos
           // lseek(fd1, 5, SEEK_CUR); //saltar 5 bytes (se ponen 5 null)
        }
    }

    lseek(fd1, 0, SEEK_SET);//colocarlo al principio
    printf("File contents are:\n");
    while (read(fd1, &c, 1) > 0)
        printf("%c", (char)c);
    printf("\n");
    close(fd1);
    exit(0);
}
