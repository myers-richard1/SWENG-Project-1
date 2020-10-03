#pragma once

#include <stdlib.h>

void process_terminated(pid_t id);
void execute(char* executable_name);
void* dispatcher_loop(void* args);