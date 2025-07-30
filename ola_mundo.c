#include <stdio.h>
#include <mpi.h>


/*
 relato de Pedro Vinicius 

Através do codigo compilado podemos ter um demosntrativo pratico de como um cluster funciona,
de maneira didática e dinamica. Tudo isso atráves do WSl um virtualizador de distribuição Linux,
após instalado a distribuição Ubuntu-18.04, baixado as bibliotecas de GCC e o compilador de MPI.

Com tudo isso ao rodarmos nossso programa podemos perceber que ao rodalo ele ira compilar 4 processos 
em 4 núcleos/representações de cluster's, nos quais rodaram em paralelo todos os processos.
Ao se fazer testes sobre o código, podemos comprovar que obtense um limite total tanto de processos a
serem executados como no número de processadores que iram realizar esses processos.

Com isso a exemplificação e a melhor comprienção do que de fato e um cluster, obteve-se uma melhor fixação mental.
*/
int main(int argc, char *argv[]) {
    int rank, size;

    MPI_Init(&argc, &argv); // Inicializa MPI
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Número total de processos
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // ID do processo atual

    printf("Olá do processo %d de %d\n", rank, size);

    MPI_Finalize(); // Finaliza MPI
    return 0;
}
