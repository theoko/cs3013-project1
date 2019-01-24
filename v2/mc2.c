/*
 * Fivos Kavassalis - fikavassalis
 * Theodoros Konstantopoulos - tkonstantopoulos
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>

#define TRUE 1
#define MAX_DIR_LEN 255
#define BUFFERSIZE 50
#define COMMANDS_MAX 200

void initConsole(); //int *customCommand
int userInput(char *option);
int checkCommand(int command);
char **parseCommand(char *);
int executeCommand(char *command);
int executeAddedCommand(char *command);
int executeBackgroundCommand(char *command);
int create_thread(pthread_t thread, char *inputCommand);

int detectAmp(char inputCommand[]);

void remove_spaces(char *source);

int command_index = 0;
char *commands[COMMANDS_MAX];

void initConsole() //int *customCommand
{

    printf("===== Mid-Day Commander, v1 =====\n");
    printf("G’day, Commander! What command would you like to run?\n");
    printf("\t0. whoami : Prints out the result of the whoamicommand\n");
    printf("\t1. last : Prints out the result of the last command\n");
    printf("\t2. ls : Prints out the result of a listing on  a user-specified path\n");

    for (int i = 0; i < command_index; i++)
    {
        printf("\t%d. %s : User added command\n", i + 3, commands[i]);
    }

    printf("\ta. add command : Adds a new command to the menu.\n");
    printf("\tc. change directory : Changes process working directory\n");
    printf("\te. exit : Leave Mid-Day Commander\n");
    printf("\tp. pwd : Prints working directory\n");
    printf("Option?: ");

}

int userInput(char *option)
{
    int check = scanf("%c", option);
    getchar();

    if (check == 1)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

int executeCommand(char *command)
{
    int rc = fork();

    if (rc < 0)
    {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0)
    {
        char *argv[2];
        argv[0] = command;
        argv[1] = NULL;

        execvp(command, argv);
    }
    else
    {
        struct rusage usage;
        getrusage(RUSAGE_CHILDREN, &usage);
	long initFault = usage.ru_majflt;
	long initRecl = usage.ru_minflt;
        struct timeval tv1, tv2; // To calculate the time the command took to execute (milliseconds)
        gettimeofday(&tv1, NULL);
        wait(NULL);
        gettimeofday(&tv2, NULL);

        // print out statistics
        long int timeToExecute = (tv2.tv_usec - tv1.tv_usec) / 1000 + (tv2.tv_sec - tv1.tv_sec) * 1000;
        getrusage(RUSAGE_CHILDREN, &usage);
	long finalFault = usage.ru_majflt;
	long finalRecl = usage.ru_minflt;
        printf("\n-- Statistics --\nElapsed Time: %ld milliseconds\nPage Faults: %ld\nPage Faults (reclaimed): %ld\n\n", timeToExecute, (finalFault - initFault), (finalRecl - initRecl));;
    }

    return 0;
}

int checkCommand(int command)
{

    switch (command)
    {
    case '0':
        printf("\n\n-- Who Am I? --\n");
        return executeCommand("whoami");
        break;

    case '1':
        printf("\n\n-- Last Logins --\n");
        return executeCommand("last");
        break;

    case '2':
        printf("\n\n-- Directory Listing --\n");
        return executeCommand("ls");
        break;

    case 'a':
        printf("\n\n-- Add a command --\n");
        printf("Command to add?: ");

        size_t buffer = BUFFERSIZE;

        char *command_input = (char *) malloc(buffer * sizeof(char));

        size_t readline = getline(&command_input, &buffer, stdin);

        if (command_input[readline - 1] == '\n')
        {
            command_input[readline - 1] = '\0';
        }

        commands[command_index] = (char *) malloc(buffer * sizeof(char));

        strcpy(commands[command_index], command_input);

        printf("Okay, added with ID %d!\n\n", command_index + 3);
        
        command_index++;

        free(command_input);

        return 0;

        break;

    case 'c':
        printf("\n\n-- Change directory --\n");

        char directory[MAX_DIR_LEN];

        printf("New Directory?: ");
        int check = scanf("%s", directory);
        getchar();

        if (check == 1)
        {
            if (chdir(directory) == 0)
                return 0;
            else
                return -2;
        }
        else
        {
            fprintf(stderr, "\n\nWrong input. Please input a valid path.\n");
            exit(1);
        }

        break;

    case 'e':
        printf("Logging you out, Commander.\n");
        exit(0);
        break;

    case 'p':
        printf("\n\n-- Current directory --\n");
        return executeCommand("pwd");
        break;

    default:

        // Check that user has added custom commands
//        printf("%d\n", command_index);
//        printf("%d\n", command);
	    command -= 48;
        if (command_index > 0 && command < command_index + 3)
        {
        	if(detectAmp(commands[command - 3])){
        		return executeBackgroundCommand(commands[command - 3]);
        	} else {
        		return executeAddedCommand(commands[command - 3]);
        	}
        }

        return -1;
        break;
    }
}

int executeAddedCommand(char input[])
{
    char **command;

    command = parseCommand(input);
    int rc = fork();

    if (rc < 0)
    {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0)
    {
        execvp(command[0], command);
        printf("execvp was not successful\n");
    }
    else
    {
	struct rusage usage;
        getrusage(RUSAGE_CHILDREN, &usage);
	long initFault = usage.ru_majflt;
	long initRecl = usage.ru_minflt;
        struct timeval tv1, tv2; // To calculate the time the command took to execute (milliseconds)
        gettimeofday(&tv1, NULL);
        wait(NULL);
        gettimeofday(&tv2, NULL);

        // print out statistics
        long int timeToExecute = (tv2.tv_usec - tv1.tv_usec) / 1000 + (tv2.tv_sec - tv1.tv_sec) * 1000;
        getrusage(RUSAGE_CHILDREN, &usage);
	long finalFault = usage.ru_majflt;
	long finalRecl = usage.ru_minflt;
        printf("\n-- Statistics --\nElapsed Time: %ld milliseconds\nPage Faults: %ld\nPage Faults (reclaimed): %ld\n\n", timeToExecute, (finalFault - initFault), (finalRecl - initRecl));
    }
    
    free(command);

    return 0;
}

int executeBackgroundCommand(char *command) {

//	pid_t pid = fork();
//
//    if (pid < 0)
//    {
//        fprintf(stderr, "fork failed\n");
//        exit(1);
//    }
//
//	// TODO: will handle error here (<0)
//	if(pid  == 0) { // child
//		// output whatever is required from the example
//		initConsole();
//		char* newInput = malloc(sizeof(char));
//
//		if(userInput(newInput) == 0) {
//			checkCommand(newInput);
//		}
//
//		free(newInput);
//
//		char **inputCommand;
//		inputCommand = parseCommand(command);
//
//		execvp();// this command -- first command
//
//	} else if (pid > 0) {
//		int status;
//
//		waitpid(pid, status, WUNTRACED);
//	}

	pthread_t p1;

	create_thread(p1, command);

	return 0;
}

int create_thread(pthread_t thread, char *inputCommand) {

	//pointer to function executeProgram
	//delete & from inputCommand value

	pthread_create(&thread, NULL, , &inputCommand);

	return 0;
}

void remove_spaces(char *source)
{
    char *i = source;
    char *j = source;
    while (*j != 0)
    {
        *i = *j++;
        if (*i != ' ')
            i++;
    }
    *i = 0;
}

int detectAmp(char inputCommand[]) {

	char *ptr;
	int amp = '&';

	ptr = strchr(inputCommand, amp);

	if(ptr == NULL)
		return 0;
	else
		return 1;

}

char **parseCommand(char inputCommand[])
{
    char **comm = malloc(8 * sizeof(char *));
    char *separator = " ";
    char *parsed;
    int j = 0;

    char tmp[strlen(inputCommand)+1];

    strcpy(tmp, inputCommand);

    parsed = strtok(tmp, separator);

    while (parsed != NULL)
    {
        comm[j] = parsed;
        j++;

        parsed = strtok(NULL, separator);
    }

    comm[j] = NULL;
    return comm;
}

int main(int argc, char const *argv[])
{

    char *option = malloc(sizeof(char));

    // commands = make2Dchar(command_index + 1, BUFFERSIZE, commands);

    while (TRUE)
    {

        // commands = make2Dchar(command_index + 1, BUFFERSIZE, commands);
        initConsole();

        if (userInput(option) == 0)
        {

            // if (*option == '0' || *option == '1' || *option == '2')
            //     printf("Test!");
            // Input read successfully
            checkCommand(*option);

            // else
            //     fprintf(stderr, "\n\nWrong input. Please type an integer from 0 to 2\n\n");
        }
        else
        {
            // Error reading input
            // Print error message
            fprintf(stderr, "\n\nWrong input.\n");
            exit(1);
        }
    }

    free(option);

    free(commands);

    return 0;
}
