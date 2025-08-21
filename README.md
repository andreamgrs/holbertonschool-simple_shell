## 📁 Project Overview
The goal of this project is to create a UNIX command-line interpreter in C that mimics the behavior of a standard shell like sh.

It supports executing simple commands (e.g. ls), searching for executables in the PATH, and handling signals.

## ❗ Requirements
- Allowed editors: `vi`, `vim`, `emacs`
- All your files will be compiled on Ubuntu 20.04 LTS using gcc
- All your files should end with a new line
- A `README.md` file, at the root of the folder of the project is mandatory
- Your code should use the `Betty` style. It will be checked using betty-style.pl and betty-doc.pl
- Your shell should not have any memory leaks
- No more than 5 functions per file
- All your header files should be include guarded
- Use system calls only when you need to

**List of allowed functions and system calls+**
- all functions from string.h
- `access` (man 2 access)
- `chdir` (man 2 chdir)
- `close` (man 2 close)
- `closedir` (man 3 closedir)
- `execve` (man 2 execve)
- `exit` (man 3 exit)
- `_exit` (man 2 _exit)
- `fflush` (man 3 fflush)
- `fork` (man 2 fork)
- `free` (man 3 free)
- `getcwd` (man 3 getcwd)
- `getline` (man 3 getline)
- `getpid` (man 2 getpid)
- `isatty` (man 3 isatty)
- `kill` (man 2 kill)
- `malloc` (man 3 malloc)
- `open` (man 2 open)
- `opendir` (man 3 opendir)
- `perror` (man 3 perror)
- `printf` (man 3 printf)
- `fprintf` (man 3 fprintf)
- `vfprintf` (man 3 vfprintf)
- `sprintf` (man 3 sprintf)
- `putchar` (man 3 putchar)
- `read` (man 2 read)
- `readdir` (man 3 readdir)
- `signal` (man 2 signal)
- `stat` (__xstat) (man 2 stat)
- `lstat` (__lxstat) (man 2 lstat)
- `fstat` (__fxstat) (man 2 fstat)
- `strtok` (man 3 strtok)
- `wait` (man 2 wait)
- `waitpid` (man 2 waitpid)
- `wait3` (man 2 wait3)
- `wait4` (man 2 wait4)
- `write` (man 2 write)

## 🔧 Programs & Functions
- Display a prompt (`#cisfun$`) and wait for user input.
- Execute simple commands with arguments (e.g. `ls -l`).
- Handle the `PATH` to find executables without typing full paths.
- Prevent calling `fork()` if a command does not exist.
- Properly handle `Ctrl-C` (`SIGINT`):
    - Does not exit the shell.
    - Prints a new prompt on a new line.
- Exit the shell when the user enters `exit` (no arguments supported).
- Print the current environment variables when the user enters `env`.

      
## 🛠️ Compilation & Execution
All the files compile this way:

	$ gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh

Then run the executable file:

	$ ./hsh

Example:

	#cisfun$ ls
	file1.c  file2.c  hsh
	#cisfun$ /bin/pwd
	/home/user/simple_shell
	#cisfun$ env
	PATH=/usr/local/bin:/usr/bin:/bin
	...
	#cisfun$ exit

## 👩‍💻 Authors
- Andrea Munoz Gris
- Dana Le
- Carla Ciar

## End
