#include "UI.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include "Job.h"
#include "ThreadsafeTypes.h"

void* scheduler_loop(void* args){
    ThreadsafeData *program_data = (ThreadsafeData*)args;
    int running = 0;
    while(1){
        //get user input
        Action action = get_user_input();
        //check if the user quit
        pthread_mutex_lock(&program_data->running_mutex);
        running = program_data->running;
        pthread_mutex_unlock(&program_data->running_mutex);
        //if the user quit, signal to dispatcher to do stuff, and exit
        if (!running){
            pthread_cond_signal(&program_data->work_available);
            pthread_exit(NULL);
        }
        //else carry out the user's request
        switch(action.type){
        case(RUN):
            //queue job
            break;
        case(FCFS):
            //fcfs scheduling
            break;
        case(SJF):
            //sjf scheduling
            break;
        case(PRIORITY):
            //priority scheduling
            break;
        }
    }
    /*
    //lock mutex
    pthread_mutex_lock(&queue_mutex);
    //insert data
    char* params[] = {"/bin/ls", "-l", "/home/clay", NULL};
    shared_job = init_job("/bin/ls", params, 0, 10);
    //signal that job is ready
    pthread_cond_signal(&work_available);
    //unlock mutex
    pthread_mutex_unlock(&queue_mutex);
    //exit?*/
}