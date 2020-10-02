#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>

#include "Job.h"
pthread_mutex_t queue_mutex;
pthread_cond_t work_available;

Job* shared_job;

void* schedulerInitialization(void* args){
    sleep(1);
    //lock mutex
    pthread_mutex_lock(&queue_mutex);
    //insert data
    char* params[] = {"/bin/ls", "-l", "/home/clay", NULL};
    shared_job = init_job("/bin/ls", params, 0, 10);
    //signal that job is ready
    pthread_cond_signal(&work_available);
    //unlock mutex
    pthread_mutex_unlock(&queue_mutex);
    //exit?

    sleep(5);
    
    pthread_exit(NULL);
}

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

void* dispatcherInitialization(void* args){
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

int main(){
    //initialize mutex and conditional
    pthread_mutex_init(&queue_mutex, NULL);
    pthread_cond_init(&work_available, NULL);

    //create both threads
    pthread_t firstThread, secondThread;
    printf("Before thread\n");
    pthread_create(&firstThread, NULL, schedulerInitialization, NULL);
    pthread_create(&secondThread, NULL, dispatcherInitialization, NULL);
    
    printf("Joining threads...\n");
    pthread_join(firstThread, NULL);
    printf("After first thread\n");
    pthread_join(secondThread, NULL);
    printf("After second thread\n");

    printf("Cleaning up...\n");
    //cleanup
    free(shared_job);
    return 0;
}