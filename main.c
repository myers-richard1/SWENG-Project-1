#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>

#include "Job.h"
#include "Scheduler.h"
#include "Dispatcher.h"
pthread_mutex_t queue_mutex;
pthread_cond_t work_available;

Job* shared_job;

void initialize(){
    
}

int main(){
    //initialize mutex and conditional
    pthread_mutex_init(&queue_mutex, NULL);
    pthread_cond_init(&work_available, NULL);

    //create both threads
    pthread_t firstThread, secondThread;
    printf("Before thread\n");
    pthread_create(&firstThread, NULL, scheduler_loop, NULL);
    pthread_create(&secondThread, NULL, dispatcher_loop, NULL);
    
    printf("Joining threads...\n");
    pthread_join(firstThread, NULL);
    printf("After first thread\n");
    pthread_join(secondThread, NULL);
    printf("After second thread\n");

    printf("Cleaning up...\n");
    //cleanup
    free(shared_job);
    printf("Exiting normally\n");
}