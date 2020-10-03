#pragma once

#include "Job.h"
#include "ThreadsafeTypes.h"

typedef enum ActionType{
    HELP, RUN, LIST, FCFS, SJF, PRIORITY, TEST, EVAL, QUIT, INVALID
} ActionType;

typedef struct Test{
    char* benchmark;
    char* policy;
    int number_of_jobs;
    int priority_levels;
    int min_cpu_time;
    int max_cpu_time;
    int jobs_per_second;
} Test;


typedef struct Action{
    ActionType type;
    Job* job;
    Test* test;
} Action;

Action* get_user_input(ThreadsafeData* program_data);