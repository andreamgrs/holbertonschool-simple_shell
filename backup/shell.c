#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

/**
 * main - first version of a super simple shell that can
 * run commands with their full path, without any argument.
 * line: pointer where the line will be store in the buffer.
 * size: pointer to the size of the buffer.
 * stdin: get the line from the user typing.
 *
 * Return: Always 0.
 */
void handled_sigint(int sig)
{
	(void)sig;
	exit(0);
}

int main(void)
{
	char *line = NULL;
	size_t size = 0;
	ssize_t read;
	int status, cont_argv;
	char *argv[100];
	char *token;
	pid_t child_pid;

	signal(SIGINT, handled_sigint);

	while (1)
	{
		if (isatty(STDIN_FILENO))
		{
			printf("#cisfun$");
		}
		read = getline(&line, &size, stdin);
		if (read == -1)
		{
			free(line);
			exit(0);
		}
		line[strcspn(line, "\n")] = '\0';
		token = strtok(line, " \t\r\n");
		cont_argv = 0;
		while (token != NULL && cont_argv < 100)
		{
			if (*token != '\0')
			{
				argv[cont_argv] = token;
				cont_argv++;
			}

			/**argv[cont_argv] = token;
			cont_argv++;*/
			token = strtok(NULL, " \t\r\n");
		}
		argv[cont_argv] = NULL;

		if (argv[0] == NULL)
		{
			continue;
		}

		child_pid = fork();
		if (child_pid == -1)
	    	{
	    		perror("Error on the fork");
		    	return (1);
	    	}
		if (child_pid == 0)
	    	{
			if (execve(argv[0], argv, NULL) == -1)
			{
				perror("./shell");
			}
		}
		else 
		{
			wait(&status);
		}

	}
	free(line);
	return (0); 
}
