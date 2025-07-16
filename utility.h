#ifndef UTILITY_H
#define UTILITY_H

// Text colors
#define WHITE       "\033[0m"
#define BLACK       "\033[30m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
//#define WHITE       "\033[37m"
#define BRIGHT_BLACK   "\033[90m"
#define BRIGHT_RED     "\033[91m"
#define BRIGHT_GREEN   "\033[92m"
#define BRIGHT_YELLOW  "\033[93m"
#define BRIGHT_BLUE    "\033[94m"
#define BRIGHT_MAGENTA "\033[95m"
#define BRIGHT_CYAN    "\033[96m"
#define BRIGHT_WHITE   "\033[97m"
//Size of process and function names
#define NAME_SIZE 50

typedef struct function{
    int PID;
    char name[NAME_SIZE];
    struct function *next;
}Function;

typedef struct{
    Function *top;
}Stack;

typedef struct{
    int PID;
    char name[NAME_SIZE];
    char priority; //a b or c
    char state; // r e or d (ready, executing, done)
    int numStack;
    Stack *functionStack;
}Process;

typedef struct node{
    Process *process;
    struct node *next;
}Node;

typedef struct{
    Node *start;
}List;

//Terminal setting
void setColor(char *color);
void simpleRuler();
void doubleRuler();

//Function Functions
Function* createFunction(int PID, char name[NAME_SIZE]);
void printFunction(Function *function);

//Process functions
Process* createProcess(int PID, char name[NAME_SIZE], char priority, int numStack);
int updateProcess(Process *process, int PID, char name[NAME_SIZE], char priority, char state, int numStack);
int popProcess(Process *process);
void printProcess(Process *process);
void freeProcess(Process *process);

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
Process* removeHeadList(List *list);
Process* getProcessList(List *list, int pos);
Process* getProcessListPID(List *list, int PID);
int insertPriorityQueue(List *list, Process *process);
int isEmptyList(List *list);
void printList(List *list);

//Stack functions
Stack* createStack();
void freeStack(Stack *stack);
void pushStack(Stack *stack, Function *function);
Function* popStack(Stack *stack);
int sizeStack(Stack *stack);
int isEmptyStack(Stack *stack);
void printStack(Stack *stack);

#endif