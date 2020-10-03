#pragma once

#include "ThreadsafeTypes.h"

void* scheduler_loop(void* args);
int sort_jobs(ThreadsafeData* program_data);