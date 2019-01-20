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

void remove_spaces(char *source);
char **make2Dchar(int nrows, int ncolumns, char **old);

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
        printf("\t%d. %s : User added command\n", i + 3, *(commands + i));
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
        struct timeval tv1, tv2; // To calculate the time the command took to execute (milliseconds)
        gettimeofday(&tv1, NULL);
        // int rc_wait =
        wait(NULL);
        gettimeofday(&tv2, NULL);
        // print out statistics
        long int timeToExecute = (tv2.tv_usec - tv1.tv_usec) / 1000 + (tv2.tv_sec - tv1.tv_sec) * 1000;
        struct rusage usage;
        getrusage(RUSAGE_CHILDREN, &usage);
        printf("\n-- Statistics --\nElapsed Time: %ld milliseconds\nPage Faults: %ld\nPage Faults (reclaimed): %ld\n\n", timeToExecute, usage.ru_majflt, usage.ru_minflt);
    }
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

        char *command_input = (char *)malloc(buffer * sizeof(char));

        size_t readline = getline(&command_input, &buffer, stdin);

        if (command_input[readline - 1] == '\n')
        {
            command_input[readline - 1] = '\0';
        }

        commands[command_index] = (char *)malloc(buffer * sizeof(char));

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
        printf("%d\n", command_index);
        printf("%d\n", command);
        if (command_index > 0 && command < command_index + 3)
        {
            return executeAddedCommand(commands[command - 3]);
        }

        return -1;
        break;
    }
}

int executeAddedCommand(char *command)
{
    printf("--%s--", command);

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

char **make2Dchar(int nrows, int ncolumns, char **old)
{

    char **a;       // Array of pointers to rows
    unsigned int i; // Loop counter

    // First allocate the array of pointers to rows
    a = (char **)malloc(nrows * sizeof(char *));
    if (!a)
    { // Unable to allocate the array
        return (char **)NULL;
    }

    // Now allocate array for each row
    for (i = 0; i < nrows; i++)
    {
        // i is the row we are about to allocate
        a[i] = malloc(ncolumns * sizeof(char));
        if (!a[i])
        {
            return (char **)NULL; // Unable to allocate
        }

        if (nrows < command_index)
        {
            // memset(a[i], '\0', strlen(a[i]));
            printf("Copy: %s to %s", old[i], a[i]);
            strncpy(a[i], old[i], BUFFERSIZE);
        }
    }
    return a;
}

char **parseCommand(char input[])
{
    char **command = malloc(8 * sizeof(char *));
    char *separator = " ";
    char *parsed;
    int index = 0;

    parsed = strtok(input, separator);

    while (parsed != NULL)
    {
        command[index] = parsed;
        index++;

        parsed = strtok(NULL, separator);
    }

    command[index] = NULL;
    return command;
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
            fprintf(stderr, "\n\nWrong input. Please run again and type an integer from 0 to 2\n");
            exit(1);
        }
    }

    free(option);

    free(commands);

    return 0;
}
