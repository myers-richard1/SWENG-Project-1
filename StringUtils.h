#pragma once

typedef struct SplitString{
    int number_of_elements;
    char* elements[5];
} SplitString;

SplitString split(char* source, char* delimiter);

