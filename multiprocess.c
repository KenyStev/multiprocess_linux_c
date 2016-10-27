#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int foo(const char *whoami)
{
        printf("I am a %s.  My pid is:%d  my ppid is %d\n",
                        whoami, getpid(), getppid() );
        return 1;
}

int main(void)
{
        int n= 10;
        int i=0;
        int status=0;

        int x = 5;

        printf("Creating %d children\n", n);
        foo("parent");
        for(i=0;i<n;i++)
        {
                pid_t pid=fork();

                if (pid==0) /* only execute this if child */
                {
                        foo("child");
                        for(i=0; i< 5; i++);
                        {
                                x++;
                                printf("x value in for: %d ", x);
                        }

                        exit(0);

                }
                wait(&status);  /* only the parent waits */
        }

        x = 100 * x;
        printf("x value in main: %d ", x);
}