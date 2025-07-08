#include "utility.h"
#include <stdio.h>
#include <stdlib.h>

List* createList(){
    List *list = (List*)malloc(sizeof(list));
    list->start = NULL;
    return list;
}

void freeList(List *list){
    while(!isEmptyList(list)){
        popList(list);
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
    if(pos<0 || pos>sizeList(list)){
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
    }
    for(int i=0;i<pos;i++){
        b4 = aux;
        aux = aux->next;
    }
    b4->next = n;
    n->next = aux;

    return 1;
}

void appendList(List *list, Process *process){
    Node *n = (Node*)malloc(sizeof(Node));
    n->process = process;
    n->next = list->start;
    list->start = n;
}

Process* removeAtList(List *list, int pos){
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

Process* getItemList(List *list, int pos){
    if(isEmptyList(list)){
        return 0;
    }

    Node *aux=list->start;
    for(int i=0;i<pos;i++){
        if(aux==NULL){
            return NULL;
        }
    }
    return aux->process;
}

int isEmptyList(List *list){
    return (list->start==NULL) ? 1 : 0;
}

//Stack functions
Stack* createStack(){
    Stack *stack = (Stack*)malloc(sizeof(Stack));
    stack->top = NULL;
    return stack;
}

void freeStack(Stack *stack){
    while(!isEmptyStack(stack)){
        popStack(stack);
    }
    free(stack);
}

void pushStack(Stack *stack, Process *process){
    Node *n=(Node*)malloc(sizeof(Node));
    n->process = process;
    n->next=stack->top;
    stack->top = n;
}

Process* popStack(Stack *stack){
    if(isEmptyStack(stack)){
        return NULL;
    }
    Node *aux=stack->top;
    stack->top = aux->next;
    Process *p=aux->process;
    free(aux);
    return p;
}

int isEmptyStack(Stack *stack){
    return (stack->top==NULL) ? 1 : 0;
}