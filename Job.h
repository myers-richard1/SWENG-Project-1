#pragma once

#include <time.h>

typedef struct Job{
    char* executable_name;
    char** parameter_list;
    int priority;
    long execution_time;
    time_t submission_time;
} Job;

typedef struct JobQueueNode{
    Job* job;
    struct JobQueueNode* next_node;
} JobQueueNode;

Job* init_job(char* executable_name, char* parameter_list[], int priority, long execution_time, time_t submission_time);
void destroy_job(Job* job);