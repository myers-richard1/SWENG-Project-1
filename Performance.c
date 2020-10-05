#include "Performance.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "UI.h"

/**
 * Just a helper function for generating random ints
 * */
int randomIntInRange(int lower, int upper){
    return (rand() % (upper - lower + 1)) + lower;
}

int num_jobs;

/**
 * This function takes 4 parameters:
 * Test: The test struct with the information describing the test to run
 * Program Data: The program data containing the job queue
 * Queue Func: A function pointer to the scheduler's queue function, since we need that to actually queue jobs
 * 
 * Like the name suggests, it reads the test parameters and queues up a set of jobs based on those parameters.
 * */
void beginTest(Test* test, ThreadsafeData* program_data, void (*queue_func)(Job*, ThreadsafeData*)){
    //check if current test is finished
    //check if there are jobs in queue or running currently
    pthread_mutex_lock(&program_data->queue_mutex);
    //if either of these are not null, there's a test running and we should wait for it to finish
    if (program_data->activeJob || program_data->head){
        printf("Beginning test...\n");
        //wait until test is done
        pthread_cond_wait(&program_data->test_finished, &program_data->queue_mutex);
    }
    //write the test results to the text file
    FILE* output = fopen("performance_evaluation.txt", "a");
    //if this is the first iteration of the function, nothing's happened yet, so don't write anything
    if (test_begun){
        write_test_result(output, program_data);
        //reset response time so the average will be correct
        program_data->total_response_time = 0.0;
    }
    //print the header for this test
    fprintf(output, "%s policy, %d jobs, %d jobs per second, %d duration\n", test->policy, test->number_of_jobs,
        test->jobs_per_second, test->max_cpu_time);
    fclose(output);
    
    pthread_mutex_unlock(&program_data->queue_mutex);

    //get the current time and the number of jobs to run so we can measure throughput
    time(&begin_time);
    num_jobs = test->number_of_jobs;
    test_begun = 1;

    //seed our rng
    srand(time(0));
    int jobs_submitted = 0;
    for (int i = 0; i < test->number_of_jobs; i++){
        //generate values for job
        int priority = randomIntInRange(0, test->priority_levels);
        int cputime = randomIntInRange(test->min_cpu_time, test->max_cpu_time);
        Job* job = malloc(sizeof(Job));
        job->executable_name = test->benchmark;
        job->priority = priority;
        job->execution_time = cputime;
        char** parameters = malloc(sizeof(char**) * 3);
        parameters[0] = job->executable_name;
        char* argument = malloc(sizeof(char*) * 50);
        sprintf(argument, "%d", cputime);
        parameters[1] = argument;
        parameters[2] = NULL;
        job->parameter_list = parameters;
        time(&job->submission_time);
        //queue the generated job
        queue_func(job, program_data);
        jobs_submitted++;
        //if we need to slow down submitting jobs, sleep for a bit
        //TODO make this work with values less than 1
        if (jobs_submitted == test->jobs_per_second){
            jobs_submitted = 0;
            sleep(1);
        }
    }
}

/**
 * This function takes the program_data and a function pointer to the queue function.
 * It runs a series of tests measuring how well each sorting policy does under various workloads.
 * */
void performance_eval(ThreadsafeData* program_data, void (*queue_func)(Job*, ThreadsafeData*)){
    //reset performance eval file
    FILE* output = fopen("performance_evaluation.txt", "w");
    fclose(output);
    printf("Beginning evaluation, please wait for tests to complete.\n");
    //these are the hardcoded test values
    int number_of_jobs[5] = {5, 10, 15, 20, 25};
    int jobs_per_second[5] = {1, 3, 5, 7, 9};
    int job_durations[5] = {1, 2, 4, 6, 8};
    char* policies[3] = {"fcfs", "sjf", "priority"};
    //run 15 tests, 5 for each scheduling policy
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 5; j++){
            //malloc a test and set up its parameters
            Test* test = malloc(sizeof(Test));
            test->benchmark = "batch_job";
            test->number_of_jobs = number_of_jobs[j];
            test->jobs_per_second = jobs_per_second[j];
            test->min_cpu_time = 1;
            test->max_cpu_time = job_durations[j];
            test->policy = policies[i];
            test->priority_levels = 5;
            //queue up the test's jobs, then free it
            beginTest(test, program_data, queue_func);
            free(test);
        }
    }
}

/**
 * Just a helper function for printing the test result to a file.
 * */
void write_test_result(FILE* output, ThreadsafeData* program_data){
        time_t now;
        time(&now);
        double elapsed = difftime(now, begin_time);
        double jps = elapsed / num_jobs;
        double average_response_time = program_data->total_response_time / (float)num_jobs;
        fprintf(output, "Average Response Time: %f seconds\n", average_response_time);
        fprintf(output, "Throughput: %f jobs per second\n", jps);
        fprintf(output, "==============================\n\n");
}