#pragma once

#include "Job.h"
#include "ThreadsafeTypes.h"

typedef enum ActionType{
    HELP, RUN, LIST, FCFS, SJF, PRIORITY, TEST, QUIT, INVALID
} ActionType;

typedef struct Action{
    ActionType type;
    Job* job;
} Action;

Action get_user_input(ThreadsafeData* program_data);