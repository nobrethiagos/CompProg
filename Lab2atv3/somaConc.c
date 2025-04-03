#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 

int N; //tamanho dos vetores
int numThreads; //numero de threads
float *vet1, *vet2; //vetores a serem lidos

void *produtoIntConc(void *tid) {
  long int id = (long int) tid; //identificador da thread
  int trab; //fracao de elementos por vetor a ser processada por threads
  int resto; //resto da divisao de elementos por vetor a ser processada por threads
  double produtoIntLocal = 0; //soma do  produto interno local 
  double *out; //saida da thread
  trab = (int) N/numThreads; 
  resto = N%numThreads;
  if(id < resto*numThreads) trab++;
  for(int i = 0; i < trab; i++) {
    produtoIntLocal += vet1[id*trab + i]*vet2[id*trab + i]; //calcula a soma dos produtos dos elementos dos vetores
  }
  out = (double*) malloc(sizeof(double));
  if(!out) fprintf(stderr, "ERRO de alocacao de memoria!\n");
  else *out = produtoIntLocal;
  pthread_exit((void*) out);
}

int main(int argc, char *argv[]) { 
  if(argc != 3) {
    fprintf(stderr, "ERRO de entrada! Uso: %s <numero de threads> <nome do arquivo>\n", argv[0]);
    return 1;
  }
  numThreads = atoi(argv[1]); //numero de threads
  double somaSeq, somaConc = 0, *ret; //variaveis para armazenar a soma dos produtos dos elementos dos vetores
  double varRelat; //variavel para armazenar a variacao relativa
  pthread_t *tid; //identificadores das threads
  FILE *descritor; //descritor do arquivo
  size_t out; //saida de leitura do arquivo
  descritor = fopen(argv[2], "rb");
  if(!descritor) {
    fprintf(stderr, "ERRO na abertura do arquivo!\n");
    return 2;
  }
  out = fread(&N, sizeof(int), 1, descritor); //leitura do tamanho dos vetores
  if(!out) {
    fprintf(stderr, "ERRO na leitura do arquivo!\n");
    return 3;
  }
  if(N < numThreads) numThreads = N; //caso o numero de threads seja maior que o tamanho dos vetores
  tid = (pthread_t*) malloc(numThreads*sizeof(pthread_t)); //alocacao de memoria para os identificadores das threads
  if(!tid) {
    fprintf(stderr, "ERRO de alocacao de memoria!\n");
    return 4;
  }
  vet1 = (float *) malloc(N*sizeof(float)); //alocacao de memoria para o vetor1
  if(!vet1) {
    fprintf(stderr, "ERRO de alocacao de memoria!\n");
    return 5;
  }
  vet2 = (float *) malloc(N*sizeof(float)); //alocacao de memoria para o vetor2
  if(!vet2) {
    fprintf(stderr, "ERRO de alocacao de memoria!\n");
    return 6;
  }
  out = fread(vet1, sizeof(float), N, descritor); //leitura do vetor1
  if(!out) {
    fprintf(stderr, "ERRO na leitura do arquivo!\n");
    return 7;
  }
  out = fread(vet2, sizeof(float), N, descritor); //leitura do vetor2
  if(!out) {
    fprintf(stderr, "ERRO na leitura do arquivo!\n");
    return 8;
  }
  out = fread(&somaSeq, sizeof(double), 1, descritor); //leitura da soma sequencial
  if(!out) {
    fprintf(stderr, "ERRO na leitura do arquivo!\n");
    return 9;
  }
  for(long int i = 0; i < numThreads; i++) {
    if(pthread_create(&tid[i], NULL, produtoIntConc, (void*) i)) {
      fprintf(stderr, "ERRO na criacao da thread %ld!\n", i);
      return 10;
    }
  }
  for(long int i = 0; i < numThreads; i++) {
    if(pthread_join(tid[i], (void**) &ret)) {
      fprintf(stderr, "ERRO no join da thread %ld!\n", i);
      return 11;
    }
    somaConc += *ret;
    free(ret);
  }
  printf("Soma sequencial = %lf\nSoma concorrente = %lf\n", somaSeq, somaConc);
  varRelat = (somaSeq - somaConc)/somaSeq;
  if(varRelat < 0) varRelat *= -1;
  printf("Variacao relativa = %lf\n", varRelat);
  free(vet1);
  free(vet2);
  free(tid);
  fclose(descritor);
  return 0;
}
