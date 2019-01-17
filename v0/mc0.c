#include<stdio.h>
#include<stdlib.h>

#define TRUE 1

int userInput(int *option);

int userInput(int *option) {
    int check = scanf("%d", option);

    if(check == 1) {
        return 0;
    }

    return -1;
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
