#include "utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int setProcessQueue(List *priorityQueue, int controllerPID);
int stackProcess(Stack *functionStack, Process *process);
Process* executeProcess(Stack *functionStack);
void printSystem(List *priorityQueue, Stack *functionStack, List *executedStack);
int revokeProcess(List *priorityQueue);
void menu();

int main(){
    menu();

    return 0;
}

int setProcessQueue(List *priorityQueue, int controllerPID){
    char priority,name[NAME_SIZE];
    int numStack;
    Process *process;

    printf("Nome do processo: "); //TODO: tratar nome e verificar se nome já existe?
    fgets(name,NAME_SIZE,stdin);
    fflush(stdin);
    name[strlen(name)-1] = '\0';

    do{
        printf("Prioridade ('a','b','c'): ");
        scanf("%c",&priority);
        fflush(stdin);

        if(priority<97 || priority>99){
            printf("Opcao invalida!\n");
        }
    }while(priority<97 || priority>99);
    
    do{
        printf("Numero de chamada de funcao: ");
        scanf("%d",&numStack);
        fflush(stdin);

        if(numStack<=0){
            printf("Valor invalido!\n");
        }
    }while(numStack<=0);

    process = createProcess(controllerPID,name,priority,numStack);
    if(insertPriorityQueue(priorityQueue,process)){
        return 1;
    }else{
        return 0;
    }
}

int stackProcess(Stack *functionStack, Process *process){
    if(functionStack==NULL || process==NULL){
        return 0;
    }
    for(int i=0;i<process->numStack;i++){
        pushStack(functionStack,process);
    }
    process->state = 'e';
    return process->PID;
}

Process* executeProcess(Stack *functionStack){
    if(functionStack==NULL){
        return NULL;
    }
    Process *process;
    while(!isEmptyStack(functionStack)){
        process = popStack(functionStack);
        printf("Chamada de funcao %d;\n",process->numStack);
        process->numStack--;
    }
    return process;
}

void printSystem(List *priorityQueue, Stack *functionStack, List *executedStack){
    printf("----------------------\n  Fila de Prioridade\n----------------------\n");
    printList(priorityQueue);
    printf("----------------------\n   Pilha de Funcoes\n----------------------\n");
    printStack(functionStack);
    printf("----------------------\n Lista de Finalizados\n----------------------\n");
    printList(executedStack);
}

int revokeProcess(List *priorityQueue){
    if(isEmptyList(priorityQueue)){
        return 0;
    }
    Process *aux;
    int PID;
    printList(priorityQueue);
    do{
        printf("Selecione o processo (PID): ");
        scanf("%d",&PID);
        fflush(stdin);
        aux = removeFromList(priorityQueue,PID);
        if(aux==NULL){
            printf("PID invalido!\n");
        }
    }while(aux==NULL);
    free(aux);
    return 1;
}

void menu(){
    int op=0,auxInt;
    List *priorityQueue = createList();
    Stack *functionStack = createStack();
    List *executedList = createList();
    Process *auxProcess; //TODO: verificar utilidade de aux e verificar se estruturas foram alocadas
    int controllerPID = 1;
    do{
        printf("----------------------------------\n Sistema Operacional Simplificado\n----------------------------------\n1 - Criar processo;\n2 - Executar processo;\n3 - Imprimir sistema;\n4 - Cancelar processo;\n5 - Sair;\n----------------------------------\n\n");
        scanf("%d",&op);
        fflush(stdin);

        switch(op){
            case 1:
                setProcessQueue(priorityQueue,controllerPID);
                controllerPID++;
            break;
            case 2:
                if(isEmptyStack(functionStack) && isEmptyList(priorityQueue)){
                    printf("Nenhum processo na fila de prioridade!\n");
                    break;
                }
                if(isEmptyStack(functionStack)){
                    auxInt = stackProcess(functionStack,removeHeadList(priorityQueue));
                    if(auxInt!=0){
                        printf("Processo de PID %d na pilha de funcoes!\n",auxInt);
                    }else{
                        printf("Falha ao empilhar processo!\n");
                    }
                }
                else{
                    auxProcess = executeProcess(functionStack);
                    if(auxProcess!=NULL){
                        pushList(executedList,auxProcess);
                        auxProcess = NULL;
                    }
                }
            break;
            case 3:
                printSystem(priorityQueue,functionStack,executedList);
            break;
            case 4:
                revokeProcess(priorityQueue);
            break;
            case 5:
                //saindo...
            break;
            default:
                printf("Opcao invalida!\n");
            break;
        }
    }while(op!=5);
}