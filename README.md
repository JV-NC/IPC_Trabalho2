# IPC_Trabalho2
Trabalho Prático 2 de Introdução a Programação de Computadores, sendo necessário desenvolver um gerenciador de tarefas de um Sistema Operacional Simplificado, podendo criar e executar processos a partir de estrutura de dados como Lista e Pilha.

### :clipboard: Exercício Proposto:
Desenvolver um programa em linguagem C que simule um sistema operacional simplificado, com suporte à
criação de processos, escalonamento por filas de prioridades, uso de pilhas para chamadas de funções e gerenciamento de processos finalizados por meio de uma lista encadeada.

### :wrench: Pré-requisitos
* [GCC](https://gcc.gnu.org/ "GCC (GNU Compiler Collection)") - Compilador para C

### :rocket: Execução
O algoritmo é executado usando arquivo Makefile, capaz de criar o executável ```main``` tanto Windows quanto Linux, compilando os arquivos ```main.c```,```utility.c``` e ```utility.h```.

### :books: Bibliotecas Utilizadas
* ```<stdio.h>``` - Biblioteca padrão de entrada e saída.
* ```<stdlib.h>``` - Biblioteca padrão para alocação de memória.
* ```<string.h>``` - Biblioteca padrão com funções em string.
* ```"utility.h``` - Biblioteca desenvolvida no projeto, contendo implementação de Lista e Pilha, bem como os TADs de Processo e Função, além de fazer manipulação de cores no terminal.

### :mag: Mais sobre `utility.h`

A biblioteca `utility.h` foi desenvolvida para fornecer estruturas de dados e funções de apoio ao gerenciamento de processos em um sistema operacional simplificado. A seguir, detalhamos os principais componentes:

---

#### :art: Definições de Cores no Terminal

São utilizados códigos ANSI para modificar a cor do texto exibido no terminal. Isso é útil para destacar mensagens, estados dos processos etc.

```c
#define RED   "\033[31m"
#define GREEN "\033[32m"
#define BLUE  "\033[34m"
// ...
```
Uso de exemplo:
```c
setColor(RED);
printf("Erro ao criar processo.\n");
setColor(WHITE);
```
#### :books: TAD Function e Stack (Pilha de Funções)
Cada processo possui uma pilha de funções (```Stack```) que simula chamadas internas de função.
```c
typedef struct function {
    int FID;
    char name[50];
    struct function *next;
} Function;
```
A pilha segue a lógica LIFO (Last In, First Out). As principais funções relacionadas são:
* ```pushStack(Stack *stack, Function *function)``` - Insere uma função no topo da pilha.
* ```popStack(Stack *stack)``` - Remove a função do topo.
* ```printStack(Stack *stack)``` - Imprime as funções contidas na pilha.

#### :mechanical_arm: TAD Process
A estrutura ```Process``` representa um processo do sistema operacional:
```c
typedef struct {
    int PID;
    char name[50];
    char priority; // 'a', 'b' ou 'c'
    char state;    // 'r' (ready), 'e' (executing), 'd' (done)
    Stack *functionStack;
} Process;
```
##### Funções principais:
* ```createProcess(...)``` - Cria e aloca memória para um novo processo.
* ```updateProcess(...)``` - Atualiza os dados de um processo existente.
* ```popProcess(...)``` - Retira uma função da pilha associada ao processo.
* ```printProcess(...)``` - Imprime as informações do processo.
* ```freeProcess(...)``` - Libera a memória alocada pelo processo.

#### :file_folder: TAD List (Lista Encadeada de Processos)
A estrutura ```List``` gerencia os processos por meio de uma lista encadeada, usada para escalonamento e manipulação da fila de execução.
```c
typedef struct node {
    Process *process;
    struct node *next;
} Node;

typedef struct {
    Node *start;
} List;
```
##### Funções principais:
* ```createList()``` - Cria uma nova lista.
* ```insertPriorityQueue(...)``` - Insere um processo considerando sua prioridade.
* ```removeFromList(...)``` - Remove um processo da lista por PID.
* ```getProcessList(...)``` - Acessa um processo da lista por posição.
* ```printList(...)``` - Imprime todos os processos da lista.
#### :triangular_ruler: Utilitários Visuais
Funções auxiliares para melhorar a apresentação no terminal:
* ```setColor(char *color)``` - Define a cor atual do texto.
* ```simpleRuler()``` - Imprime uma linha com ```-``` (64 caracteres).
* ```doubleRuler()``` - Imprime uma linha com ```=``` (64 caracteres).

Essas funções são úteis para organizar a saída visual, especialmente em menus e relatórios de status.



### :gear: Funcionamento main.c
#### `menu()`
``` mermaid
flowchart TD
    A[Início do Programa] --> B[Exibir Menu Principal]
    B --> D{Usuário escolhe opção}

    
    D --> C1[1: Criar Novo Processo]
    C1 --> D1[Solicita nome, prioridade e número de funções]
    D1 --> E1[Cria processo e adiciona na posição correta na fila de prioridade]
    E1 --> B

    D --> C2[2: Executar Processo]
    C2 --> D2[Seleciona processo de maior prioridade]
    D2 --> E2[Marca como 'Executando']
    E2 --> F2["Esvazia a pilha (simula execução)"]
    F2 --> G2[Move para lista de finalizados]
    G2 --> B

    D --> C3[3: Imprimir Estado do Sistema]
    C3 --> D3[Mostrar processos na fila de prioridade]
    D3 --> F3[Mostrar lista de finalizados]
    F3 --> B

    D --> C4[4: Cancelar Processo]
    C4 --> D4[Solicita PID]
    D4 --> E4["Remove processo da fila (sem executar)"]
    E4 --> B

    D --> C5[5: Sair do Sistema]
    C5 --> Z[Fim do Programa]
```

#### :zap: Funções (`main.c`)
* ```Function* setFunction(int controllerPID, int functionCounter)``` - Solicita os dados de uma função ao usuário (nome e PID) e retorna uma nova estrutura `Function` alocada dinamicamente.

* ```int setProcessQueue(List *priorityQueue, int controllerPID)``` - Coleta os dados de um processo do usuário (nome, prioridade, número de funções), cria o processo com sua pilha e o insere na fila de prioridade.
* ```Process* executeProcess(List *priorityQueue)``` - Remove o primeiro processo da fila de prioridade, marca como "Executando", simula a execução esvaziando sua pilha, e retorna o processo executado para posterior inserção na lista de finalizados.

* ```void printSystem(List *priorityQueue, List *executedList)``` - Exibe o estado atual do sistema: fila de prioridade e lista de processos finalizados.

* ```int revokeProcess(List *priorityQueue)``` - Solicita um PID ao usuário e remove o processo correspondente da fila de prioridade (sem executá-lo), se encontrado.

* ```void menu()``` - Exibe o menu principal de operações para o usuário e gerencia a navegação entre as funcionalidades do sistema.