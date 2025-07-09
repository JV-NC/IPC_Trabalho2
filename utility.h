#ifndef UTILITY_H
#define UTILITY_H

#define NAME_SIZE 50

typedef struct{
    int PID;
    char name[NAME_SIZE];
    char priority; //a b or c
    char state; // r e or d (ready, executing, done)
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

//Process functions
Process* createProcess(int PID, char name[NAME_SIZE], char priority, int numStack);
int updateProcess(Process *process, int PID, char name[NAME_SIZE], char priority, char state, int numStack);
int popProcess(Process *process);
void printProcess(Process *process);

//List functions
List* createList();
void freeList(List *list);
int sizeList(List *list);
int insertAtList(List *list, Process *process, int pos);
int pushList(List *list, Process *process);
int appendList(List *list, Process *process);
Process* removeFromListByPos(List *list, int pos);
Process* removeFromList(List *list, int PID);
Process* popList(List *list);
Process* getProcessList(List *list, int pos);
Process* getProcessListPID(List *list, int PID);
int insertPriorityQueue(List *list, Process *process);
int isEmptyList(List *list);
void printList(List *list);

//Stack functions
Stack* createStack();
void freeStack(Stack *stack);
void pushStack(Stack *stack, Process *process);
Process* popStack(Stack *stack);
int sizeStack(Stack *stack);
int isEmptyStack(Stack *stack);
void printStack(Stack *stack);

#endif