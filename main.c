#include "utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Function* setFunction(int controllerFID, int functionCounter); //get Function atributes from user
int setProcessQueue(List *priorityQueue, int controllerPID, int *controllerFID); //get Process atributes from user and insert in Queue
Process* executeProcess(List *priorityQueue); //execute first Process in priorityQueue and insert at executedList
void printSystem(List *priorityQueue, List *executedList); //print priorityQueue and ExecutedList
int revokeProcess(List *priorityQueue); //revoke Process and remove from priorityList
void clearBuffer(); //clear buffer depending on the OS
void menu(); //main menu used by user

int main(){
    menu();

    return 0;
}

Function* setFunction(int controllerFID, int functionCounter){
    char name[NAME_SIZE];
    setColor(CYAN);
    do{
        printf("%d: Nome da funcao: ",functionCounter+1);
        setColor(WHITE);
        fgets(name,NAME_SIZE,stdin);
        fflush(stdin);
        if(name[0]=='\n' || name[0]==' ' || name[0]=='\t' || name[0]=='\0'){
            setColor(RED);
            printf("Nome invalido!\n");
            setColor(CYAN);
        }
    }while(name[0]=='\n' || name[0]==' ' || name[0]=='\t' || name[0]=='\0'); //get function name
    name[strlen(name)-1] = '\0';

    return createFunction(controllerFID,name); //create and return Function
}

int setProcessQueue(List *priorityQueue, int controllerPID, int *controllerFID){
    char priority,name[NAME_SIZE];
    int numStack,i=0;
    Process *process;
    Function *aux=NULL;
    Stack *auxStack = createStack();

    setColor(CYAN);
    do{
        printf("Nome do processo: ");
        setColor(WHITE);
        fgets(name,NAME_SIZE,stdin);
        fflush(stdin);
        if(name[0]=='\n' || name[0]==' ' || name[0]=='\t' || name[0]=='\0'){
            setColor(RED);
            printf("Nome invalido!\n");
            setColor(CYAN);
        }
    }while(name[0]=='\n' || name[0]==' ' || name[0]=='\t' || name[0]=='\0'); //get Process name
    
    name[strlen(name)-1] = '\0';

    do{
        setColor(CYAN);
        printf("Prioridade ('a','b','c'): ");
        setColor(WHITE);
        scanf("%c",&priority);
        clearBuffer();

        if(priority>=65 && priority<=67){ //TODO: priority error with getchar()
            priority+=32;//uppercase to lowercase
        }

        if(priority<97 || priority>99){
            setColor(RED);
            printf("Opcao invalida!\n");
            setColor(WHITE);
        }
    }while(priority<97 || priority>99); //get Process priority
    
    do{
        setColor(CYAN);
        printf("Numero de chamadas de funcao: ");
        setColor(WHITE);
        scanf("%d",&numStack);
        clearBuffer();

        if(numStack<=0){
            setColor(RED);
            printf("Valor invalido!\n");
            setColor(WHITE);
        }
    }while(numStack<=0); // get Process numStack

    process = createProcess(controllerPID,name,priority,numStack); //create Process
    do{
        setColor(CYAN);
        aux = setFunction(*controllerFID,i); // set Function for every numStack
        if(aux==NULL){
            setColor(RED);
            printf("Falha ao criar funcao. Tente novamente!\n");
            setColor(WHITE);
        }else{
            i++;
            pushStack(auxStack,aux); //push Function in auxStack
            aux=NULL;
            (*controllerFID)++;
        }
    }while(i<numStack);
    setColor(WHITE);
    while(!isEmptyStack(auxStack)){
        pushStack(process->functionStack,popStack(auxStack)); //reStack in functionStack, for inverse order
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
    Process *process = removeHeadList(priorityQueue); //get first Process
    process->state = 'e';
    setColor(CYAN);simpleRuler();setColor(YELLOW);
    printf("Executando processo %d: %s;\n",process->PID,process->name);
    setColor(WHITE);
    Function *aux = NULL;

    for(int i = 0;i<process->numStack;i++){ //get every Process Function, pop it and free it
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
    printf("<Fila de Prioridade>\n"); //priorityQueue
    if(!isEmptyList(priorityQueue)){
        printList(priorityQueue);
    }
    setColor(CYAN);simpleRuler();setColor(GREEN);
    printf("<Lista de Finalizados>\n"); //executedList
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
    printList(priorityQueue); //print Queue
    do{
        setColor(CYAN);simpleRuler();
        printf("Selecione o processo (PID): ");
        setColor(WHITE);
        scanf("%d",&PID);
        clearBuffer();
        aux = removeFromList(priorityQueue,PID);
        if(aux==NULL){
            setColor(CYAN);simpleRuler();setColor(RED);
            printf("PID invalido!\n");
            setColor(WHITE);
        }
    }while(aux==NULL); //get valid Process by PID and free it
    freeProcess(aux);
    return 1;
}

void clearBuffer(){
    #if defined(_WIN32) || defined(_WIN64)
        fflush(stdin);
    #elif defined(__linux__) || defined(__unix__)
        char ch;
        while ((ch = getchar()) != '\n' && ch != EOF);
    #endif
}

void menu(){
    int op=0;
    List *priorityQueue = createList();
    List *executedList = createList();
    Process *auxProcess;
    int controllerPID = 1;
    int controllerFID = 1;
    do{
        setColor(CYAN);
        doubleRuler();
        printf("\t\tSistema Operacional Simplificado\n");
        doubleRuler();
        printf("\t1 - Criar processo;\n\t2 - Executar processo;\n\t3 - Imprimir sistema;\n\t4 - Cancelar processo;\n\t5 - Sair;\n");
        simpleRuler();setColor(WHITE);
        scanf("%d",&op);
        clearBuffer();

        switch(op){
            case 1: //create Process
                setColor(CYAN);simpleRuler();setColor(WHITE);
                if(setProcessQueue(priorityQueue,controllerPID,&controllerFID)){
                    setColor(GREEN);
                    printf("Processo criado com sucesso!\n");
                    setColor(WHITE);
                    controllerPID++;
                }else{
                    setColor(RED);
                    printf("Falha ao criar processo!\n");
                    setColor(WHITE);
                }
                setColor(CYAN);simpleRuler();setColor(WHITE);
            break;
            case 2: //execute Process in priorityQueue
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
            case 3: //printSystem
                printSystem(priorityQueue,executedList);
            break;
            case 4: //revokeProcess
                if(isEmptyList(priorityQueue)){
                    setColor(CYAN);simpleRuler();setColor(RED);
                    printf("Fila de prioridade vazia!\n");
                    setColor(CYAN);simpleRuler();setColor(WHITE);
                }
                if(revokeProcess(priorityQueue)){
                    setColor(CYAN);simpleRuler();setColor(GREEN);
                    printf("Processo Cancelado!\n");
                }
                setColor(CYAN);simpleRuler();setColor(WHITE);
            break;
            case 5: //exit program
                setColor(CYAN);simpleRuler();setColor(YELLOW);
                printf("Saindo...\n");
                setColor(CYAN);simpleRuler();
            break;
            default: //invalid option
                setColor(CYAN);simpleRuler();setColor(RED);
                printf("Opcao invalida!\n");
                setColor(CYAN);simpleRuler();
            break;
        }
    }while(op!=5);
    setColor(WHITE);
    freeList(priorityQueue); //free Lists
    freeList(executedList);
}