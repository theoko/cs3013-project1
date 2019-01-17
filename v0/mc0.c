#include <pthread.h> 
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h> 

#define MAX 5
#define TRUE 1

void initConsole();
int userInput(int *option);
int checkCommand(int command);
int executeCommand(char* command);


void initConsole() {

	printf("===== Mid-Day Commander, v0 =====\n");
	printf("G’day, Commander! What command would you like to run?\n");
	printf("\t0. whoami : Prints out the result of the whoamicommand\n");
	printf("\t1. last : Prints out the result of the last command\n");
	printf("\t2. ls : Prints out the result of a listing on  a user-specified path\n");
	printf("Option?: ");	
	
}

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
        char *argv[2];
        argv[0] = command;
        argv[1] = NULL;

        execvp(command, argv);
	
	

    } else {
	struct timeval tv1, tv2; // To calculate the time the command took to execute (milliseconds)
	gettimeofday(&tv1, NULL);
	int rc_wait = wait(NULL);
	gettimeofday(&tv2, NULL);
	// print out statistics
	long int timeToExecute = (tv2.tv_usec - tv1.tv_usec)/1000 + (tv2.tv_sec - tv1.tv_sec)*1000;
	struct rusage usage;
	getrusage(RUSAGE_CHILDREN, &usage);	
	getrusage(RUSAGE_CHILDREN, &usage);
	printf("\n-- Statistics --\nElapsed Time: %ld milliseconds\nPage Faults: %ld\nPage Faults (reclaimed): %ld\n\n", timeToExecute, usage.ru_majflt, usage.ru_minflt);
    }
}

int checkCommand(int command) {
    switch(command) {
        case 0:
	    printf("\n\n-- Who Am I? --\n");
            return executeCommand("whoami");
            break;

        case 1:
    	    printf("\n\n-- Last Logins --\n");
            return executeCommand("last");
            break;

        case 2:	
	    printf("\n\n-- Directory Listing --\n");
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
	
	initConsole();	

        if(userInput(option) == 0) {

	    if(*option==0 || *option==1 || *option==2) 
		// Input read successfully 
            	checkCommand(*option);
	    else
	    	fprintf(stderr, "\n\nWrong input. Please type an integer from 0 to 2\n\n");	

        } else {
            // Error reading input
            // Print error message
	    fprintf(stderr, "\n\nWrong input. Please run again and type an integer from 0 to 2\n");
	    exit(1);
	    	
        }
    }


    free(option);
    
    return 0;
}

