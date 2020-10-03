#include "UI.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Job.h"
#include "ThreadsafeTypes.h"
#include "StringUtils.h"

//Returns a NULL job if the user didnt add a new job
//program data is used for setting running status for threads to check if they should exit
Action* get_user_input(ThreadsafeData* program_data){
    //greet the user and print instructions
    printf(">");

    Action* action = malloc(sizeof(Action));
    action->type = INVALID;
    //init an array for holding user input
    char userInput[0xFF] = "";
    //get user input
    fgets(userInput, sizeof(userInput), stdin);
    //strip trailing newline
    userInput[strcspn(userInput, "\n")] = 0;
    if (strstr(userInput, "run")){
        //extract 
        SplitString splitString = split(userInput, " ");
        if (splitString.number_of_elements < 3){
            printf("Invalid number of parameters for 'run' command.\n");
        }
        else{
            //create new job
            Job* newJob = malloc(sizeof(Job));
            //newJob->executable_name = splitString.elements[1];
            newJob->executable_name = malloc(sizeof(char) * (strlen(splitString.elements[1]) + 2));
            strcpy(newJob->executable_name, splitString.elements[1]);
            newJob->execution_time = atoi(splitString.elements[2]);
            newJob->priority = atoi(splitString.elements[3]);
            time(&newJob->submission_time);
            action->job = newJob;
            action->type = RUN;
        }
    }
    if (strcmp(userInput, "list") == 0){
        printf("Name\tCPU_Time\tPriority\tArrival_Time\tProgress\n");
        pthread_mutex_lock(&program_data->queue_mutex);
        Job* activeJob = program_data->activeJob;
        if (program_data->activeJob){
            printf("%s\t%ld\t%d\t%ld\tRun\n", activeJob->executable_name, activeJob->execution_time, 
                activeJob->priority, activeJob->submission_time);
        }
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
        SplitString splitString = split(userInput, " "); 
        if (splitString.number_of_elements < 5){
            printf("Invalid number of arguments for test command.\n");
            action->type = INVALID;
        }
        else{
            Test* test = malloc(sizeof(Test));
            test->benchmark = malloc(sizeof(char) * 50);
            strcpy(test->benchmark, splitString.elements[1]);
            test->policy = splitString.elements[2];
            test->number_of_jobs = atoi(splitString.elements[3]);
            test->priority_levels = atoi(splitString.elements[4]);
            test->min_cpu_time = atoi(splitString.elements[5]);
            test->max_cpu_time = atoi(splitString.elements[6]);
            test->jobs_per_second = 0xFFFFFFFF;
            action->type = TEST;
            action->test = test;
        }
    }
    if (!strcmp(userInput, "eval")){
        action->type = EVAL;
    }
    if (!strcmp(userInput, "quit")){
        pthread_mutex_lock(&program_data->running_mutex);
        program_data->running = 0;
        pthread_mutex_unlock(&program_data->running_mutex);
        action->type = QUIT;
    }
    if (!strcmp(userInput, "help")){
        printf("run <job> <time> <pri>: submit a job named <job>, execution time is <time>\n");
        printf("list: display the job status.\n");
        printf("fcfs: change the scheduling policy to priority.\n");
        printf("test <benchmark> <policy> <num_of_jobs> <priority_levels> <min_CPU_time> <max_CPU_time>\n");
        printf("quit: exit CSUbatch\n");
        action->type = HELP;
    }
    if (action->type == INVALID){
        printf("Invalid input! Type 'help' to find more about CSUBatch commands.\n");
    }
    //debug
    if (action->type == RUN){
        printf("UI: Returning action with job executable name %s, jobaddress: %p\n", action->job->executable_name, action->job);
        printf("UI: address of string: %p\n", action->job->executable_name);
    }

    if (action->type == TEST){
        printf("Returning test action for benchmark: %s\n", action->test->benchmark);
        printf("Benchmark string is at %p\n", action->test->benchmark);
    }
    return action;
}