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

struct Node
{
  unsigned int id;
  char *data;
  int active;
  struct Node *next;
};

void initConsole(); //int *customCommand

int userInput(char *option);

int checkCommand(int command);

char **parseCommand(char *, char *);

int executeCommand(char *command);

int executeAddedCommand(char *command, int background, int current);

int executeBackgroundCommand(char *command);

void searchAndDisable(struct Node *node, unsigned int thread_id);

int checkList(struct Node *node);

void *executeProgram(void *arg);

void printChar(void *c);

int detectAmp(char inputCommand[]);

struct Node *start = NULL;

int command_index = 0;
int background_command_count = 0;
int total_background = 0;
char *commands[COMMANDS_MAX];

void initConsole() //int *customCommand
{

    printf("===== Mid-Day Commander, v2 =====\n");
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
    printf("\tr. running processes : Print list of running processes\n");

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

void push(struct Node** head_ref, void *new_data, int id, size_t data_size)
{
    // Allocate memory for node
    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));

    new_node->id = id;
    new_node->data  = malloc(data_size);
    strcpy(new_node->data, (char *) new_data);
    new_node->active = 1;
    new_node->next = (*head_ref);

    int i;
    for (i=0; i<data_size; i++)
        *(char *)(new_node->data + i) = *(char *)(new_data + i);

    (*head_ref)    = new_node;
}

void searchAndDisable(struct Node *node, unsigned int thread_id)
{
    while (node != NULL)
    {
    	if(node->id == thread_id) {
    		node->active = 0;
    	}
        node = node->next;
    }
}

int checkList(struct Node *node) {
	while (node != NULL)
	{
		if(node->active)
			return 0;
		node = node->next;
	}

	return 1;
}

void printList(struct Node *node, void (*fptr)(void *))
{
	int i=0;
    while (node != NULL)
    {
    	if(node->active) {
    		i++;
    		printf("%d. ", i);
    		(*fptr)(node->data);
    	}
        node = node->next;
    }

    printf("\n");
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
    	if(!checkList(start)) {
    		printf("The background processes have not been completed yet.\n");
    		printf("Waiting.\n");
    		while(!checkList(start));

    		printf("Logging you out, Commander.\n");
    		exit(0);
    	} else {
    		printf("Logging you out, Commander.\n");
        	exit(0);
    	}
        break;

    case 'p':
        printf("\n\n-- Current directory --\n");
        return executeCommand("pwd");
        break;

    case 'r':
    	printf("\n\n -- Background Processes -- \n\n");
    	printList(start, printChar);
    	break;

    default:

        // Check that user has added custom commands
//        printf("%d\n", command_index);
//        printf("%d\n", command);
	    command -= 48;
        if (command_index > 0 && command < command_index + 3)
        {
        	if(detectAmp(commands[command - 3])){
        		background_command_count++;
        		total_background++;
        		return executeBackgroundCommand(commands[command - 3]);
        	} else {
        		return executeAddedCommand(commands[command - 3], 0, 0);
        	}
        }

        return -1;
        break;
    }

    return 0;
}

int executeAddedCommand(char input[], int background, int current)
{
    char **command;

    command = parseCommand(input, " ");
    int rc = fork();

    if (rc < 0)
    {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0)
    {
//    	if(background) {
//    		printf("\n -- Command: %s -- \n", input);
//    		printf("[%d] %ld", background_command_count, pthread_self());
//    	}

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

		if(background) {
			printf("\n\n -- Job Complete [%d] -- \n", current);
			printf("Process ID: %ld\n", pthread_self());
			searchAndDisable(start, pthread_self());
			background_command_count--;
		}

        printf("\n-- Statistics --\nElapsed Time: %ld milliseconds\nPage Faults: %ld\nPage Faults (reclaimed): %ld\n\n", timeToExecute, (finalFault - initFault), (finalRecl - initRecl));
    }
    
    free(command);

    return 0;
}

void printChar(void *c)
{
   printf("%s\n", (char *)c);
}

int executeBackgroundCommand(char *command) {


	pthread_t thr;
	pthread_t* pntr = &thr;

//	create_thread(pntr, command);

	pthread_create(pntr, NULL, executeProgram, (void *) command);


//	pthread_join(pntr, NULL);

	return 0;
}

void *executeProgram(void *arg) {

//	printf("\n Thread started: %s \n", (char *) arg);

	char command[strlen(arg)];
	strncpy(command, arg, strlen(arg));
	command[strlen(arg) - 1] = '\0';

//	printf("\n Thread executes: %s \n", command);

	int command_number = background_command_count;

	unsigned int thread_id = pthread_self();
	push(&start, command, thread_id, sizeof(thread_id));

	printf("\n\n -- Command: %s -- \n", (char *) arg);
	printf("[%d] %ld\n", background_command_count, pthread_self());

	executeAddedCommand(command, 1, command_number);

	return NULL;
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

char **parseCommand(char inputCommand[], char *separator)
{
    char **comm = malloc(8 * sizeof(char *));
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
