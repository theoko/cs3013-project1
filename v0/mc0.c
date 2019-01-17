#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>

#define TRUE 1

int userInput(int *option);
int checkCommand(int command);
int executeCommand(char* command);

int userInput(int *option) {
    int check = scanf("%d", option);

    if(check == 1) {
        return 0;
    }

    return -1;
}

int executeCommand(char* command) {
    int rc = fork();
    
    if(rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if(rc == 0) {

    } else {

    }
}

int checkCommand(int command) {
    switch(command) {
        case 0:
            return executeCommand("whoami");
            break;

        case 1:
            return executeCommand("last");
            break;

        case 2:
            return executeCommand("ls");
            break;

        default:
            return -1;
            break;
    }
}

void initConsole();

int main(int argc, char const *argv[])
{

    initConsole();

    int *option = malloc(sizeof(int));

    while(TRUE) {
        if(userInput(option) == 0) {
            // Input read successfully 
            printf("%d", *option);
        } else {
            // Error reading input
            // Print error message
        }
    }


    free(option);
    
    return 0;
}


void initConsole() {

	printf("===== Mid-Day Commander, v0 =====\n");
	printf("G’day, Commander! What command would you like to run?\n");
	printf("\t0. whoami : Prints out the result of the whoamicommand\n");
	printf("\t0. last : Prints out the result of the last command\n");
	printf("\t0. ls : Prints out the result of a listing on  a user-specified path\n");
	printf("Option?: ");	
	
}

