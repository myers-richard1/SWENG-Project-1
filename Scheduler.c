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

void* scheduler_loop(void* args){
    ThreadsafeData *program_data = (ThreadsafeData*)args;
    int running = 0;
    int numsorted = 0;
    sort_type = FCFS;
    while(1){
        //get user input
        Action* action = get_user_input(program_data);
        //check if the user quit
        pthread_mutex_lock(&program_data->running_mutex);
        running = program_data->running;
        pthread_mutex_unlock(&program_data->running_mutex);
        //if the user quit, signal to dispatcher to do stuff, and exit
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
            beginTest(action->test, program_data, sort_type, queue_job);
            break;
        case(EVAL):;
            performance_eval(program_data, queue_job);
            break;
        case(FCFS):
            sort_type = FCFS;
            pthread_mutex_lock(&program_data->queue_mutex);
            numsorted = sort_jobs(program_data);
            pthread_mutex_unlock(&program_data->queue_mutex);
            printf("Scheduling policy is switched to FCFS. All %d waiting jobs have been rescheduled.\n", numsorted);
            break;
        case(SJF):
            sort_type = SJF;
            pthread_mutex_lock(&program_data->queue_mutex);
            numsorted = sort_jobs(program_data);
            pthread_mutex_unlock(&program_data->queue_mutex);
            printf("Scheduling policy is switched to SJF. All %d waiting jobs have been rescheduled.\n", numsorted);
            break;
        case(PRIORITY):
            sort_type = PRIORITY;
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
    }
}

void queue_job(Job* job, ThreadsafeData* program_data){
    JobQueueNode* new_node = malloc(sizeof(JobQueueNode));
    new_node->job = job;
    new_node->next_node = NULL;
    pthread_mutex_lock(&program_data->queue_mutex);
    //if head is null, set it to the new job
    if (!program_data->head) program_data->head = new_node;
    else{
        JobQueueNode* tail = program_data->head;
        while (tail->next_node != NULL){
            tail = tail->next_node;
        }
        tail->next_node = new_node;
    }
    sort_jobs(program_data);
    pthread_cond_signal(&program_data->work_available);    
    pthread_mutex_unlock(&program_data->queue_mutex);
}

int sort_jobs(ThreadsafeData* program_data){
    int sorted = 0;
    int nodecount;
    if (!program_data->head) return 0;
    while (sorted == 0){
        sorted = 1;
        JobQueueNode* current_job = program_data->head;
        JobQueueNode* next_job;
        nodecount = 0;
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
            default:
                printf("Current sorting algorithm invalid!\n");
                break;
            }
        }
    }
    nodecount++;
    return nodecount;
}