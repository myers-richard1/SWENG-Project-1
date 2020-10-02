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
    ThreadsafeData *program_data = (ThreadsafeData*)args;
    while(1){
        //lock mutex
        pthread_mutex_lock(&program_data->queue_mutex);
        //check if jobs in queue
        JobQueueNode* head = program_data->head;
        if (head == NULL)
            //wait for condition to be true
            pthread_cond_wait(&program_data->work_available, &program_data->queue_mutex);
        //make sure program is still running
        pthread_mutex_lock(&program_data->running_mutex);
        int running = program_data->running;
        pthread_mutex_unlock(&program_data->running_mutex);
        if (!running){
            pthread_mutex_unlock(&program_data->queue_mutex);
            pthread_exit(NULL);
        }
        //get tail of list
        JobQueueNode* jobToExecute = program_data->head;
        while(jobToExecute->next_node != NULL){
            jobToExecute = jobToExecute->next_node;
        }
        //todo remove from queue
        pthread_mutex_unlock(&program_data->queue_mutex);
        //execute job
        execute(jobToExecute->job->parameter_list);    
    }
}