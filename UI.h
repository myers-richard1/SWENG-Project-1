#pragma once

#include "Job.h"

typedef enum ActionType{
    HELP, RUN, LIST, FCFS, SJF, PRIORITY, QUIT, INVALID
} ActionType;

typedef struct Action{
    ActionType type;
    Job* job;
} Action;

Action get_user_input();