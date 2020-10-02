#include <stdlib.h>

#include "Job.h"


Job* init_job(char* executable_name, char* parameter_list[], int priority, long execution_time){
    Job* toReturn = malloc(sizeof(Job));
    toReturn->executable_name = executable_name;
    toReturn->parameter_list = parameter_list;
    toReturn->priority = priority;
    toReturn->execution_time = execution_time;
    return toReturn;
}