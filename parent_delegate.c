#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int  parse(char *line, char **argU)
{
	int cant=1;
	*argU = "./child_sort";
	*argU++;
     while (*line != '\0') {       /* if not the end of line ....... */ 
          while (*line == ' ' || *line == '\t' || *line == '\n')
          {
               	*line++ = '\0';     /* replace white spaces with 0    */
          		cant++;
          }
          *argU++ = line;          /* save the argvent position     */
          while (*line != '\0' && *line != ' ' && 
                 *line != '\t' && *line != '\n') 
               line++;             /* skip the argvent until ...    */
     }
    *argU = '\0';                 /* mark the end of argvent list  */
 	cant++;
 	return cant;
}

pid_t  execute(char **argU)
{
     pid_t  pid;

     if ((pid = fork()) < 0) {     /* fork a child process           */
          printf("*** ERROR: forking child process failed\n");
          exit(1);
     }
     else if (pid == 0) {          /* for the child process:         */
          if (execvp(*argU, argU) < 0) {     /* execute the command  */
               printf("*** ERROR: exec failed\n");
               exit(1);
          }
     }
     return pid;
}

int  main(int argc, char *argv[])
{
	char  line[1024];             /* the input line                 */
	char  *argU[64];              /* the command line argvent      */
	int slides = 5;
	int status;

	if (argc>1)
		slides = atoi(argv[1]);

	while (1) {                   /* repeat until done ....         */
		char  numbers[] = {"5 8 7 4 1 2 9 4 0 6"};

		printf("Shell -> ");     /*   display a prompt             */
		gets(line);              /*   read in the command line     */
		//if(line==NULL)*line="0";
		printf("\n");
		int cant_nums = parse(line, argU);       /*   parse the line               */
		if (strcmp(argU[1], "exit") == 0)  /* is it an "exit"?     */
		   exit(0);            /*   exit if it is                */
		if (strcmp(argU[1], "nums") == 0)
			cant_nums = parse(numbers, argU);

		int count_process = cant_nums/slides;
		//count_process += (count_process*slides < cant_nums)?1:0;
		pid_t all_pids[count_process];
		int returned_numbers[count_process];
		//printf("Size: %d\n",count_process);
		//exit(0);

		for (int i = 0; i < count_process; ++i)
		{
			char *slided[slides+2];
			slided[0] = argU[0];
			printf("0: %s\n", slided[0]);
			for (int j = slides*i+1, k=1; j < slides*i + slides + 1; ++j, k++)
			{
				slided[k] = argU[j];
				printf("%d: %s\n",k, slided[k]);
			}
			slided[slides+1] = '\0';
			// memcpy(slided[1], &argU[1], slides*sizeof(char));
			// printf("%s\n", slided);
			all_pids[i] = execute(slided);           /* otherwise, execute the command */
		}

		//pid_t pid = execute(argU);
		for (int i = 0; i < count_process; ++i)
		{
			wait(&status);
			returned_numbers[i] = WEXITSTATUS(status);
			printf("pid: %d num: %d\n",all_pids[i], returned_numbers[i]);
		}
	}
}