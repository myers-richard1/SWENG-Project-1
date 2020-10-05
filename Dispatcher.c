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

/**
 * This function takes an executable name and parameter list,
 * and spawns a new process using fork(),
 * then passes the executable name and parameters to 
 * execv() to run it.
 * */
void execute(char* executable_name, char** parameters){
    //spawn child process
    pid_t child_id = fork();
    pid_t temp_id;
    int child_status;

    //if i'm the child,
    if (child_id == 0){
        //init parameters if it's null (if the job was queued by run and has no additional params)
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
    //else if i'm the parent process,
    //wait until the child is done
    else{
        do {
            temp_id = wait(&child_status);
        } while(temp_id != child_id);
    }
}

/**
 * This method should not be called manually, but rather
 * passed to the pthread_create function, hence the void pointer
 * return type and arguments.
 * 
 * The function loops endlessly until the UI module sets the 
 * running variable in program_data to false.
 * 
 * This function removes jobs from the queue and executes them,
 * and then signals to the Scheduler that it's finished if all the jobs
 * are done.
 * */
void* dispatcher_loop(void* args){
    //get the arguments from main()
    ThreadsafeData *program_data = (ThreadsafeData*)args;
    
    while(1){
        //make sure the program is still supposed to be running
        pthread_mutex_lock(&program_data->queue_mutex);
        pthread_mutex_lock(&program_data->running_mutex);
        int running = program_data->running;
        pthread_mutex_unlock(&program_data->running_mutex);
        if (!running) pthread_exit(NULL);

        //check if jobs in queue
        JobQueueNode* head = program_data->head;
        //if no jobs in queue,
        if (!head){
            //signal that there's no work left in case the scheduler is waiting to run another test
            pthread_cond_signal(&program_data->test_finished);
            //wait until work is available
            pthread_cond_wait(&program_data->work_available, &program_data->queue_mutex);
        }
        //if we're here, it means the user added a job OR told the program to quit while we were waiting
        //make sure the program is still supposed to be running
        pthread_mutex_lock(&program_data->running_mutex);
        running = program_data->running;
        pthread_mutex_unlock(&program_data->running_mutex);
        //if program should close, close
        if (!running){
            pthread_mutex_unlock(&program_data->queue_mutex);
            pthread_exit(NULL);
        }

        //get head of list
        JobQueueNode* jobToExecute = program_data->head;
        //error checking
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
        execute(jobToExecute->job->executable_name, jobToExecute->job->parameter_list); 
        //set active job to null since we're finished with the job
        pthread_mutex_lock(&program_data->queue_mutex);
        time_t end_time;
        time(&end_time);
        double elapsedTime = difftime(end_time, jobToExecute->job->submission_time);
        program_data->total_response_time += elapsedTime;
        //free job
        free(program_data->activeJob);
        //free node that was holding the job
        free(jobToExecute);
        program_data->activeJob = NULL;
        
        pthread_mutex_unlock(&program_data->queue_mutex);
    }
}

void test_dispatch(ThreadsafeData* program_data){
    //attempt running executable
    char* executable_name = "/bin/touch";
    char** params = malloc(sizeof(char*) * 2);
    params[0] = executable_name;
    char* filename = malloc(sizeof(char) * 4);
    strcpy(filename, "test");
    params[1] = filename;

    execute(executable_name, params);
    if( access( "test", F_OK ) != -1 ) {
        printf("Dispatch test 1 passed!\n");
        remove("test");
    } else {
        printf("Dispatch test 1 failed!\n");
    }

    //attempt reading shared data setting active job
    if (program_data->head->job->execution_time == 1) printf("Dispatch test 2 passed!\n");
    else printf("Dispatch test 1 failed!\n");
    program_data->activeJob = program_data->head->job;
    program_data->head = program_data->head->next_node;

    if (program_data->head->job->execution_time== 5) printf("Dispatch test 3 passed\n");
    else printf("Dispatch test 2 failed!\n");

    
    
}