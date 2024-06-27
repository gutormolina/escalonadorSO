#include <stdio.h>

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

    while (!EOF)
    {
        fread();
    }

    return 0;
}
