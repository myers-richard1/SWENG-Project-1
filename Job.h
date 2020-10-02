typedef struct Job{
    char* executable_name;
    char** parameter_list;
    int priority;
    long execution_time;
} Job;

typedef struct JobQueueNode{
    Job* this_job;
    Job* next_job;
} JobQueueNode;

pthread_mutex_t queue_mutex;
pthread_cond_t work_available;

Job* shared_job;

Job* init_job(char* executable_name, char* parameter_list[], int priority, long execution_time);
void destroy_job(Job* job);