#ifndef UTILITY_H
#define UTILITY_H

#define NAME_SIZE 50

typedef struct{
    int PID;
    char name[NAME_SIZE];
    char priority; //a b or c
    char state; // r e or f
    int numStack;
}Process;

typedef struct node{
    Process *process;
    struct node *next;
}Node;

typedef struct{
    Node *start;
}List;

typedef struct{
    Node *top;
}Stack;

Process* createProcess(int PID, char name[NAME_SIZE],char priority, int numStack);


List* createList();
void freeList(List *list);
int sizeList(List *list);
int insertAtList(List *list, Process *process, int pos);
void appendList(List *list, Process *process);
Process* removeAtList(List *list, int pos);
Process* popList(List *list);
Process* getItemList(List *list, int pos);
int isEmptyList(List *list);

Stack* createStack();
void freeStack(Stack *stack);
void pushStack(Stack *stack, Process *process);
Process* popStack(Stack *stack);
int isEmptyStack(Stack *stack);

#endif