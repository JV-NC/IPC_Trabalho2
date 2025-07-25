#include "utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Terminal setting
void setColor(char *color){
    printf("%s",color);
}
void simpleRuler(){
    printf("----------------------------------------------------------------\n");
}
void doubleRuler(){
    printf("================================================================\n");
}

//Function Functions
Function* createFunction(int FID, char name[NAME_SIZE]){
    if(FID<=0 || strlen(name)<=0){
        return NULL;
    }
    Function *function = (Function*)malloc(sizeof(Function));
    strcpy(function->name,name);
    function->FID = FID;
    function->next = NULL;
    return function;
}
void printFunction(Function *function){
    if(function==NULL){
        setColor(RED);
        printf("Funcao nula!\n");
        setColor(WHITE);
    }else{
        printf("FID: %d, Funcao: %s;\n",function->FID,function->name);
    }
}

//Process functions
Process* createProcess(int PID, char name[NAME_SIZE], char priority, int numStack){
    if(PID<=0 || strlen(name)<=0 || (priority<97 || priority>99) || numStack<=0){
        return NULL;
    }
    Process *process = (Process*)malloc(sizeof(Process));
    process->PID = PID;
    strcpy(process->name, name);
    process->priority = priority;
    process->state = 'r';
    process->priority = priority;
    process->numStack = numStack;
    process->functionStack = createStack();

    return process;
}
int updateProcess(Process *process, int PID, char name[NAME_SIZE], char priority, char state, int numStack){
    if(PID<=0 || strlen(name)<=0 || numStack<=0 || (priority<97 || priority>99) || (state=='r' || state=='e' || state=='d')){
        return 0;
    }
    process->PID = PID;
    strcpy(process->name, name);
    process->priority = priority;
    process->state = state;
    process->priority = priority;
    process->numStack = numStack;

    return 1;
}
int popProcess(Process *process){
    if(process->numStack<=0){
        return 0;
    }
    process->numStack--;
    return 1;
}
void printProcess(Process *process){
    if(process==NULL){
        setColor(RED);
        printf("Processo nulo!\n");
        setColor(WHITE);
    }else{
        printf("PID: %d , Nome: %s , Prioridade: ",process->PID, process->name);
        switch(process->priority){
            case 'a':
                printf("alta");
            break;
            case 'b':
                printf("media");
            break;
            default:
                printf("baixa");
            break;
        }
        printf(", Estado: ");
        switch(process->state){
            case 'r':
                printf("pronto");
            break;
            case 'e':
                printf("executando");
            break;
            default:
                printf("feito");
            break;
        }
        printf(", Numero de pilha: %d;\n",process->numStack);
        if(isEmptyStack(process->functionStack)){
            printf("\t");
        }
        printStack(process->functionStack);
    }
}
void freeProcess(Process *process){
    if(!isEmptyStack(process->functionStack)){
        freeStack(process->functionStack);
    }
    free(process);
}

