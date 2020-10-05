#pragma once

#include "ThreadsafeTypes.h"
#include "UI.h"

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
void* scheduler_loop(void* args);

/**
 * This function takes a pointer to a job to be queued and 
 * a pointer to the program data to get the queue.
 * It simply adds the job to the tail of the queue
 * and then calls the sort function
 * */
void queue_job(Job* job, ThreadsafeData* program_data);

/**
 * This function sorts the jobs queue in the given 
 * program data. It uses a case switch on the 
 * sort_type to determine which parameter to sort by.
 * It returns the number of jobs that are waiting so 
 * we can inform the user how many jobs were rescheduled.
 * */
int sort_jobs(ThreadsafeData* program_data);
void process_eval();

int sort_type;

//unit test
void test_scheduler(ThreadsafeData* program_data);