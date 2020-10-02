#include "UI.h"
#include <stdio.h>
#include <string.h>

#include "Job.h"
#include "ThreadsafeTypes.h"

//Returns a NULL job if the user didnt add a new job
//program data is used for setting running status for threads to check if they should exit
Action get_user_input(ThreadsafeData* program_data){
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
        JobQueueNode* node = program_data->head;
        while(node){
            printf("%s\n", node->job->executable_name);
            node = node->next_node;
        }
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
        pthread_mutex_lock(&program_data->running_mutex);
        program_data->running = 0;
        pthread_mutex_unlock(&program_data->running_mutex);
    }
    if (!strcmp(userInput, "help")){
        printf("User entered \"help\"\n");
    }
    return newJob;
}