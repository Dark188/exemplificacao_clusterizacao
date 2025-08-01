
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include <string.h>
// Função Bubble Sort simples
void bubble_sort(int *array, int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (array[j] > array[j+1]) {
                int tmp = array[j];
                array[j] = array[j+1];
                array[j+1] = tmp;
            }
        }
    }
}

int main(int argc, char** argv) {
    int rank, size, n = 32;
    int *data = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (n % size != 0) {
        if (rank == 0) printf("Erro: n deve ser múltiplo de número de processos.\n");
        MPI_Finalize();
        return 1;
    }

    double start_time, end_time;

    int local_n = n / size;
    int *local_data = (int*)malloc(local_n * sizeof(int));

    if (rank == 0) {
        data = (int*)malloc(n * sizeof(int));
        srand(time(NULL));
        printf("Vetor original:\n");
        for (int i = 0; i < n; i++) {
            data[i] = rand() % 100;
            printf("%d ", data[i]);
        }
        printf("\n");
    }
    printf("\n#elementos = %ld\n",n);

    start_time = MPI_Wtime();

    MPI_Scatter(data, local_n, MPI_INT, local_data, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    printf("Processo %d - vetor antes da ordenação: ", rank);
    for (int i = 0; i < local_n; i++) {
        printf("%d ", local_data[i]);
    }
    printf("\n");

    bubble_sort(local_data, local_n);

    printf("Processo %d - vetor após ordenação: ", rank);
    for (int i = 0; i < local_n; i++) {
        printf("%d ", local_data[i]);
    }
    printf("\n");

    MPI_Gather(local_data, local_n, MPI_INT, data, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        // (Opcional) substitua por merge se desejar implementar
        bubble_sort(data, n);

        end_time = MPI_Wtime();

        printf("Vetor ordenado final:\n");
        for (int i = 0; i < n; i++) {
            printf("%d ", data[i]);
        }
        printf("\n");

        printf("Tempo de execução: %f segundos\n", end_time - start_time);
        free(data);
    }

    free(local_data);
    MPI_Finalize();
    return 0;
}

/*
Experiencia com bubble short em Clusterização 

- Implementa ordenação paralela usando MPI e bubble sort.
- Divide vetor em subvetores, ordena localmente e recolhe no processo 0.
- Processo 0 ordena vetor finalmente para garantir ordenação global.
- Imprime vetores locais antes e depois da ordenação para debug.
- Mede tempo total da operação usando MPI_Wtime.
- Testado com vetores de 32, 64, 128 e 256 elementos.
- Executado com 2, 4 e 8 processos para analisar escalabilidade.
Problemas e limitações:
- Firewall do Windows bloqueou paralelismo real, forçando execução single-core,
  anulando ganhos ao aumentar processos.
- Tentativa de rodar com 100000 processos causou erro por limite de pipes do sistema.
- Bubble sort é ineficiente para vetores grandes; ordenação final sequencial é gargalo.
- MPI_Gather concatena dados, mas não garante vetor global ordenado sem merge.
Análise do algoritmo e melhorias:
- Bubble sort local tem complexidade O(n²), não eficiente para grandes dados.
- Ordenação final sequencial limita escalabilidade.
- Sugestão: substituir bubble sort por quicksort ou mergesort localmente.
- Implementar fusão paralela (merge) no processo 0 para juntar subvetores ordenados.
- Usar algoritmos paralelos específicos como sample sort, bitonic sort, etc.
- Ajustar ambiente para liberar firewall e permitir uso de múltiplos núcleos.
- Limitar número de processos ao número de núcleos físicos/lógicos para evitar erros.
Conclusão:
- Experiência mostrou desafios do paralelismo em ambiente Windows com MPI.
- Código funciona, mas paralelismo não foi efetivo devido ao ambiente.
- Documentado para futuras melhorias, incluindo fusão paralela e algoritmo mais eficiente.
*/
