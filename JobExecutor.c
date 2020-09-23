#include <stdlib.h>
#include "JobExecutor.h"

int executeJob(char* command){
    int result = system(command);
    return result;
}