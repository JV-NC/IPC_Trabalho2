#include "utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Function* setFunction(int controllerPID);
int setProcessQueue(List *priorityQueue, int controllerPID);
Process* executeProcess(List *priorityQueue);
void printSystem(List *priorityQueue, List *executedStack);
int revokeProcess(List *priorityQueue);
void menu();

int main(){
    menu();

    return 0;
}

Function* setFunction(int controllerPID){
    char name[NAME_SIZE];
    printf("Nome da funcao: ");
    fgets(name,NAME_SIZE,stdin);
    fflush(stdin);
    name[strlen(name)-1] = '\0';

    return createFunction(controllerPID,name);
}

int setProcessQueue(List *priorityQueue, int controllerPID){
    char priority,name[NAME_SIZE];
    int numStack,i=0;
    Process *process;
    Function *aux=NULL;
    Stack *auxStack = createStack();

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
        printf("Numero de chamadas de funcao: ");
        scanf("%d",&numStack);
        fflush(stdin);

        if(numStack<=0){
            printf("Valor invalido!\n");
        }
    }while(numStack<=0);

    process = createProcess(controllerPID,name,priority,numStack);
    //TODO:fazer criarFunções invertido;
    do{
        printf("%d: ",i+1);
        aux = setFunction(controllerPID);
        if(aux==NULL){
            printf("Falha ao criar funcao. Tente novamente!");
        }else{
            i++;
            pushStack(auxStack,aux);
            aux=NULL;
        }
    }while(i<numStack);
    while(!isEmptyStack(auxStack)){
        pushStack(process->functionStack,popStack(auxStack));
    }
    freeStack(auxStack);
    if(insertPriorityQueue(priorityQueue,process)){
        return 1;
    }else{
        return 0;
    }
}

Process* executeProcess(List *priorityQueue){
    if(priorityQueue==NULL){
        return NULL;
    }
    Process *process = removeHeadList(priorityQueue);
    process->state = 'e';
    printf("Executando processo %d: %s;\n",process->PID,process->name);
    Function *aux = NULL;

    for(int i = 0;i<process->numStack;i++){
        aux = popStack(process->functionStack);
        if(aux == NULL){
            printf("Erro na funcao %d;\n",i);
        }else{
            printFunction(aux);
            free(aux);
        }
    }
    process->state = 'd';
    return process;
}

void printSystem(List *priorityQueue, List *executedStack){
    printf("----------------------\n  Fila de Prioridade\n----------------------\n");
    printList(priorityQueue);
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
    freeProcess(aux);
    return 1;
}

void menu(){
    int op=0,auxInt;
    List *priorityQueue = createList();
    List *executedList = createList();
    Process *auxProcess; //TODO: verificar utilidade de aux e verificar se estruturas foram alocadas
    int controllerPID = 1;
    do{
        printf("----------------------------------\n Sistema Operacional Simplificado\n----------------------------------\n1 - Criar processo;\n2 - Executar processo;\n3 - Imprimir sistema;\n4 - Cancelar processo;\n5 - Sair;\n----------------------------------\n\n");
        scanf("%d",&op);
        fflush(stdin);

        switch(op){
            case 1:
                if(setProcessQueue(priorityQueue,controllerPID)){
                    controllerPID++;
                }else{
                    printf("Falha ao criar processo!\n");
                }
            break;
            case 2:
                auxProcess = executeProcess(priorityQueue);
                if(auxProcess!=NULL){
                    if(pushList(executedList,auxProcess)){
                        printf("Processo executado com sucesso!\n");
                    }else{
                        printf("Falha em armazenar processo na lista de executados!\n");
                    }
                    auxProcess = NULL;
                }else{
                    printf("Falha ao executar o processo!\n");
                }
            break;
            case 3:
                printSystem(priorityQueue,executedList);
            break;
            case 4:
                revokeProcess(priorityQueue);
            break;
            case 5:
                printf("Saindo...");
            break;
            default:
                printf("Opcao invalida!\n");
            break;
        }
    }while(op!=5);
    freeList(priorityQueue);
    freeList(executedList);
}