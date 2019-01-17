#include<stdio.h>
#include<stdlib.h>

void initConsole();

int main(int argc, char const *argv[])
{
    initConsole();
    
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

