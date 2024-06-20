# Simple Shell
This is a simple shell implementation written in C that supports basic command execution, command history, and piped commands. It also handles SIGINT (Ctrl+C) to display detailed command history.

# Features
> Basic Command Execution: Execute standard shell commands.
> Command History: Maintain a history of executed commands.
> Piped Commands: Handle commands connected by pipes (|).
> Signal Handling: Display detailed command history upon receiving SIGINT (Ctrl+C).
> Getting Started


# Prerequisites
A C compiler (e.g., gcc)
A Unix-like operating system (Linux, macOS)
Building the Shell

# To compile the shell, use the following command:

gcc -o simple_shell simple_shell.c
Running the Shell
After compiling, you can run the shell with:


./simple_shell

# Usage

**Basic Commands**
You can enter any standard shell command, and it will be executed. For example:
ls -l
**Piped Commands**
You can also enter commands with pipes. For example:


ls -l | grep "myfile"

**Command History**
To view a brief command history, enter:


**history**
Signal Handling
Pressing Ctrl+C will display detailed command history, including the start and end times, and the process IDs (PIDs) of the executed commands.

Code Structure
Header Files and Macros
Includes standard libraries for I/O, string manipulation, process control, signal handling, and time management.
Defines MAX_HISTORY_SIZE to limit the command history size.
Data Structures
struct CommandHistory: Stores command details including the command string, start and end times, and the PID.

# Functions
# Command History
void his(): Displays a brief command history.
void signal_handler(int signum): Handles the SIGINT signal and displays detailed command history.

# Command Reading and Parsing
int readcommand(char *command): Reads user input.
void parsecmd_pipe(char *command, char **argument): Parses a command into arguments for piped commands.
void parsecmd(char *command, char **argument, int *numarg): Parses a command into arguments for non-piped commands.

# Command Execution
void executesimplecommand(char **argument): Executes a simple (non-piped) command.
int checkpipe(char *command): Checks if a command contains a pipe.
int countpipe(char *command): Counts the number of pipes in a command.
int parsepipe(char *argument, char **pipeargument): Parses a piped command into individual commands.
void execute_piped_commands(int num_of_piped_commands, char **pipeargument): Executes piped commands.

# Main Function
Sets up the SIGINT signal handler.
Reads and processes commands in an infinite loop.
Handles history commands.
Distinguishes between piped and non-piped commands.
Manages command execution and history storage.

# Notes
The shell supports a maximum command length of 20000 characters and a maximum of 200 arguments.
Command history is limited to 100 entries by default.
Ensure to handle memory allocation and deallocation appropriately to avoid memory leaks.

# License
This project is licensed under the MIT License. See the LICENSE file for details.
