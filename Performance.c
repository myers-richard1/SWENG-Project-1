#include "Performance.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "UI.h"

int randomIntInRange(int lower, int upper){
    return (rand() % (upper - lower + 1)) + lower;
}

int num_jobs;
void beginTest(Test* test, ThreadsafeData* program_data, ActionType sort_type, void (*queue_func)(Job*, ThreadsafeData*)){
    //check if current test is finished
    //check if there are jobs in queue or running currently
    pthread_mutex_lock(&program_data->queue_mutex);
    //if either of these are not null, there's a test running and we should wait for it to finish
    if (program_data->activeJob || program_data->head){
        printf("Beginning test...\n");
        //wait until test is done
        pthread_cond_wait(&program_data->test_finished, &program_data->queue_mutex);
    }
    FILE* output = fopen("performance_evaluation.txt", "a");
    if (test_begun){
        write_test_result(output, program_data);
        
        program_data->total_response_time = 0.0;//reset response time while we have the mutex locked
    }
    fprintf(output, "%s policy, %d jobs, %d jobs per second, %d duration\n", test->policy, test->number_of_jobs,
        test->jobs_per_second, test->max_cpu_time);
    fclose(output);
    
    pthread_mutex_unlock(&program_data->queue_mutex);

    time(&begin_time);
    num_jobs = test->number_of_jobs;
    test_begun = 1;
    
    if (!strcmp("fcfs", test->policy)) sort_type = FCFS;
    if (!strcmp("priority", test->policy)) sort_type = PRIORITY;
    if (!strcmp("sjf", test->policy)) sort_type = SJF;
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
        queue_func(job, program_data);
        jobs_submitted++;
        if (jobs_submitted == test->jobs_per_second){
            jobs_submitted = 0;
            sleep(1);
        }
    }
}

void performance_eval(ThreadsafeData* program_data, void (*queue_func)(Job*, ThreadsafeData*)){
    //reset performance eval file
    FILE* output = fopen("performance_evaluation.txt", "w");
    fclose(output);
    printf("Beginning evaluation, please wait for tests to complete.\n");
    int number_of_jobs[5] = {5, 10, 15, 20, 25};
    int jobs_per_second[5] = {1, 3, 5, 7, 9};
    int job_durations[5] = {1, 2, 4, 6, 8};
    char* policies[3] = {"fcfs", "sjf", "priority"};
    ActionType policyEnums[3] = {FCFS, SJF, PRIORITY};
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 5; j++){
            Test* test = malloc(sizeof(Test));
            test->benchmark = "batch_job";
            test->number_of_jobs = number_of_jobs[j];
            test->jobs_per_second = jobs_per_second[j];
            test->min_cpu_time = 1;
            test->max_cpu_time = job_durations[j];
            test->policy = policies[i];
            test->priority_levels = 5;
            beginTest(test, program_data, policyEnums[i], queue_func);
            free(test);
        }
    }
}

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