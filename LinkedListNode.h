//TODO make this less generic and adapt to actual CSUBatch jobs
//maybe introduce Job struct and have LLNode contain job pointers

typedef struct LLNode {
    void* thisNode;
    void* nextNode;
} LLNode;