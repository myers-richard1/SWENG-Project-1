#include "Scheduler.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include "Job.h"
#include "ThreadsafeTypes.h"
#include "UI.h"
#include "Performance.h"

/**
 * This method should not be called manually, but rather
 * passed to the pthread_create function, hence the void pointer
 * return type and arguments.
 * 
 * The function loops endlessly until the UI module sets the 
 * running variable in program_data to false.
 * 
 * The logic of the function is pretty simple. Gets an Action
 * from the UI module, and react to it.
 * */
void* scheduler_loop(void* args){
    //get parameter passed from main
    ThreadsafeData *program_data = (ThreadsafeData*)args;
    //set up our stack vars outside the loop
    int running = 0;
    int numsorted = 0;
    //default sort type
    sort_type = FCFS;
    while(1){
        //get user input
        Action* action = get_user_input(program_data);
        //check if the user quit
        pthread_mutex_lock(&program_data->running_mutex);
        running = program_data->running;
        pthread_mutex_unlock(&program_data->running_mutex);
        //if the user quit, join back to main thread
        if (!running){
            //signal dispatcher thread to wake up if it happens to be waiting for work
            pthread_mutex_lock(&program_data->queue_mutex);
            pthread_cond_signal(&program_data->work_available);
            pthread_mutex_unlock(&program_data->queue_mutex);
            pthread_exit(NULL);
        }
        //else carry out the user's request
        switch(action->type){
        case(RUN):;
            queue_job(action->job, program_data);
            break;
        case(TEST):
            beginTest(action->test, program_data, queue_job);
            break;
        case(EVAL):;
            performance_eval(program_data, queue_job);
            break;
        case(FCFS):
            sort_type = FCFS;
            //sort the waiting jobs and tell the user how many jobs were rescheduled
            pthread_mutex_lock(&program_data->queue_mutex);
            numsorted = sort_jobs(program_data);
            pthread_mutex_unlock(&program_data->queue_mutex);
            printf("Scheduling policy is switched to FCFS. All %d waiting jobs have been rescheduled.\n", numsorted);
            break;
        case(SJF):
            sort_type = SJF;
            //sort the waiting jobs and tell the user how many jobs were rescheduled
            pthread_mutex_lock(&program_data->queue_mutex);
            numsorted = sort_jobs(program_data);
            pthread_mutex_unlock(&program_data->queue_mutex);
            printf("Scheduling policy is switched to SJF. All %d waiting jobs have been rescheduled.\n", numsorted);
            break;
        case(PRIORITY):
            sort_type = PRIORITY;
            //sort the waiting jobs and tell the user how many jobs were rescheduled
            pthread_mutex_lock(&program_data->queue_mutex);
            numsorted = sort_jobs(program_data);
            pthread_mutex_unlock(&program_data->queue_mutex);
            printf("Scheduling policy is switched to priority. All %d waiting jobs have been rescheduled.\n", numsorted);
            break;
        case(HELP):
        case(LIST):
        case(QUIT):
        case(INVALID):
        default:
            break;
        }
        //free the action
        //dont free the job or test here because those get freed 
        //in dispatch.c and performance.c respectively
        if (action){
            free(action);
            action = NULL;
        }
    }
}

/**
 * This function takes a pointer to a job to be queued and 
 * a pointer to the program data to get the queue.
 * It simply adds the job to the tail of the queue
 * and then calls the sort function
 * */
void queue_job(Job* job, ThreadsafeData* program_data){
    //set up new jobqueuenode
    JobQueueNode* new_node = malloc(sizeof(JobQueueNode));
    new_node->job = job;
    new_node->next_node = NULL;
    pthread_mutex_lock(&program_data->queue_mutex);
    //if head is null, set it to the new job
    if (!program_data->head) program_data->head = new_node;
    //else search for the tail of the queue and append this new node
    else{
        JobQueueNode* tail = program_data->head;
        while (tail->next_node != NULL){
            tail = tail->next_node;
        }
        tail->next_node = new_node;
    }
    //sort the jobs, signal to the dispatcher that there's work to be done, and unlock the mutex
    sort_jobs(program_data);
    pthread_cond_signal(&program_data->work_available);    
    pthread_mutex_unlock(&program_data->queue_mutex);
}

/**
 * This function sorts the jobs queue in the given 
 * program data. It uses a case switch on the 
 * sort_type to determine which parameter to sort by.
 * It returns the number of jobs that are waiting so 
 * we can inform the user how many jobs were rescheduled.
 * */
int sort_jobs(ThreadsafeData* program_data){
    //sorted is a flag that keeps track of whether the queue is sorted or not.
    //since it's bubble sort, we have to keep sorting over and over until
    //the whole queue is sorted.
    int sorted = 0;
    //here we keep track of the total number of nodes
    //so we can imform the user how many nodes were sorted
    int nodecount;
    if (!program_data->head) return 0;
    while (sorted == 0){
        sorted = 1;
        //this is the current node we're sorting
        JobQueueNode* current_job = program_data->head;
        //this is the node we're comparing to
        JobQueueNode* next_job;
        nodecount = 0;
        //loop through until we hit the tail
        while(current_job->next_node){
            nodecount++;
            next_job = current_job->next_node;
            //compare
            switch(sort_type){
            case(FCFS):
                //if current job is later than next job, swap them
                if (difftime(current_job->job->submission_time, next_job->job->submission_time) > 0){
                    Job* temp = current_job->job;
                    current_job->job = next_job->job;
                    next_job->job = temp;
                    sorted = 0;
                }
                //move to next job
                else{
                    current_job = next_job;
                }
                break;
            case(SJF):
                //if current job is longer than next job, swap them
                if (current_job->job->execution_time > next_job->job->execution_time){
                    Job* temp = current_job->job;
                    current_job->job = next_job->job;
                    next_job->job = temp;
                    sorted = 0;
                }
                //move to next job
                else{
                    current_job = next_job;
                }
                break;
            case(PRIORITY):;
                //if current job has lower priority than next job, swap them
                if (current_job->job->priority < next_job->job->priority){
                    Job* temp = current_job->job;
                    current_job->job = next_job->job;
                    next_job->job = temp;
                    sorted = 0;
                }
                //move to next job
                else{
                    current_job = next_job;
                }
                break;
            //error checking
            default:
                printf("Current sorting algorithm invalid!\n");
                break;
            }
        }
    }
    nodecount++;
    return nodecount;
}