#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <limits.h>
#include <stddef.h>

extern char **environ;

/**
 * main - first version of a super simple shell that can
 * run commands with their full path, without any argument.
 * line: pointer where the line will be store in the buffer.
 * size: pointer to the size of the buffer.
 * stdin: get the line from the user typing.
 *
 * Return: Always 0.
 */

void print_env()
{
	int cont;
	for (cont = 0; environ[cont] != NULL; cont++)
	{
		printf("%s\n", environ[cont]);
	}
}

char *_getenv(const char *name)
{
	int cont;
	char *list;
	size_t name_len = strlen(name);
	int value;

	for (cont = 0; environ[cont] != NULL; cont++)
	{
		list = environ[cont];
		value = strncmp(list, name, name_len);

		if(value == 0 && list[name_len] == '=')
		{
			return (&list[name_len + 1]);
		}
	}
	return (NULL);
}

void handled_sigint(int sig)
{
	(void)sig;
	exit(0);
}


/* Check if file exists and is executable */
int check_file_exists(const char *file)
{
	/* X_OK: Check if the file is executable by current user*/
	if (access(file, X_OK) == 0)
	{
		/* file exist and current user has execute permission */
		return (1);
	}
	else
	{
		/* doesn't exist or not executable by user */
		return (0);
	}
}

/* Search for the full path of a command */
char *check_command(char *cmd)
{
	char *name = "PATH";
	char fullpath[PATH_MAX];
	char *path_env;
	char *path_copy;
	char *token;

	/* Check if the command contains / */
	/* If yes, check if file exists and executable then return a copy of the command */
	if (strchr(cmd, '/'))
	{
		if(check_file_exists(cmd))
		{
			return (strdup(cmd));
		}
		else
		{
			return (NULL);
		}
	}
	/* Get the value of the path environment variable */
	path_env = _getenv(name);
	if (path_env == NULL)
	{
		return (NULL);
	}
	/* Back up the PATH to not lose its value when using strtok later */
	path_copy = strdup(path_env);

	/* Tokenise the PATH by : to get each diretory */
	token = strtok(path_copy, ":");

	while (token)
	{
		/* Print the full path of the command */
		snprintf(fullpath, sizeof(fullpath), "%s/%s", token, cmd);
		
		if (check_file_exists(fullpath))
		{
			free(path_copy);
			return (strdup(fullpath));
		}
		token = strtok(NULL, ":");
	}
	free (path_copy);
	return (NULL);
}

int main(void)
{
	char *line = NULL;
	size_t size = 0;
	ssize_t read;
	int status = 0;
	int cont_argv;
	char *argv[100];
	char *token;
	pid_t child_pid;
	char *cmd_path; 

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
			/* return the last status */
			exit(status);
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
			token = strtok(NULL, " \t\r\n");
		}
		argv[cont_argv] = NULL;

		if (argv[0] == NULL)
		{
			continue;
		}

		if (strcmp(argv[0], "exit") == 0)
		{
			free(line);
			exit(status);
		}

		if (strcmp(argv[0], "env") == 0)
		{
			print_env();
			status = 0;
			continue;
		}

		/* Call the check_command function to get the command user typed */
		cmd_path = check_command(argv[0]);
		/* Check command exists */
		if (cmd_path == NULL)
		{
			fprintf(stderr, "./hsh: 1: %s: not found\n", argv[0]);
			status = 127;
			/* do not fork, skip the rest of the code */
			continue;
		}

		child_pid = fork();
		if (child_pid == -1)
		{
			perror("Error on the fork");
			free(cmd_path);
			return (1);
		}
		if (child_pid == 0)
		{
			argv[0] = cmd_path;
			if (execve(argv[0], argv, NULL) == -1)
			{
				perror("./hsh");
				free(cmd_path);
				exit(EXIT_FAILURE);
			}
		}
		else 
		{
			wait(&status);
			status = status >> 8;
		}
		free(cmd_path);
	}

	free(line);
	return (status);
}

