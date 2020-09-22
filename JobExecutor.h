#pragma once

#include <stdlib.h>



int executeJob(char* command){
    int result = system(command);
    return result;
}