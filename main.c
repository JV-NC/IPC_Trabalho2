#include "utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Function* setFunction(int controllerPID);
int setProcessQueue(List *priorityQueue, int controllerPID);
Process* executeProcess(List *priorityQueue);
void printSystem(List *priorityQueue, List *executedList);
int revokeProcess(List *priorityQueue);
void menu();

int main(){
    menu();

    return 0;
}

Function* setFunction(int controllerPID){
    char name[NAME_SIZE];
    setColor(CYAN);
    printf("Nome da funcao: ");
    setColor(WHITE);
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

    setColor(CYAN);
    printf("Nome do processo: ");
    setColor(WHITE);
    fgets(name,NAME_SIZE,stdin);
    fflush(stdin);
    name[strlen(name)-1] = '\0';

    do{
        setColor(CYAN);
        printf("Prioridade ('a','b','c'): ");
        setColor(WHITE);
        scanf("%c",&priority);
        fflush(stdin);

        if(priority>=65 && priority<=67){
            priority+=32;//maisculo para minusculo
        }

        if(priority<97 || priority>99){
            setColor(RED);
            printf("Opcao invalida!\n");
            setColor(WHITE);
        }
    }while(priority<97 || priority>99);
    
    do{
        setColor(CYAN);
        printf("Numero de chamadas de funcao: ");
        setColor(WHITE);
        scanf("%d",&numStack);
        fflush(stdin);

        if(numStack<=0){
            setColor(RED);
            printf("Valor invalido!\n");
            setColor(WHITE);
        }
    }while(numStack<=0);

    process = createProcess(controllerPID,name,priority,numStack);
    do{
        setColor(CYAN);
        printf("%d: ",i+1);
        aux = setFunction(controllerPID);
        if(aux==NULL){
            setColor(RED);
            printf("Falha ao criar funcao. Tente novamente!");
            setColor(WHITE);
        }else{
            i++;
            pushStack(auxStack,aux);
            aux=NULL;
        }
    }while(i<numStack);
    setColor(WHITE);
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
    if(priorityQueue==NULL || isEmptyList(priorityQueue)){
        return NULL;
    }
    Process *process = removeHeadList(priorityQueue);
    process->state = 'e';
    setColor(CYAN);simpleRuler();setColor(YELLOW);
    printf("Executando processo %d: %s;\n",process->PID,process->name);
    setColor(WHITE);
    Function *aux = NULL;

    for(int i = 0;i<process->numStack;i++){
        aux = popStack(process->functionStack);
        if(aux == NULL){
            setColor(RED);
            printf("Erro na funcao %d;\n",i);
        }else{
            setColor(YELLOW);
            printf("\t");
            printFunction(aux);
            free(aux);
        }
    }
    setColor(WHITE);
    process->state = 'd';
    return process;
}

void printSystem(List *priorityQueue, List *executedList){
    setColor(CYAN);simpleRuler();setColor(YELLOW);
    printf("<Fila de Prioridade>\n");
    if(!isEmptyList(priorityQueue)){
        printList(priorityQueue);
    }
    setColor(CYAN);simpleRuler();setColor(GREEN);
    printf("<Lista de Finalizados>\n");
    if(!isEmptyList(executedList)){
        printList(executedList);
    }
    setColor(CYAN);simpleRuler();
}

int revokeProcess(List *priorityQueue){
    if(isEmptyList(priorityQueue)){
        return 0;
    }
    Process *aux;
    int PID;
    setColor(CYAN);simpleRuler();
    printList(priorityQueue);
    do{
        setColor(CYAN);simpleRuler();
        printf("Selecione o processo (PID): ");
        setColor(WHITE);
        scanf("%d",&PID);
        fflush(stdin);
        aux = removeFromList(priorityQueue,PID);
        if(aux==NULL){
            setColor(CYAN);simpleRuler();setColor(RED);
            printf("PID invalido!\n");
            setColor(WHITE);
        }
    }while(aux==NULL);
    freeProcess(aux);
    return 1;
}
//TODO: getchar em vez de fflush? e printar processos melhor
void menu(){
    int op=0,auxInt;
    List *priorityQueue = createList();
    List *executedList = createList();
    Process *auxProcess;
    int controllerPID = 1;
    do{
        setColor(CYAN);
        doubleRuler();
        printf("\t\tSistema Operacional Simplificado\n");
        doubleRuler();
        printf("\t1 - Criar processo;\n\t2 - Executar processo;\n\t3 - Imprimir sistema;\n\t4 - Cancelar processo;\n\t5 - Sair;\n");
        simpleRuler();setColor(WHITE);
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
                        setColor(GREEN);
                        printf("Processo executado com sucesso!\n");
                        setColor(WHITE);
                    }else{
                        setColor(RED);
                        printf("Falha em armazenar processo na lista de executados!\n");
                        setColor(WHITE);
                    }
                    setColor(CYAN);simpleRuler();
                    auxProcess = NULL;
                }else{
                    setColor(CYAN);simpleRuler();setColor(RED);
                    printf("Fila de prioridade vazia!\n");
                    setColor(CYAN);simpleRuler();setColor(WHITE);
                }
            break;
            case 3:
                printSystem(priorityQueue,executedList);
            break;
            case 4:
                if(isEmptyList(priorityQueue)){
                    setColor(CYAN);simpleRuler();setColor(RED);
                    printf("Fila de prioridade vazia!");
                    setColor(CYAN);simpleRuler();setColor(WHITE);
                }
                if(revokeProcess(priorityQueue)){
                    setColor(CYAN);simpleRuler();setColor(GREEN);
                    printf("Processo Cancelado!\n");
                }
                setColor(CYAN);simpleRuler();setColor(WHITE);
            break;
            case 5:
                setColor(CYAN);simpleRuler();setColor(YELLOW);
                printf("Saindo...\n");
                setColor(CYAN);simpleRuler();
            break;
            default:
                setColor(CYAN);simpleRuler();setColor(RED);
                printf("Opcao invalida!\n");
                setColor(CYAN);simpleRuler();
            break;
        }
    }while(op!=5);
    setColor(WHITE);
    freeList(priorityQueue);
    freeList(executedList);
}