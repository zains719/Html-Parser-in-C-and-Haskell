int STACK_START_SIZE;

#include <stdio.h>
#include <stdlib.h>

struct Stack{
    int top;
    unsigned size;
    int *array;
};

struct Stack* createStack();
int isFull(struct Stack* stack);
int isEmpty(struct Stack* stack);
int * reSizeStack(struct Stack* stack, int * stackArray);
void push(struct Stack* stack, int num);
int pop(struct Stack* stack);
int lastValue(struct Stack* stack);
void freeStack(struct Stack* stack, int * stackArray);