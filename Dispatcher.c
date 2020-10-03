#include "Dispatcher.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "Job.h"
#include "ThreadsafeTypes.h"

void execute(char* executable_name){
    pid_t child_id = fork();
    pid_t temp_id;
    int child_status;

    char* parameters[2] = {executable_name, NULL};

    if (child_id == 0){
        printf("Forked thread running execv on job called %s\n", executable_name);
        //execv(executable_name, parameters);
        execv(executable_name, parameters);
        exit(0);
    }
    else{
        do {
            temp_id = wait(&child_status);
        } while(temp_id != child_id);
    }
}

void* dispatcher_loop(void* args){
    ThreadsafeData *program_data = (ThreadsafeData*)args;
    while(1){
        //lock mutex
        pthread_mutex_lock(&program_data->queue_mutex);
        //check if jobs in queue
        JobQueueNode* head = program_data->head;
        if (!head){
            //block until there's work to do
            printf("Dispatch: Waiting for work\n");
            pthread_cond_wait(&program_data->work_available, &program_data->queue_mutex);
            printf("Dispatch: Awake\n");
        }
        //make sure the program is still supposed to be running
        pthread_mutex_lock(&program_data->running_mutex);
        int running = program_data->running;
        pthread_mutex_unlock(&program_data->running_mutex);
        //if program should close, close
        if (!running){
            printf("Dispatch: Told to exit, exiting\n");
            pthread_mutex_unlock(&program_data->queue_mutex);
            pthread_exit(NULL);
        }
        //get head of list
        JobQueueNode* jobToExecute = program_data->head;
        if (!jobToExecute){
            printf("Error! Job to execute is null!\n");
        }
        //remove from queue
        program_data->head = program_data->head->next_node;
        //mark as active job
        program_data->activeJob = jobToExecute->job;
        //unlock mutex
        pthread_mutex_unlock(&program_data->queue_mutex);
        //execute job
        printf("Dispatch: Executing job called %s\n", jobToExecute->job->executable_name);
        execute(jobToExecute->job->executable_name); 
        //set active job to null since we're finished with the job
        pthread_mutex_lock(&program_data->queue_mutex);
        program_data->activeJob = NULL;
        pthread_mutex_unlock(&program_data->queue_mutex);
          
    }
}