# Bucket Sort Paralelo
Implementação do algoritmo de ordenação Bucket Sort com __OpenMP__ para a primeira avaliação da disciplina de Introdução ao Processamento Paralelo e Distribuído na UFPel.
* O limite máximo dos valores está ajustado para 1000 logo no início do código e o limite mínimo é sempre zero. <br>
* A quantidade de "baldes" também está ajustada no início do código para 10. <br>
* O tamanho dos "baldes" é calculado como ```limite_maximo/quantidade_de_baldes```. <br>
* Foi utilizada a função ```qsort()``` para ordenação dos valores dentro dos "baldes".
 
## 🛠️ Compilando e executando:
```
gcc parallelBucketSort.c -o pbucket -fopenmp
.\pbucket 10
```
Onde 10 é o tamanho do vetor a ser ordenado. Os valores nesse vetor são escolhidos "aleatoriamente".

### ✔️ Saída
Para um vetor de tamanho 5:
```
Vetor nao ordenado:
767 566 113 521 896
Vetor ordenado:
113 521 566 767 896
```
