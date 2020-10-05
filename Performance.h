#pragma once

#include <time.h>
#include <stdio.h>
#include "ThreadsafeTypes.h"
#include "UI.h"

void beginTest(Test* test, ThreadsafeData* program_data, ActionType sort_type, void (*queue_func)(Job*, ThreadsafeData*));
void write_test_result(FILE* output, ThreadsafeData* program_data);
void performance_eval(ThreadsafeData* program_data, void (*queue_func)(Job*, ThreadsafeData*));

int test_begun;
time_t begin_time;