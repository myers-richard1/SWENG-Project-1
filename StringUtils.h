#pragma once

typedef struct SplitString{
    int number_of_elements;
    char* elements[10];
} SplitString;

SplitString split(char* source, char* delimiter);

