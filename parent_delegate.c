#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

char* itoa(int val, int base){
	
	static char buf[32] = {0};
	
	int i = 30;
	
	for(; val && i ; --i, val /= base)
	
		buf[i] = "0123456789abcdef"[val % base];
	
	return &buf[i+1];
	
}

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

int call_children(int slides, int cant_nums, char **argU[64], int *status, int *returned_numbers)
{
	int count_process = cant_nums/slides;
	pid_t all_pids[count_process+1];
	// int returned_numbers[count_process];

	// if(cant_nums==3)
	// {
	// 	printf("Num segundo: %d\n", cant_nums);
	// 	exit(0);
	// }

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
		all_pids[i] = execute(slided);           /* otherwise, execute the command */
	}

	int remainders = cant_nums - count_process*slides - 1;
	if(remainders>0)
	{
		char *slided[slides+2];
		slided[0] = argU[0];
		printf("0: %s\n", slided[0]);
		for (int i = cant_nums - remainders, k=1; i < cant_nums; ++i,k++)
		{
			slided[k] = argU[i];
			printf("%d: %s\n",k, slided[k]);
		}
		slided[slides+1] = '\0';
		all_pids[count_process] = execute(slided);           /* otherwise, execute the command */
		count_process++;
	}

	for (int i = 0; i < count_process; ++i)
	{
		wait(status);
		returned_numbers[i] = WEXITSTATUS(*status);
		printf("pid: %d num: %d\n",all_pids[i], *returned_numbers);
		// returned_numbers++;
	}

	// *returned_numbers_arr = returned_numbers;

	return count_process;
}

void update_next_args(int get_nums, char **argU, int *returned_numbers)
{
	*argU = "./child_sort";
	// printf("Word: %s\n", *argU);
	*argU++;
	for (int i = 0; i < get_nums; ++i)
	{
		// char new_value[20];
		 // itoa(returned_numbers[i], *argU, 10);
		// *argU[i] = new_value;

		*argU = itoa(returned_numbers[i],10);
		// printf("probando num: %d\n",returned_numbers[0]);
		// printf("probando num: %d\n",returned_numbers[1]);
		// printf("probando num: %s\n",*argU);
		// exit(0);
		// *argU[i] = new_value;
		*argU++;
	}
	*argU = '\0';
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

		int get_nums = cant_nums/slides + 1;
		int returned_numbers[get_nums];
		do{
			get_nums = call_children(slides, cant_nums, &argU, &status, &returned_numbers);
			update_next_args(get_nums, &argU, &returned_numbers);
			cant_nums = get_nums + 1;
			if(get_nums==1)
				printf("Max Number: %d\n", returned_numbers[0]);
		}while(get_nums>1);
	}
}