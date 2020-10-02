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