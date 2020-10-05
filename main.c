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
#include "ThreadsafeTypes.h"
#include "UI.h"

//Program state shared between all threads.
//This structure contains mutexes and conditional variables for both the JobQueue
//and for the variable that tells the threads to stop running
ThreadsafeData program_data;

void run_unit_tests(ThreadsafeData* program_data);

int main(int argc, char* args[]){
    //initialize mutex and conditional for job queue
    pthread_mutex_init(&program_data.queue_mutex, NULL);
    pthread_cond_init(&program_data.work_available, NULL);

    //initialize conditional for test
    pthread_cond_init(&program_data.test_finished, NULL);

    //initialize mutex for sentinel loop controller
    pthread_mutex_init(&program_data.running_mutex, NULL);
    //set the value directly since there's only 1 thread atm
    program_data.running = 1;

    if (argc > 1){
        if (!strcmp(args[1], "test")){
            printf("Program launched in test mode, beginning unit tests.\n");
            run_unit_tests(&program_data);
            printf("All tests finished, exiting\n");
            return 0;
        }
    }

    printf("Welcome to Richard and Harrison's batch job scheduler Version 1.0\n");
    printf("Type 'help' to find more about CSUBatch commands.\n");

    //create both threads
    pthread_t firstThread, secondThread;
    pthread_create(&firstThread, NULL, scheduler_loop, &program_data);
    pthread_create(&secondThread, NULL, dispatcher_loop, &program_data);
    
    //block until user exits
    pthread_join(firstThread, NULL);
    pthread_join(secondThread, NULL);

    printf("Finished.\n");
    return 0;
}

void run_unit_tests(ThreadsafeData* program_data){
    test_ui(program_data);
    test_scheduler(program_data);
    test_dispatch(program_data);
}