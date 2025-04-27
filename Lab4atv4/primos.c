#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<math.h>
#include "timer.h"

long long int N, inc = 1, ret = 0; //var globais utilizadas para respectivamente salvar o valor de N, o contador de números e o contador de primos
pthread_mutex_t mutex; //variável de mutex para garantir a exclusão mútua

int ehPrimo(long long int n) { //função que verifica se o número é primo
  int i;
  if (n<=1) return 0;
  if (n==2) return 1;
  if (n%2==0) return 0;
  for (i=3; i<sqrt(n)+1; i+=2)
    if(n%i==0) return 0;
  return 1;
}

void *verifPrimo() { //função da thread 
  long long int x; //variável local para guardar o número a ser verificado
  do {
    pthread_mutex_lock(&mutex); //bloqueia a seção crítica
    inc++; //incrementa o contador
    x = inc;
    pthread_mutex_unlock(&mutex); //desbloqueia a seção crítica
    if(ehPrimo(x)) ret++; //verifica se o número é primo
  } while (inc < N);
  pthread_exit(NULL);
}

int main(int  argc, char *argv[]) {
  if(argc != 3) {  //verifica se o número de argumentos está correto
    printf("Entrada incorreta! Use: %s <valor de N> <num de threads>\n", argv[0]);
    return 1;
  }
  int nthreads = atoi(argv[2]), i; //nicializa o número de threads
  double inicio, fim, delta; //variaveis para contagem de tempo
  N = atoll(argv[1]); //inicializa o valor de N
  pthread_t *tid; //inicializa o vetor de threads
  if(nthreads > N) nthreads = N; //se o número de threads for maior que N, o número de threads é igual a N
  GET_TIME(inicio); //inicializa o cronometro
  tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads); //aloca o vetor de threads
  if(tid==NULL) { //verifica se a alocação foi bem sucedida
    puts("ERRO--malloc"); 
    return 2;
  }
  pthread_mutex_init(&mutex, NULL); //inicializa o mutex
  for(i = 0; i < nthreads; i++) {
    if(pthread_create(&tid[i], NULL, verifPrimo, NULL)) { //cria as threads
      printf("--ERRO: pthread_create()\n"); 
      exit(-1);
    }
  }
  for(i = 0; i < nthreads; i++) {
    if(pthread_join(tid[i], NULL)) { //aguarda as threads terminarem
      printf("--ERRO: pthread_join() \n"); exit(-1); 
    } 
  }
  GET_TIME(fim) //finaliza o cronometro
  delta = fim - inicio;
  delta *= 1000;
  free(tid); //libera o vetor de threads
  pthread_mutex_destroy(&mutex); //destrói o mutex
  //printf("Entre 1 e %lld, existem %lld primos!\n", N, ret);
  printf("%lld, %d, %lf\n", N, nthreads, delta); 
  return 0;
}
