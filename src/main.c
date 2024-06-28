#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct 
{
    char nome[20];
    int tempo_execucao;
} Tarefa;

typedef struct 
{
    int id;
    int tempo_atual;
    int n_tarefas;
    Tarefa tarefas[100];
    int inicio[100];
    int fim[100];
} Processador;

int compararTarefasSJF(const void *a, const void *b)
{
    Tarefa *tarefaA = (Tarefa *)a;
    Tarefa *tarefaB = (Tarefa *)b;
    return tarefaA->tempo_execucao - tarefaB->tempo_execucao;
} // parametro para o qSort;

int compararTarefasInverso(const void *a, const void *b)
{
    Tarefa *tarefaA = (Tarefa *)a;
    Tarefa *tarefaB = (Tarefa *)b;
    return tarefaB->tempo_execucao - tarefaA->tempo_execucao;
} // parametro para o qSort;

int processadorLivre(Processador *processadores, int n_processadores)
{
    int livreIndex = 0;

    for (int i = 0; i < n_processadores; i++)
    {
        if (processadores[i].tempo_atual < processadores[livreIndex].tempo_atual)
        {
            livreIndex = i;
        }
    } // encontra e retorna o processador com menor tempo registrado;

    return livreIndex;
}

void escalonarTarefas(Tarefa *tarefas, int n_tarefas, int n_processadores, 
                      int (*comparar)(const void*, const void*), const char *arquivo_saida)
{
    qsort(tarefas, n_tarefas, sizeof(Tarefa), comparar);

    Processador processadores[n_processadores];

    for (int i = 0; i < n_processadores; i++)
    {
        processadores[i].id = i;
        processadores[i].tempo_atual = 0;
        processadores[i].n_tarefas = 0;
    } // inicializa os processadores;

    for (int i = 0; i < n_tarefas; i++)
    {
        int livre = processadorLivre(processadores, n_processadores);
        int inicio = processadores[livre].tempo_atual;
        int fim = inicio + tarefas[i].tempo_execucao;

        processadores[livre].tarefas[processadores[livre].n_tarefas] = tarefas[i];
        processadores[livre].inicio[processadores[livre].n_tarefas] = inicio;
        processadores[livre].fim[processadores[livre].n_tarefas] = fim;
        processadores[livre].n_tarefas++;

        processadores[livre].tempo_atual = fim;
    }

    FILE *saida = fopen(arquivo_saida, "w");

    if (saida == NULL)
    {
        printf("Erro ao criar arquivo de saída.\n");
        exit(1);
    }

    for (int i = 0; i < n_processadores; i++)
    {
        fprintf(saida, "Processador_%d\n", processadores[i].id);
        for (int j = 0; j < processadores[i].n_tarefas; j++)
        {
            fprintf(saida, "%s;%d;%d\n", processadores[i].tarefas[j].nome,
                    processadores[i].inicio[j], processadores[i].fim[j]);
        }
    }

    fclose(saida);
}

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        printf("Uso: %s <arquivo_de_entrada> <n_processadores>\n", argv[0]);
        return 1;
    }
    
    char *arquivo_entrada = argv[1];
    int n_processadores = atoi(argv[2]);

    FILE *fp = fopen(arquivo_entrada, "r");

    if (fp == NULL)
    {
        printf("Erro ao abrir arquivo\n");
        return 1;
    }

    Tarefa tarefas[100];
    int n_tarefas = 0;

    while (fscanf(fp, "%s %d", tarefas[n_tarefas].nome, 
                               &tarefas[n_tarefas].tempo_execucao) != EOF)
    {
        n_tarefas++;
    } // lê as tarefas e armazena na ED;

    fclose(fp);

    escalonarTarefas(tarefas, n_tarefas, n_processadores, 
                      compararTarefasSJF, "saida_sjf.txt");
    escalonarTarefas(tarefas, n_tarefas, n_processadores, 
                      compararTarefasInverso, "saida_inverso.txt");

    return 0;
}