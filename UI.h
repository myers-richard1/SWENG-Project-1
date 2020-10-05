#pragma once

#include "Job.h"
#include "ThreadsafeTypes.h"

//This structure serves two purposes, both for the scheduler.
//This is used to tell the scheduler which type of action the user 
//requested, but the scheduler also uses an ActionType variable
//for its scheduling policy since it has FCFS, SJF, and PRIORITY values.
typedef enum ActionType{
    HELP, RUN, LIST, FCFS, SJF, PRIORITY, TEST, EVAL, QUIT, INVALID
} ActionType;

//The test struct is used for the test and eval commands 
//to tell the scheduler and performance modules what kind of workload
//to queue up
typedef struct Test{
    char* benchmark;
    char* policy;
    int number_of_jobs;
    int priority_levels;
    int min_cpu_time;
    int max_cpu_time;
    int jobs_per_second;
} Test;

//An action is made of an ActionType, a Job, and a Test.
//The job and test pointers will be null if the Action doesn't
//involve a Job or Test.
typedef struct Action{
    ActionType type;
    Job* job;
    Test* test;
} Action;

/**
 * This function takes in the threadsafe program data and returns an "action" struct
 * containing the user's request. This function should only be called by the
 * scheduler when it's checking to see what to do next.
 * */
Action* get_user_input(ThreadsafeData* program_data);