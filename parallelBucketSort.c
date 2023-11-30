#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define BUCKETS 10  // quantidade de baldes
#define LIM 1000 // numeros de 0 a LIM

typedef struct bucket {
    int qtd; // quantidade de elementos no balde
    int indice; // posicao do balde no contexto da ordenacao
    int inicio; // inicio do intervalo de valores associado a esse balde
}bucket;

int cmpfunc (const void * a, const void * b){   // comparacao para o qsort
    return ( *(int*)a - *(int*)b );
}

int main(int argc, char *argv[]) {    
    int *A, *B, i, N, qtd_threads, tam_buckets, qtd_elem_thread, b_indice;
    bucket *buckets; 
    if(argc != 2){
        printf("Numero de argumentos invalido.\nNao esqueca de dizer o tamanho do vetor a ser ordenado");
        return -1;
    }
    srand(time(NULL));

    N = atoi(argv[1]);  // tamanho do vetor a ser ordenado

    A = (int *) malloc(sizeof(int)*N);
    for(i=0; i<N; i++) {
	    A[i] = rand() % LIM;  // preenche o vetor a ser ordenado 
	}
	
    printf("Vetor nao ordenado: \n");
    for(i=0; i<N; i++) {
        printf("%d ", A[i]);
    }
    printf("\n");
    
    // --- informacoes compartilhadas entre threads 
    int qtds[BUCKETS];    // quantidade de elementos em cada balde
    int init_buckets[BUCKETS]; // indica onde os elementos de cada balde comecam no vetor final
    
    memset(qtds, 0, sizeof(int)*BUCKETS); // inicializa todos os elementos do vetor qtds com zero
    memset(init_buckets, 0, sizeof(int)*BUCKETS); // o mesmo para o vetor init_buckets 

    qtd_threads = BUCKETS;
    omp_set_num_threads(qtd_threads);

    tam_buckets = LIM/BUCKETS;   // tamanho dos baldes

    B = (int *) malloc(sizeof(int)*N);

    buckets = (bucket*) calloc(BUCKETS*qtd_threads, sizeof(bucket));

    #pragma omp parallel 
    {
        qtd_threads = omp_get_num_threads();
        int j, k, thread;
        int indice_balde; // posicao relativa do balde dentro da estrutura de baldes
        int indice_balde_thread; // o mesmo, mas relativa ao conjunto de baldes da thread 
        thread = omp_get_thread_num();
        qtd_elem_thread = N/qtd_threads; // quantidade de elementos que cada thread sera responsavel por processar

        #pragma omp for private(i, indice_balde_thread)
            for(i=0; i<N; i++){
                indice_balde_thread = A[i]/tam_buckets;
                if (indice_balde_thread > BUCKETS-1)
                    indice_balde_thread = BUCKETS-1;
                indice_balde = indice_balde_thread + thread*BUCKETS;
                buckets[indice_balde].qtd++;
            }

        int qtd_elem_balde = 0;   // armazena temporariamente a qtd de elementos em cada balde da thread
        for(j = thread; j < BUCKETS*qtd_threads; j = j+qtd_threads){
            qtd_elem_balde += buckets[j].qtd;
        }
        qtds[thread] = qtd_elem_balde;
        
        #pragma omp barrier // threads esperam umas pelas outras

        #pragma omp master  // bloco executado apenas pela thread mestre 
        {
            for(j=1; j<BUCKETS; j++){
		        init_buckets[j] = init_buckets[j-1] + qtds[j-1]; /* atualiza init_buckets 
                somando a posicao inicial do balde anterior com a contagem total de elementos no balde anterior */
				buckets[j].inicio = buckets[j-1].inicio + qtds[j-1]; // o mesmo para o inicio e indice dos baldes
				buckets[j].indice = buckets[j-1].indice + qtds[j-1]; 
		    }
        }

        #pragma omp barrier
        
        for(j = thread+BUCKETS; j < BUCKETS*qtd_threads; j = j+qtd_threads){
            int indice_ant = j-BUCKETS;
            buckets[j].inicio = buckets[indice_ant].inicio + buckets[indice_ant].qtd; /* atualiza a posicao 
            inicial dos elementos no balde j */
            buckets[j].indice = buckets[indice_ant].indice + buckets[indice_ant].qtd; /* atualiza o indice  
            dos elementos no balde j */
	    }
	    
        #pragma omp barrier

        #pragma omp for private(i, b_indice)
            for(i=0; i<N; i++){
                j = A[i]/tam_buckets; // calcula o indice do balde ao qual o elemento A[i] pertence
                if (j > BUCKETS-1) // verifica se o indice j esta alem do ultimo balde 
                    j = BUCKETS-1; // se estiver, ajusta j para o indice do ultimo balde
                k = j + thread*BUCKETS; // calcula o indice do balde no vetor final
                b_indice = buckets[k].indice++; 
                // indice do balde k incremetado, usado para determinar a 
                B[b_indice] = A[i]; // posicao onde o proximo elemento sera colocado no vetor final B 
            }

        #pragma omp for private(i)
            for(i=0; i<BUCKETS; i++)
                qsort(B+init_buckets[i], qtds[i], sizeof(int), cmpfunc);
    }

    printf("Vetor ordenado: \n");
    for(i=0; i<N; i++) {
        printf("%d ", B[i]);
    }
    printf("\n");

    return 0;
}
