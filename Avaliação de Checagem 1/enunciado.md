# Avaliação de Checagem — MPI em Laboratório
## Aplicações de clusterização em servidores

**Tema:** Comunicação entre processos com MPI (Scatter, Gather, rank, size)  
**Aluno:** _["Pedro Vinicius Ferreira Mesquita"]_  
**Data:** [05/08/2025]  

---

###  Objetivo

Executar, analisar e modificar um programa MPI que distribui dados entre processos, realiza cálculos locais e coleta os resultados para ordenação.

---

###  Arquivo base

O código que você deve utilizar está nesse diretorio:

```
media_mpi.c
```

Compile com:

```bash
mpicc media_mpi.c -o media_mpi
```

Execute com:

```bash
mpirun -np 4 ./media_mpi
```

---

### Parte 1 — Execução básica

#### 1.1. Saída esperada

Execute o programa com 4 processos. Copie aqui a saída do terminal:

```
["Processo 0 enviando dado: 42 para processo 1
Processo 0 enviando dado: 42 para processo 2
Processo 1 recebeu dado: 42 do processo 0
Processo 2 recebeu dado: 42 do processo 0
Processo 3 recebeu dado: 42 do processo 0
[DESKTOP-HIVGCRP:524] *** An error occurred in MPI_Send
[DESKTOP-HIVGCRP:524] *** reported by process [238551041,0]
[DESKTOP-HIVGCRP:524] *** on communicator MPI_COMM_WORLD
[DESKTOP-HIVGCRP:524] *** MPI_ERR_RANK: invalid rank
[DESKTOP-HIVGCRP:524] *** MPI_ERRORS_ARE_FATAL (processes in this communicator will now abort,
[DESKTOP-HIVGCRP:524] ***    and potentially your MPI job)
[warn] Epoll ADD(4) on fd 35 failed.  Old events were 0; read change was 0 (none); write change was 1 (add): Bad file descriptor
Processo 0 enviando dado: 42 para processo 3
Processo 0 enviando dado: 42 para processo 4
[warn] Epoll ADD(4) on fd 38 failed.  Old events were 0; read change was 0 (none); write change was 1 (add): Bad file descriptor
[warn] Epoll ADD(4) on fd 40 failed.  Old events were 0; read change was 0 (none); write change was 1 (add): Bad file descriptor"]
```

---

###  Parte 2 — Análise de funcionamento

#### 2.1. O que faz `MPI_Scatter` neste código?

_Resposta:_
["Ele é usado quando o processo raiz tem todos os dados e precisa dividir e enviar uma parte para cada processo.
Divide um vetor no processo raiz e envia partes para todos os processos."]
---

#### 2.2. Qual o papel de `MPI_Gather`?

_Resposta:_
["Junta valores de todos os processos e envia ao processo raiz."]
---

#### 2.3. Por que a ordenação das médias acontece apenas no processo 0?

_Resposta:_
["MPI_Gather coleta dados de todos os processos no processo 0, pois somente o processo 0 tem todos os dados, logo apenas ele pode ordenar.Os outros processos não recebem os dados dos colegas."]
---

###  Parte 3 — Modificação

#### 3.1. Modifique o código para que **cada processo envie também seu maior valor local**, além da média.

Use `MPI_Gather` para coletar ambos os dados no processo 0.

 - Faça um **commit com sua modificação** e anexe abaixo o arquivo completo.

---

### 3.2. Copie aqui a saída do seu programa modificado:

```
[SAÍDA DO PROGRAMA MODIFICADO]
```

---

### Análise com utilitários Linux

#### 4.1. Use o comando `time` para medir o tempo de execução do programa com 2, 4 e 6 processos.

Anote abaixo:

| Processos | Tempo (real) |
|-----------|--------------|
| 2         |    280s      |
| 4         |    306s      |
| 6         |    338s      |

#### 4.2. Use `htop` ou `top` para observar o uso de CPU. O uso foi balanceado entre os processos?

_Resposta:_
["O uso da CPU foi balancedo, porem creio que isso ocorreu devido a ao bloquio do Kernel cujo FireWall impos ao meu 
computador."]
---

#### 4.3. Use `strace`, `taskset` ou `MPI_Wtime` para investigar comportamento adicional do programa. Comente algo que tenha achado interessante:

_Resposta:_
["Usando MPI_Wtime, percebi que o tempo gasto na comunicação MPI entre processos é maior do que o tempo de computação local, indicando que a comunicação pode ser o gargalo do programa.
Com taskset, fixar processos em CPUs específicas melhorou o desempenho, provavelmente por reduzir trocas de contexto e cache misses.
E com strace notei que os processos frequentemente fazem chamadas bloqueantes esperando pela sincronização, o que explica tempos ociosos."]
---

### Observações

- Faça commits frequentes com mensagens claras.
- Crie um `commit` final com a tag `atividade-finalizada`.
- Envie o link do seu repositório *forkado* com a atividade completa para luis.professor@uniatenas.edu.br

---

**Boa prática!** 
