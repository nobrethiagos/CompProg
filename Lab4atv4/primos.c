#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<math.h>
#include "timer.h"

long long int N, inc = 1, ret = 0;
pthread_mutex_t mutex;

int ehPrimo(long long int n) {
  int i;
  if (n<=1) return 0;
  if (n==2) return 1;
  if (n%2==0) return 0;
  for (i=3; i<sqrt(n)+1; i+=2)
    if(n%i==0) return 0;
  return 1;
}

void *verifPrimo() {
  long long int x;
  do {
    pthread_mutex_lock(&mutex);
    inc++;
    x = inc;
    pthread_mutex_unlock(&mutex);
    if(ehPrimo(x)) ret++;
  } while (inc < N);
  pthread_exit(NULL);
}

int main(int  argc, char *argv[]) {
  if(argc != 3) {
    printf("Entrada incorreta! Use: %s <valor de N> <num de threads>\n", argv[0]);
    return 1;
  }
  int nthreads = atoi(argv[2]), i;
  double inicio, fim, delta;
  N = atoll(argv[1]);
  pthread_t *tid;
  if(nthreads > N) nthreads = N;
  GET_TIME(inicio);
  tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
  if(tid==NULL) {
    puts("ERRO--malloc"); 
    return 2;
  }
  pthread_mutex_init(&mutex, NULL);
  for(i = 0; i < nthreads; i++) {
    if(pthread_create(&tid[i], NULL, verifPrimo, NULL)) {
      printf("--ERRO: pthread_create()\n"); 
      exit(-1);
    }
  }
  for(i = 0; i < nthreads; i++) {
    if(pthread_join(tid[i], NULL)) {
      printf("--ERRO: pthread_join() \n"); exit(-1); 
    } 
  }
  GET_TIME(fim)   
  delta = fim - inicio;
  delta *= 1000;
  free(tid);
  pthread_mutex_destroy(&mutex);
  //printf("Entre 1 e %lld, existem %lld primos!\n", N, ret);
  printf("%lld, %d, %lf\n", N, nthreads, delta);
  return 0;
}
