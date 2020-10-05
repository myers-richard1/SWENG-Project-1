#pragma once

#include <time.h>
#include <stdio.h>
#include "ThreadsafeTypes.h"
#include "UI.h"

/**
 * This function takes 4 parameters:
 * Test: The test struct with the information describing the test to run
 * Program Data: The program data containing the job queue
 * Queue Func: A function pointer to the scheduler's queue function, since we need that to actually queue jobs
 * 
 * Like the name suggests, it reads the test parameters and queues up a set of jobs based on those parameters
 * */
void beginTest(Test* test, ThreadsafeData* program_data, void (*queue_func)(Job*, ThreadsafeData*));




/**
 * This function takes the program_data and a function pointer to the queue function.
 * It runs a series of tests measuring how well each sorting policy does under various workloads.
 * */
void performance_eval(ThreadsafeData* program_data, void (*queue_func)(Job*, ThreadsafeData*));

/**
 * Just a helper function for printing the test result to a file.
 * */
void write_test_result(FILE* output, ThreadsafeData* program_data);

int test_begun;
time_t begin_time;