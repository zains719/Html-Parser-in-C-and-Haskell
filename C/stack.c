#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"stack.h"

int STACK_START_SIZE = 500;

struct Stack* createStack() {
    struct Stack* stackPtr = (struct Stack*)malloc(sizeof(struct Stack));
    stackPtr->size = STACK_START_SIZE;
    stackPtr->top = -1;
    stackPtr->array = malloc(stackPtr->size * sizeof(int));
    return stackPtr;
}

int isFull(struct Stack* stack) {
    return (stack->top == stack->size - 1);
}

int isEmpty(struct Stack* stack) {
     return (stack->top == -1);
}

int * reSizeStack(struct Stack* stack, int * stackArray) {
    stack->size *= 2;
    stackArray = realloc(stackArray, stack->size * sizeof(int));
    return stackArray;
}

void push(struct Stack* stack, int num) {
    if (isFull(stack)){
        stack->array = reSizeStack(stack, stack->array);
    }
    stack->top++;
    stack->array[stack->top] = num;
}

int pop(struct Stack* stack) {
    if (isEmpty(stack)){
        return -1;
    }
    int popValue = stack->array[stack->top];
    stack->top --;
    return popValue;
}

int lastValue(struct Stack* stack) {
    if (isEmpty(stack)){
        return -1;
    }
    return stack->array[stack->top];
}

void freeStack(struct Stack* stack, int * stackArray) {
    free(stackArray);
    free(stack);
}