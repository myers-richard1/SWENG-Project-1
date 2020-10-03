#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int c, char* args[]){
    
    if (c >= 2){
        int sleep_duration = atoi(args[1]);
        sleep(sleep_duration);
    }
    else{
        printf("Batch job: not enough args\n");
    }
    return 0;
}