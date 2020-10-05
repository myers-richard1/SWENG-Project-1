#include "UI.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Job.h"
#include "ThreadsafeTypes.h"
#include "StringUtils.h"

/**
 * This function takes in the threadsafe program data and returns an "action" struct
 * containing the user's request. This function should only be called by the
 * scheduler when it's checking to see what to do next.
 * */
Action* get_user_input(ThreadsafeData* program_data){
    //greet the user and print instructions
    printf(">");
    //malloc the action (this is free'd in Scheduler.c after it's finished with it)
    Action* action = malloc(sizeof(Action));
    //default to invalid type in case none of the inputs match
    action->type = INVALID;
    //init an array on the stack for holding user input
    char userInput[0xFF] = "";
    //get user input
    fgets(userInput, sizeof(userInput), stdin);
    //strip trailing newline
    userInput[strcspn(userInput, "\n")] = 0;
    if (strstr(userInput, "run")){
        //if it's a run command, split the string into its parameters
        SplitString splitString = split(userInput, " ");
        if (splitString.number_of_elements < 3){
            printf("Invalid number of parameters for 'run' command.\n");
        }
        else{
            //malloc a new job and set up its parameters from the string the user supplied
            Job* newJob = malloc(sizeof(Job));
            newJob->executable_name = malloc(sizeof(char) * (strlen(splitString.elements[1]) + 2));
            strcpy(newJob->executable_name, splitString.elements[1]);
            newJob->execution_time = atoi(splitString.elements[2]);
            newJob->priority = atoi(splitString.elements[3]);
            time(&newJob->submission_time);
            action->job = newJob;
            action->type = RUN;
        }
    }
    //if user wants to list the jobs
    if (strcmp(userInput, "list") == 0){
        //set up header 
        //TODO format this better
        printf("Name\tCPU_Time\tPriority\tArrival_Time\tProgress\n");
        //block until mutex lock so we can get the pointers to the active job and the job queue
        pthread_mutex_lock(&program_data->queue_mutex);
        //print current active job first
        Job* activeJob = program_data->activeJob;
        if (program_data->activeJob){
            printf("%s\t%ld\t%d\t%ld\tRun\n", activeJob->executable_name, activeJob->execution_time, 
                activeJob->priority, activeJob->submission_time);
        }
        //print the waiting jobs
        JobQueueNode* node = program_data->head;
        while(node){
            printf("%s\t%ld\t%d\t%ld\tWait\n", node->job->executable_name, node->job->execution_time, 
                node->job->priority, node->job->submission_time);
            node = node->next_node;
        }
        pthread_mutex_unlock(&program_data->queue_mutex);
        action->type = LIST;
    }
    if (!strcmp(userInput, "fcfs")){
        action->type = FCFS;
    }
    if (!strcmp(userInput, "sjf")){
        action->type = SJF;
    }
    if (!strcmp(userInput, "priority")){
        action->type = PRIORITY;
    }
    if (strstr(userInput, "test")){
        //if user wants to run the test job, split the parameters up
        SplitString splitString = split(userInput, " "); 
        //error checking
        if (splitString.number_of_elements < 5){
            printf("Invalid number of arguments for test command.\n");
            action->type = INVALID;
        }
        //if valid, malloc a new test and set its parameters
        else{
            //this will be free'd in Scheduler.c after the test is finished
            Test* test = malloc(sizeof(Test));
            test->benchmark = malloc(sizeof(char) * 50);
            strcpy(test->benchmark, splitString.elements[1]);
            test->policy = splitString.elements[2];
            test->number_of_jobs = atoi(splitString.elements[3]);
            test->priority_levels = atoi(splitString.elements[4]);
            test->min_cpu_time = atoi(splitString.elements[5]);
            test->max_cpu_time = atoi(splitString.elements[6]);
            test->jobs_per_second = 0xFFFFFFFF; //default value
            action->type = TEST;
            action->test = test;
        }
    }
    if (!strcmp(userInput, "eval")){
        action->type = EVAL;
    }
    if (!strcmp(userInput, "quit")){
        printf("Exiting. Please wait for current job to finish, or CTRL+C to forcefully terminate\n");
        //change running status variable to false so that the Scheduler and Dispatch will quit the next time their loop runs
        pthread_mutex_lock(&program_data->running_mutex);
        program_data->running = 0;
        pthread_mutex_unlock(&program_data->running_mutex);
        action->type = QUIT;
    }
    //print help menu
    if (!strcmp(userInput, "help")){
        printf("run <job> <time> <pri>: submit a job named <job>, execution time is <time>\n");
        printf("list: display the job status.\n");
        printf("fcfs: change the scheduling policy to first come first serve.\n");
        printf("sjf: change the scheduling policy to shortest job first.\n");
        printf("priority: change the scheduling policy to priority.\n");
        printf("test <benchmark> <policy> <num_of_jobs> <priority_levels> <min_CPU_time> <max_CPU_time>\n");
        printf("quit: exit CSUbatch\n");
        action->type = HELP;
    }
    //error checking response
    if (action->type == INVALID){
        printf("Invalid input! Type 'help' to find more about CSUBatch commands.\n");
    }
    return action;
}