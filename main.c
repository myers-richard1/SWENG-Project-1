#include <stdio.h>
#include <string.h>

int main(){
    //greet the user and print instructions
    printf("Welcome to [Name]'s batch job scheduler Version 0.01\n");
    printf("Type 'help' to find more about CSUBatch commands.\n");

    //init an array for holding user input
    char userInput[10] = "";
    //sentinel control loop, runs while user input != "quit"
    while (strcmp(userInput,"quit")){
        //get user input
        fgets(userInput, sizeof(userInput), stdin);
        //strip trailing newline
        userInput[strcspn(userInput, "\n")] = 0;
        if (strstr(userInput, "run")){
            //TODO run job
            printf("User entered \"run\"\n");
        }
        if (strcmp(userInput, "list") == 0){
            //TODO list jobs
            printf("User entered \"list\"\n");
        }
        if (!strcmp(userInput, "fcfs")){
            //TODO change scheduling policy to fcfs
            printf("User entered \"fcfs\"\n");
        }
        if (!strcmp(userInput, "sjf")){
            //TODO change scheduling policy to sjf
            printf("User entered \"sjf\"\n");
        }
        if (!strcmp(userInput, "priority")){
            //TODO change scheduling policy to priority
            printf("User entered \"list\"\n");
        }
        if (strstr(userInput, "test")){
            //TODO run test
            printf("User entered \"test\"\n");
        }
        if (!strcmp(userInput, "priority")){
            //TODO change scheduling policy to priority
            printf("User entered \"list\"\n");
        }
        if (!strcmp(userInput, "quit")){
            printf("User entered \"quit\"\n");
        }
        if (!strcmp(userInput, "help")){
            //TODO print help text
            printf("User entered \"help\"\n");
        }
    }

    printf("Exiting normally\n");
}