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

//Size of process and function names
#define NAME_SIZE 50

//Function ADT (TAD)
typedef struct function{
    int FID;
    char name[NAME_SIZE];
    struct function *next;
}Function;

//Stack ADT (TAD)
typedef struct{
    Function *top;
}Stack;

//Process ADT (TAD)
typedef struct{
    int PID;
    char name[NAME_SIZE];
    char priority; //a b or c
    char state; // r e or d (ready, executing, done)
    int numStack;
    Stack *functionStack;
}Process;

//Node ADT (TAD)
typedef struct node{
    Process *process;
    struct node *next;
}Node;

//List ADT (TAD)
typedef struct{
    Node *start;
}List;

//Terminal setting
void setColor(char *color); //set terminal text color
void simpleRuler(); //print a 64 long ruler with '-'
void doubleRuler(); //print a 64 long ruler with '='

//Function Functions
Function* createFunction(int FID, char name[NAME_SIZE]); //create and malloc Function
void printFunction(Function *function); //print Function

//Process functions
Process* createProcess(int PID, char name[NAME_SIZE], char priority, int numStack); //create and malloc Process
int updateProcess(Process *process, int PID, char name[NAME_SIZE], char priority, char state, int numStack); //update Process struct
int popProcess(Process *process); //decrease in 1 from numStack
void printProcess(Process *process); //print Process
void freeProcess(Process *process); //free memory from Process

//List functions
List* createList(); //create and malloc List
void freeList(List *list); //free memory from List
int sizeList(List *list); //get List size
int insertAtList(List *list, Process *process, int pos); //insert at List by position
int pushList(List *list, Process *process); //insert at List start
int appendList(List *list, Process *process); //insert at List end
Process* removeFromListByPos(List *list, int pos); // remove from List by position
Process* removeFromList(List *list, int PID); // remove from List by PID
Process* popList(List *list); //remove from List end
Process* removeHeadList(List *list); //remove from List start
Process* getProcessList(List *list, int pos); //get Process from List by position
Process* getProcessListPID(List *list, int PID); //get Process from List by PID
int insertPriorityQueue(List *list, Process *process); //insert at List considering Process priority
int isEmptyList(List *list); //verify if List is empty
void printList(List *list); //print List

//Stack functions
Stack* createStack(); //create and malloc Stack
void freeStack(Stack *stack); //free memory from Stack
void pushStack(Stack *stack, Function *function); //insert at Stack top
Function* popStack(Stack *stack); //remove from Stack top
int sizeStack(Stack *stack); //get Stack size
int isEmptyStack(Stack *stack); //verify if Stack is empty
void printStack(Stack *stack); //print Stack

#endif