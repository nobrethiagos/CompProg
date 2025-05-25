#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<math.h>

int *lista, listaEnd = 0, verif, *cont, N, M, C;
sem_t slotCheio, slotVazio;
sem_t mutex;

void printBuffer() {
   for(int i = 0; i < M; i++) printf("%d ", lista[i]); 
   puts("\n");
}

int Retira(int id) {
   int item;
   static int out = 0;
   if(verif >= N) {
     sem_post(&slotCheio);
     sem_post(&mutex);
     return 0;
   }
   sem_wait(&slotCheio); //aguarda slot cheio para retirar
   sem_wait(&mutex); //exclusao mutua entre consumidores
   item = lista[out];
   lista[out] = 0;
   out = (out + 1) % M;
   printf("Consumidor %d: retirou %d\n", id, item);
   printBuffer(); //para log apenas
   verif++;
   sem_post(&mutex);
   sem_post(&slotVazio); //sinaliza um slot vazio
   return item;
}

void Insere(int item) {
   static int in = 0;
   sem_wait(&slotVazio); //aguarda slot vazio para inserir
   sem_wait(&mutex); //exclusao mutua entre produtores
   lista[in] = item; 
   in = (in + 1) % M;
   printf("Produtor: inseriu %d\n", item);
   printBuffer(); //para log apenas
   sem_post(&mutex);
   sem_post(&slotCheio); //sinaliza um slot cheio
}

int ehPrimo(long long int n) {
  int i;
  if(n<=1) return 0;
  if(n==2) return 1;
  if(n%2==0) return 0;
  for(i=3; i<sqrt(n)+1; i+=2)
    if(n%i==0) return 0;
  return 1;
}

void *produtor(void *arg) {
  for(int i = 1; i < N + 1; i++) {
    Insere(i); //insere o proximo item
  }
  listaEnd = 1;
  pthread_exit(NULL);
}

void *consumidor(void *arg) {
  int item; 
  long int id = (long int) arg;
  while(verif < N) {
    printf("%d\n", verif);
    item = Retira(id);
    if(ehPrimo(item)) {
      cont[id]++;
    }
  } 
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  pthread_t *tid;
  if(argc != 4) {
    printf("Uso: %s <N> <M> <C>\n", argv[0]);
    return 1;
  }
  N = atoi(argv[1]);
  M = atoi(argv[2]);
  C = atoi(argv[3]);
  if(C > M) C = M;
  verif = C - 1;
  tid = (pthread_t*) malloc((C + 1)*sizeof(pthread_t));
  lista = calloc(M, sizeof(int));
  cont = calloc(C + 1, sizeof(int));
  sem_init(&slotCheio, 0, 0);
  sem_init(&slotVazio, 0, M);
  sem_init(&mutex, 0, 1);
  pthread_create(&tid[0], NULL, produtor, NULL);
  for(long int i = 1; i < C + 1; i++) {
    pthread_create(&tid[i], NULL, consumidor, (void *) i);
  }
  pthread_join(tid[0], NULL); //aguarda o produtor
  for(long int i = 1; i < C + 1; i++) {
    pthread_join(tid[i], NULL); //aguarda os consumidores
  }
  sem_destroy(&slotCheio);
  sem_destroy(&slotVazio);
  sem_destroy(&mutex);
  for(long int i = 1; i < C + 1; i++) {
    printf("Consumidor %ld obteve %d primos!\n", i, cont[i]);
  }
  free(cont);
  free(lista);
  free(tid);
  return 0;
}

