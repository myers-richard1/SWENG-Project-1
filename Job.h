typedef struct Job{
    char* executable_name;
    char** parameter_list;
    int priority;
    long execution_time;
} Job;

Job* init_job(char* executable_name, char* parameter_list[], int priority, long execution_time);
void destroy_job(Job* job);