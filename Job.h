#pragma once

#include <time.h>

//Simple struct that tells the dispatcher what kind of job to run
typedef struct Job{
    char* executable_name;
    char** parameter_list;
    int priority;
    long execution_time;
    time_t submission_time;
} Job;

//Very simple implementation of a linked list node
typedef struct JobQueueNode{
    Job* job;
    struct JobQueueNode* next_node;
} JobQueueNode;

//Job struct creator. This doesn't have to be used but it can be handy if you forget what 
//parameters you need to make one.
Job* init_job(char* executable_name, char* parameter_list[], int priority, long execution_time, time_t submission_time);