#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //sleep
#include <signal.h>
#include <sys/wait.h> //fork

/*programa que temporiza la ejecución de un proceso hijo */

int main(int argc, char **argv)
{
    for (int i = 1;; i++)
    {
        printf("%d : Inside son function\n", i);
        sleep(1); // Delay for 1 second
    }
    return 0;
}
