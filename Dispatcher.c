#include "Dispatcher.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "Job.h"

void process_terminated(pid_t id){
    printf("todo\n");
}

void execute(char* parameters[]){
    pid_t child_id = fork();
    pid_t temp_id;
    int child_status;

    if (child_id == 0){
        execv(parameters[0], parameters);
        exit(0);
    }
    else{
        do {
            temp_id = wait(&child_status);
            if(temp_id != child_id) process_terminated(temp_id);
        } while(temp_id != child_id);
    }
}

void* dispatcher_loop(void* args){
    //lock mutex
    pthread_mutex_lock(&queue_mutex);
    //wait for condition to be true
    pthread_cond_wait(&work_available, &queue_mutex);
    //do work
    if (!shared_job){
        printf("Shared job is null!\n");
    }
    else if (!shared_job->executable_name){
        printf("executable name is null!\n");
    }
    else if (!shared_job->parameter_list){
        printf("parameter list is null!\n");
    }
    printf("Calling command...\n");
    execute(shared_job->parameter_list);
    
    //unlock mutex
    pthread_mutex_unlock(&queue_mutex);
    pthread_exit(NULL);
}