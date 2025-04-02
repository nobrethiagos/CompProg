#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define MAX 1000 //valor maximo possivel para os elementos dos vetores
#define PRINT // Comente essa linha para não imprimir o conteudo do arquivo

void preencheVetor(float *vet, int N) { //funcao para preencher o vetor com valores aleatorios
  int i;
  for(i = 0; i < N; i++) {
    vet[i] = (rand()%(MAX+1)); //gera um valor aleatorio dentro do range 0 a MAX
    if(rand()%2) vet[i] *= -1; //gera um valor aleatorio para multiplicar o valor gerado 
  }
}

int main(int argc, char *argv[]) {
  if(argc != 3) {
    fprintf(stderr, "ERRO de entrada! Uso: %s <tamanho do vetor> <nome do arquivo>\n", argv[0]);
    return 1;
  }
  float *vet1, *vet2; //vetores a serem gerados
  int N = atoi(argv[1]); //tamanho do vetor
  double soma = 0; //variavel para armazenar a soma dos elementos dos vetores
  size_t verif0, verif1; //variaveis para verificar a escrita no arquivo
  FILE *descritor; //descritor do arquivo
  vet1 = (float *) malloc(N*sizeof(float)); //alocacao de memoria para os vetor 1 
  if(vet1 == NULL) {
    fprintf(stderr, "ERRO de alocacao de memoria!\n");
    return 2;
  }
  vet2 = (float *) malloc(N*sizeof(float)); //alocacao de memoria para os vetor 2
  if(vet2 == NULL) {
    fprintf(stderr, "ERRO de alocacao de memoria!\n");
    return 3;
  }
  srand(time(NULL)); //inicializa a semente para geracao de numeros aleatorios
  preencheVetor(vet1, N);
  preencheVetor(vet2, N);
  descritor = fopen(argv[2], "wb");
  fwrite(&N, sizeof(int), 1, descritor); 
  verif0 = fwrite(vet1, sizeof(float), N, descritor);
  verif1 = fwrite(vet2, sizeof(float), N, descritor);
  if(verif0 < N || verif1 < N) { //verifica se todos os elementos foram escritos
    fprintf(stderr, "Erro de escrita no  arquivo\n");
    return 4;
  }
  for(int i = 0; i < N; i++) {
    soma += vet1[i] * vet2[i]; //calcula a soma dos produtos dos elementos dos vetores
  }
  fwrite(&soma, sizeof(double), 1, descritor); 
  #ifdef PRINT
    printf("N = %d\nVetor1 = ", N);
    for(int i = 0; i < N; i++) {
      printf("%f ", vet1[i]);
    }
    printf("\nVetor2 = ");
    for(int i = 0; i < N; i++) {
      printf("%f ", vet2[i]);
    }
    printf("\nSoma = %lf\n", soma);
  #endif
  fclose(descritor);
  free(vet1);
  free(vet2);
  return 0;
}
