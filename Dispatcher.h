#pragma once

#include "ThreadsafeTypes.h"
/**
 * This function takes an executable name and parameter list,
 * and spawns a new process using fork(),
 * then passes the executable name and parameters to 
 * execv() to run it.
 * */
void execute(char* executable_name, char** parameters);

/**
 * This method should not be called manually, but rather
 * passed to the pthread_create function, hence the void pointer
 * return type and arguments.
 * 
 * The function loops endlessly until the UI module sets the 
 * running variable in program_data to false.
 * 
 * This function removes jobs from the queue and executes them,
 * and then signals to the Scheduler that it's finished if all the jobs
 * are done.
 * */
void* dispatcher_loop(void* args);

//unit test
void test_dispatch(ThreadsafeData* program_data);