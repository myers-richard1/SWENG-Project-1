#pragma once

#include "Job.h"

typedef enum ActionType{
    HELP, RUN, LIST, FCFS, SJF, PRIORITY, QUIT, INVALID
} ActionType;

Job* get_user_input();