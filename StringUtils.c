#include "StringUtils.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

SplitString split(char* source, char* delimiter){
    SplitString splitString;
    printf("Getting first token from string %s\n", source);
    char* token = strtok(source, delimiter);
    int index = 0;
    while (token != NULL){
        printf("Setting element at index %d to be %s\n", index, token);
        splitString.elements[index] = token;
        index++;
        printf("Getting next token\n");
        token = strtok(NULL, delimiter);        
    }
    printf("Setting number of elements to %d\n", index);
    splitString.number_of_elements = index;
    printf("Returning split string\n");
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