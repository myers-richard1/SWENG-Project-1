#include "UI.h"
#include <stdio.h>
#include <string.h>

#include "Job.h"

//Returns a NULL job if the user didnt add a new job
Job* get_user_input(){
    //greet the user and print instructions
    printf("Welcome to Richard and Harrison's batch job scheduler Version 1.0\n");
    printf("Type 'help' to find more about CSUBatch commands.\n");

    Job* newJob = NULL;
    //init an array for holding user input
    char userInput[10] = "";
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
    if (!strcmp(userInput, "quit")){
        printf("User entered \"quit\"\n");
    }
    if (!strcmp(userInput, "help")){
        printf("User entered \"help\"\n");
    }
    return newJob;
}