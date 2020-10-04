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

void execute(char* executable_name, char** parameters){
    pid_t child_id = fork();
    pid_t temp_id;
    int child_status;

    if (child_id == 0){
        if (!parameters){
            parameters = malloc(sizeof(char*) * 2);
            parameters[0] = executable_name;
            parameters[1] = NULL;
        }
        else{
        }
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
            //signal that there's no work left in case the scheduler is waiting to run another test
            printf("Dispatch: No work left, signalling and waiting for work\n");
            pthread_cond_signal(&program_data->test_finished);
            //wait until work is available
            pthread_cond_wait(&program_data->work_available, &program_data->queue_mutex);
        }
        //make sure the program is still supposed to be running
        pthread_mutex_lock(&program_data->running_mutex);
        int running = program_data->running;
        pthread_mutex_unlock(&program_data->running_mutex);
        //if program should close, close
        if (!running){
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
        printf("Dispatcher: Starting job\n");
        execute(jobToExecute->job->executable_name, jobToExecute->job->parameter_list); 
        //set active job to null since we're finished with the job
        printf("Dispatcher: job finished\n");
        pthread_mutex_lock(&program_data->queue_mutex);
        time_t end_time;
        time(&end_time);
        double elapsedTime = difftime(end_time, jobToExecute->job->submission_time);
        program_data->total_response_time += elapsedTime;
        program_data->activeJob = NULL;
        
        pthread_mutex_unlock(&program_data->queue_mutex);
        
    }
}