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

int main(int argc, char const *argv[])
{
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
