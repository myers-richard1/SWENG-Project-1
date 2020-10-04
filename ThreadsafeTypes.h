#pragma once

#include <pthread.h>
#include "Job.h"

typedef struct ThreadsafeData{
    int running; //set to 0 by UI if program should stop. This is written to by UI and read by Scheduler and Dispatcher
    pthread_mutex_t running_mutex; //used to make 'running' threadsafe    

    pthread_mutex_t queue_mutex; //this mutex protects the below variables
    JobQueueNode* head; //this is the first node in the queue
    pthread_cond_t work_available; //used by scheduler to signal to the dispatcher that there's work to be done
    pthread_cond_t test_finished; //used by dispatcher to signal to the scheduler that the test is done
    double total_response_time; //used to keep total response time so it can be averaged later
    

    Job* activeJob; //this is the job that's been removed from the queue by the dispatcher that's still running
} ThreadsafeData;


