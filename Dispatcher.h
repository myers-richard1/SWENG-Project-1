#pragma once

#include <stdlib.h>

void process_terminated(pid_t id);
void execute(char* parameters[]);
void* dispatcher_loop(void* args);