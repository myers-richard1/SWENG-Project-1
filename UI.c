#include "UI.h"
#include <stdio.h>
#include <string.h>

#include "Job.h"
#include "ThreadsafeTypes.h"

//Returns a NULL job if the user didnt add a new job
//program data is used for setting running status for threads to check if they should exit
Action get_user_input(ThreadsafeData* program_data){
    //greet the user and print instructions
    printf(">");

    Action action;
    action.type = INVALID;
    //init an array for holding user input
    char userInput[10] = "";
    //get user input
    fgets(userInput, sizeof(userInput), stdin);
    //strip trailing newline
    userInput[strcspn(userInput, "\n")] = 0;
    if (strstr(userInput, "run")){
        //extract 
        action.type = RUN;
    }
    if (strcmp(userInput, "list") == 0){
        JobQueueNode* node = program_data->head;
        while(node){
            printf("%s\n", node->job->executable_name);
            node = node->next_node;
        }
        action.type = LIST;
    }
    if (!strcmp(userInput, "fcfs")){
        //TODO change scheduling policy to fcfs
        printf("User entered \"fcfs\"\n");
        action.type = FCFS;
    }
    if (!strcmp(userInput, "sjf")){
        //TODO change scheduling policy to sjf
        printf("User entered \"sjf\"\n");
        action.type = SJF;
    }
    if (!strcmp(userInput, "priority")){
        //TODO change scheduling policy to priority
        printf("User entered \"priority\"\n");
        action.type = PRIORITY;
    }
    if (strstr(userInput, "test")){
        //TODO run test
        printf("User entered \"test\"\n");
        action.type = TEST;
    }
    if (!strcmp(userInput, "quit")){
        printf("User entered \"quit\"\n");
        pthread_mutex_lock(&program_data->running_mutex);
        program_data->running = 0;
        pthread_mutex_unlock(&program_data->running_mutex);
        action.type = QUIT;
    }
    if (!strcmp(userInput, "help")){
        printf("run <job> <time> <pri>: submit a job named <job>, execution time is <time>\n");
        printf("list: display the job status.\n");
        printf("fcfs: change the scheduling policy to priority.\n");
        printf("test <benchmark> <policy> <num_of_jobs> <priority_levels> <min_CPU_time> <max_CPU_time>\n");
        printf("quit: exit CSUbatch\n");
        action.type = HELP;
    }
    if (action.type == INVALID){
        printf("Invalid input! Type 'help' to find more about CSUBatch commands.\n");
    }
    return action;
}