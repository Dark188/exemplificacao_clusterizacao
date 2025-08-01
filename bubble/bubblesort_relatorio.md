# Ordenação Paralela com MPI e Bubble Sort

Este repositório contém uma implementação simples de ordenação paralela usando MPI (Message Passing Interface) e o algoritmo bubble sort.

---

## Descrição do Projeto

O programa divide um vetor em subvetores distribuídos entre os processos MPI, realiza a ordenação local (bubble sort) em cada processo e coleta os subvetores no processo 0. Este processo 0 realiza uma ordenação final sequencial para garantir a ordenação global do vetor.

### Funcionalidades

- Divide o vetor original em subvetores para cada processo MPI.
- Ordena localmente cada subvetor com bubble sort.
- Processo 0 coleta os subvetores ordenados e ordena o vetor completo.
- Imprime os vetores locais antes e depois da ordenação para fins de debug.
- Mede o tempo total da operação usando `MPI_Wtime`.
- Testado com vetores de tamanhos 32, 64, 128 e 256 elementos.
- Executado com 2, 4 e 8 processos para análise de escalabilidade.

---

## Problemas e Limitações Encontradas

- O firewall do Windows bloqueou o paralelismo real, forçando execução single-core e anulando ganhos com aumento de processos.
- Tentativa de executar com 100000 processos causou erro devido ao limite de pipes do sistema operacional.
- O bubble sort é ineficiente para vetores grandes; a ordenação final sequencial é um gargalo para a escalabilidade.
- A função `MPI_Gather` concatena os dados, mas não garante vetor global ordenado sem um processo de merge adicional.

---

## Análise do Algoritmo e Possíveis Melhorias

- Bubble sort local tem complexidade O(n²), não é eficiente para dados grandes.
- Ordenação final sequencial limita a escalabilidade do programa.
- Sugestão de melhorias:
  - Substituir bubble sort por algoritmos mais eficientes localmente, como quicksort ou mergesort.
  - Implementar fusão paralela (merge) no processo 0 para juntar subvetores ordenados ao invés de ordenar sequencialmente.
  - Utilizar algoritmos de ordenação paralela especializados, como sample sort ou bitonic sort.
  - Ajustar o ambiente para liberar o firewall do Windows e permitir o uso real de múltiplos núcleos.
  - Limitar o número de processos ao número de núcleos físicos ou lógicos da máquina para evitar erros do sistema.

---

## Conclusão

- A experiência demonstrou desafios práticos do paralelismo em ambiente Windows com MPI.
- O código funciona corretamente, mas o paralelismo não foi efetivo devido às limitações do ambiente.
- O projeto está documentado para futuras melhorias, incluindo fusão paralela e uso de algoritmos mais eficientes.

---