//List functions
List* createList(){
    List *list = (List*)malloc(sizeof(list));
    list->start = NULL;
    return list;
}
void freeList(List *list){
    Process *aux;
    while(!isEmptyList(list)){
        aux = removeHeadList(list);
        if(aux!=NULL){
            freeProcess(aux);
            aux=NULL;
        }
    }
    free(list);
}
int sizeList(List *list){
    if(isEmptyList(list)){
        return 0;
    }
    int count=0;
    Node *aux=list->start;
    while(aux!=NULL){
        count++;
        aux=aux->next;
    }
    return count;
}
int insertAtList(List *list, Process *process, int pos){
    if(pos<0 || pos>sizeList(list) || process==NULL){
        return 0;
    }
    Node *n, *aux, *b4;
    n = (Node*)malloc(sizeof(Node));
    n->process = process;
    n->next = NULL;
    aux = list->start;
    if(pos==0){
        list->start = n;
        n->next = aux;
        return 1;
    }
    for(int i=0;i<pos;i++){
        b4 = aux;
        aux = aux->next;
    }
    b4->next = n;
    n->next = aux;

    return 1;
}
int pushList(List *list, Process *process){
    if(process==NULL){
        return 0;
    }
    Node *n = (Node*)malloc(sizeof(Node));
    n->process = process;
    n->next = list->start;
    list->start = n;
    return 1;
}
int appendList(List *list, Process *process){
    if(process==NULL){
        return 0;
    }
    Node *n = (Node*)malloc(sizeof(Node));
    n->process = process;
    n->next = NULL;
    if(isEmptyList(list)){
        list->start = n;
        return 1;
    }
    Node *aux = list->start;
    while(aux->next!=NULL){
        aux = aux->next;
    }
    aux->next = n;
    return 1;
}
Process* removeFromListByPos(List *list, int pos){
    if(isEmptyList(list) || pos<0 || pos>sizeList(list)){
        return NULL;
    }
    Node *aux, *b4;
    Process *p;
    if(pos==0){
        aux = list->start;
        b4 = list->start->next;
        list->start = b4;
        p = aux->process;
        free(aux);
        return p;
    }
    aux = list->start;
    for(int i=0;i<pos;i++){
        b4 = aux;
        aux = aux->next;
    }
    b4->next = aux->next;
    p = aux->process;
    free(aux);
    return p;
}
Process* removeFromList(List *list, int PID){
    if(isEmptyList(list) || PID<=0){
        return NULL;
    }
    Node *aux, *b4;
    Process *p;
    aux = list->start;
    b4 = NULL;
    while(aux!=NULL){
        if(aux->process->PID == PID){
            if(b4==NULL){
                list->start = aux->next;
            }else{
                b4->next=aux->next;
            }
            p = aux->process;
            free(aux);
            return p;
        }
        b4 = aux;
        aux = aux->next;
    }
    return NULL;
}
Process* popList(List *list){
    if(isEmptyList(list)){
        return NULL;
    }
    Node *aux = list->start;
    while(aux->next!=NULL){
        aux=aux->next;
    }
    Process *p = aux->process;
    free(aux);
    return p;
}
Process* removeHeadList(List *list){
    if(isEmptyList(list)){
        return NULL;
    }
    Node *aux = list->start;
    Process *p;
    list->start = aux->next;
    p = aux->process;
    free(aux);
    return p;
}
Process* getProcessList(List *list, int pos){
    if(isEmptyList(list) || pos<0){
        return NULL;
    }

    Node *aux=list->start;
    for(int i=0;i<pos;i++){
        if(aux==NULL){
            return NULL;
        }
    }
    return aux->process;
}
Process* getProcessListPID(List *list, int PID){
    if(isEmptyList(list) || PID<=0){
        return NULL;
    }

    Node *aux=list->start;
    while(aux!=NULL){
        if(aux->process->PID==PID){
            return aux->process;
        }
        aux = aux->next;
    }
    return NULL;
}
int insertPriorityQueue(List *list, Process *process){
    if(process==NULL){
        return 0;
    }
    if(isEmptyList(list)){
        return pushList(list,process);
    }
    if(process->priority=='c'){
        if(appendList(list,process)){
            return 1;
        }
    }
    Node *n = (Node*)malloc(sizeof(Node));
    n->process = process;
    Node *aux=list->start;
    Node *b4 = NULL;
    while(aux!=NULL){
        if(aux->process->priority > n->process->priority){
            if(b4==NULL){
                list->start = n;
            }else{
                b4->next = n;
            }
            n->next = aux;
            return 1;
        }
        b4 = aux;
        aux = aux->next;
    }
    b4->next = n;
    n->next = NULL;
    return 1;
}
int isEmptyList(List *list){
    return (list->start==NULL) ? 1 : 0;
}
void printList(List *list){
    if(list == NULL){
        printf("Lista nao foi instanciada!\n");
    }else if(isEmptyList(list)){
        printf("Lista vazia!\n");
    }else{
        Node *aux = list->start;
        int i = 1;
        while(aux!=NULL){
            printf("Posicao %d: ",i);
            printProcess(aux->process);
            aux = aux->next;
            i++;
        }
    }
}

//Stack functions
Stack* createStack(){
    Stack *stack = (Stack*)malloc(sizeof(Stack));
    stack->top = NULL;
    return stack;
}
void freeStack(Stack *stack){
    Function *function;
    while(!isEmptyStack(stack)){
        function = popStack(stack);
        if(function!=NULL){
            free(function);
        }
    }
    free(stack);
}
void pushStack(Stack *stack, Function *function){
    function->next = stack->top;
    stack->top = function;
}
Function* popStack(Stack *stack){
    if(isEmptyStack(stack)){
        return NULL;
    }
    Function *aux=stack->top;
    stack->top = aux->next;
    aux->next = NULL;
    return aux;
}
int sizeStack(Stack *stack){
    if(isEmptyStack(stack)){
        return 0;
    }
    int count=0;
    Function *aux = stack->top;
    while(aux!=NULL){
        aux = aux->next;
        count++;
    }
    return count;
}
int isEmptyStack(Stack *stack){
    return (stack->top==NULL) ? 1 : 0;
}
void printStack(Stack *stack){
    if(stack == NULL){
        printf("Pilha nao foi instanciada!\n");
    }else if(isEmptyStack(stack)){
        printf("Pilha vazia!\n");
    }else{
        Function *aux = stack->top;
        int i = 1;
        while(aux!=NULL){
            printf("\t");
            printFunction(aux);
            aux = aux->next;
            i++;
        }
    }
}