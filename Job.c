#include "Job.h"

#include <stdlib.h>
#include <time.h>

Job* init_job(char* executable_name, char* parameter_list[], int priority, long execution_time, time_t submission_time){
    Job* toReturn = malloc(sizeof(Job));
    toReturn->executable_name = executable_name;
    toReturn->parameter_list = parameter_list;
    toReturn->priority = priority;
    toReturn->execution_time = execution_time;
    toReturn->submission_time = submission_time;
    return toReturn;
}