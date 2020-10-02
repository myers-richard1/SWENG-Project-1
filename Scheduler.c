#include "UI.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include "Job.h"


void* scheduler_loop(void* args){
    while(1){
        get_user_input();
    }
    
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