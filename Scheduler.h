#pragma once

#include "ThreadsafeTypes.h"
#include "UI.h"

void* scheduler_loop(void* args);
void queue_job(Job* job, ThreadsafeData* program_data);
int sort_jobs(ThreadsafeData* program_data);
void process_eval();

int sort_type;