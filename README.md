## 📁 Project Overview
The goal of this project is to create a UNIX command-line interpreter in C that mimics the behavior of a standard shell like sh.
It supports executing simple commands (e.g. ls), searching for executables in the PATH, and handling signals.


## 🔧 Programs & Functions
- Display a prompt (#cisfun$) and wait for user input.
- Execute simple commands with arguments (e.g. ls -l).
- Handle the PATH to find executables without typing full paths.
- Prevent calling fork() if a command does not exist.
- Properly handle Ctrl-C (SIGINT):
    - Does not exit the shell.
    - Prints a new prompt on a new line.

      
## 🛠️ Compilation & Execution
All the files compile this way:

	$ gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh

Then run the executable file:

	$ ./hsh
 

## End
