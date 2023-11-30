# Bucket Sort Paralelo
Implementação do algoritmo de ordenação Bucket Sort com __OpenMP__ para a primeira avaliação da disciplina de Introdução ao Processamento Paralelo e Distribuído na UFPel.
* O limite máximo dos valores está ajustado para 1000 no início do código e o limite mínimo é sempre zero.<br>
* A quantidade de "baldes" também está ajustada no início do código para 10. <br>
* O tamanho dos baldes é calculado como limite_maximo/quantidade_de_baldes. <br>
* Foi utilizada a função qsort() para ordenação dos valores dentro dos "baldes".

## 🛠️ Compilando e executando:
```
gcc parallelBucketSort.c -o pbucket -fopenmp
.\pbucket 10
```
Onde: 10 é tamanho do vetor a ser ordenado. Os valores nesse vetor são calculados "aleatoriamente".

### ✔️ Saída
Para vetor de tamanho 5:


  ![sort - Copia](https://github.com/bihw/ParallelBucketSort/assets/76601652/b303db9e-0947-4ea8-8edd-9f56dd84a83f)

