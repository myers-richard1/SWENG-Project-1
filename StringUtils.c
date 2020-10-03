#include "StringUtils.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

SplitString split(char* source, char* delimiter){
    SplitString splitString;
    char* token = strtok(source, delimiter);
    int index = 0;
    while (token != NULL){
        splitString.elements[index] = token;
        index++;
        token = strtok(NULL, delimiter);        
    }
    splitString.number_of_elements = index;
    return splitString;
}

int test(){
    printf("Split string test\n");
    char string[50] = "Split string test";
    SplitString splitString = split(string, " ");
    printf("Received %d elements\n", splitString.number_of_elements);
    printf("Printing elements\n");
    for (int i = 0; i < splitString.number_of_elements; i++){
        printf("Element %d: %s\n", i, splitString.elements[i]);
    }
    return 0;
}