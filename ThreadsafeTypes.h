#pragma once

#include <pthread.h>
#include "Job.h"

typedef struct ThreadsafeData{
    int running; //set to 0 by UI if program should stop. This is written to by UI and read by Scheduler and Dispatcher
    pthread_mutex_t running_mutex; //used to make 'running' threadsafe    

    JobQueueNode* head; //this is the first node in the queue
    pthread_mutex_t queue_mutex; //used to make the queue threadsafe
    pthread_cond_t work_available; //used to signal to the dispatcher that there's work to be done
    Job* activeJob; //this is the job that's been removed from the queue by the dispatcher that's still running
} ThreadsafeData;


