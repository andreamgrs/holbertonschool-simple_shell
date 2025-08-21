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
 * print_env - prints the environment using extern char **environ
 *
 * Return: nothing.
 */

void print_env()
{
	int cont;
	for (cont = 0; environ[cont] != NULL; cont++)
	{
		printf("%s\n", environ[cont]);
	}
}

/**
 * _getenv - function searches the environment
 * list to find the environment variable name,
 * and returns a pointer to the corresponding value string.
 *
 * @name: name of the env variable. 
 *
 * Return: returns a pointer to the value in the
 * environment, or NULL if there is no match.
 */
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

/**
 * handled_sigint - function to handle the Ctrl + C to exit the shell.
 * @sig: not using.
 *
 * Return: nothing
 */
void handled_sigint(int sig)
{
	(void)sig;
	exit(0);
}

/**
 * check_file_exists - Check if file exists and is executable
 * @file: file to evaluate. 
 * X_OK: Check if the file is executable by current user.
 *
 * Return: 1 if the file exist and current user has execute
 * permission and 0 if doesn't exist or not executable by user.
 */
int check_file_exists(const char *file)
{
	if (access(file, X_OK) == 0)
	{
		return (1);
	}
	else
	{
		return (0);
	}
}
/**
 * check_command - Search for the full path of a command.
 * @cmd: command to evaluate.
 *
 * strchr()
 * Check if the command contains / if yes, check if file
 * exists and executable then return a copy of the command.
 *
 * snprintf()
 * Print the full path of the command
 *
 * Return: the full path of the command.
 */
char *check_command(char *cmd)
{
	char *name = "PATH";
	char fullpath[PATH_MAX];
	char *path_env;
	char *path_copy;
	char *token;

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
	path_env = _getenv(name);
	if (path_env == NULL)
	{
		return (NULL);
	}
	path_copy = strdup(path_env);

	token = strtok(path_copy, ":");

	while (token)
	{
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

/**
 * main - our own version of a simple shell that can
 * run commands with their full path, with arguments.
 * Works in interactive and non-interactive modes.
 *
 * Return: Always 0;
 */
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
		}
		free(cmd_path);
	}

	free(line);
	return (status);
}

