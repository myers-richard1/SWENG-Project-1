#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

//just sleeps
int main(int c, char* args[]){
    
    if (c >= 2){
        int seconds = atoi(args[1]);
        sleep(seconds);
    }
    else{
        printf("Batch job: not enough args\n");
    }
    return 0;
}