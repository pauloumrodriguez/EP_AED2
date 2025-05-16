//--------------------------------------------------------------
// COLOQUE OS NOMES DOS RESPONSÁVEIS AQUI: 
// Lucas Gurgel do Amaral
// Paulo Ubiratan Muniz Rodrigues
//--------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>
#include <limits.h>

#define NEGATIVO -1

// INFORME O NRO DO GRUPO CONFORME PLANILHA DIVULGADA
int grupo() {
  return 20;
}

// ######### ESCREVA O NROUSP AQUI
int nroUSP1() {
    return 14760234;
}

// E O SEGUNDO NROUSP AQUI, SE HOUVER
int nroUSP2() {
    return 14748678;
}

// elemento da lista de resposta - NAO ALTERE ESTA DEFINICAO
typedef struct estr {
      int adj; // elemento do caminho
      struct estr *prox;
} NO;


// funcao principal
NO *caminho(int N, int A, int ijpeso[], int ganhos[], int inicio, int fim, int* dinheiro);

// Função para inicializar a matriz
void initMatrizAdj(int **matriz, int *ijpeso, int A) {
  for (int i = 0; i < 3 * A; i += 3) {
    int vertice1 = ijpeso[i];
    int vertice2 = ijpeso[i + 1];
    matriz[vertice1][vertice2] = ijpeso[i + 2];
    matriz[vertice2][vertice1] = ijpeso[i + 2];
  }
}

// Função para imprimir a resposta
void imprimirCaminho(NO* teste) {
    NO* atual = teste;
    printf("{");
    while (atual != NULL) {
        if (atual->prox != NULL)
            printf("%i, ", atual->adj);
        else
            printf("%i", atual->adj);
        atual = atual->prox;
    }
    printf("}");
    printf("\n");
}

// Função para imprimir a matriz (só para visualizar melhor o que está acontecendo)
void printMatrizAdj(int **MatrizADJ, int N) {
  for (int i = 0; i < N + 1; i++) {
    for (int j = 0; j < N + 1; j++)
      printf("%i ", MatrizADJ[i][j]);
    printf("\n");
  }
}

int ** constroiMatrizAdj(int N){
  int **MatrizADJ = (int **)calloc((N + 1), sizeof(int *));
  
  for (int i = 0; i < N + 1; i++) 
    MatrizADJ[i] = (int *)calloc((N + 1), sizeof(int));
  
  return MatrizADJ; 
}

int encontrarMenorDistancia(int dist[], bool visitado[], int vertices) {
  int min_dist = INT_MAX;
  int min_index = -1;

  for (int v = 1; v <= vertices; v++) {
      if (!visitado[v] && dist[v] < min_dist) {
          min_dist = dist[v];
          min_index = v;
      }
  }

  return min_index;
}

NO* buscarCaminho(int **MatrizADJ, int N, int ganhos[], int inicio, int fim, int *dinheiro) {
  int dist[N + 1]; 
  bool visitado[N + 1]; 
  int pai[N + 1];

  for (int i = 1; i < N + 1; i++) {
      dist[i] = INT_MAX; // Inicializa todas as distâncias como infinito
      visitado[i] = false;
      pai[i] = -1;
  }
  
  dist[inicio] = *dinheiro;

  for (int count = 1; count < N + 1; count++) {
      int u = encontrarMenorDistancia(dist, visitado, N);
      
      // Verifica se nenhum vértice com distância mínima foi encontrado
      if (u == -1) break;
      
      visitado[u] = true;

      for (int v = 1; v < N + 1; v++) {
          if (!visitado[v] && MatrizADJ[u][v] != 0)  {
            int novaDist = dist[u] + MatrizADJ[u][v];
            if((novaDist < dist[v] || dist[v] == INT_MAX) && novaDist <= 0) { //dist[u] >= MatrizADJ[u][v] && //dist[u] + ganhos[v-1]*NEGATIVO + MatrizADJ[u][v] < dist[v]
              dist[v] = novaDist + ganhos[v-1]*NEGATIVO; 
              pai[v] = u;
            }
          }
      }
    }

  // Reconstrói o caminho do fim até o início
  int caminho[N], tamanho_caminho = 0;
  int atual = fim;
  
  while (atual != -1) {
      caminho[tamanho_caminho++] = atual;
      if(dist[atual] > 0)
        return NULL;
      atual = pai[atual];
  }
  
  // Cria a lista de resposta com os elementos do caminho
  NO *melhorCaminho = NULL;
  NO *proximo = NULL;

  for (int i = tamanho_caminho - 1; i >= 0; i--) {
    NO *novo = (NO*)malloc(sizeof(NO));
    novo->adj = caminho[i];
    novo->prox = NULL;
    if (melhorCaminho == NULL) {
      melhorCaminho = novo;
      proximo = novo;
    } else {
      proximo->prox = novo;
      proximo = proximo->prox;
    }
  }

  *dinheiro = dist[fim];

  return melhorCaminho;
}

// função para liberar matriz de adjacências
void liberaMatrizAdj(int **MatrizADJ, int N) {
    for (int i = 0; i <= N; i++) {
        free(MatrizADJ[i]);
    }
    free(MatrizADJ);
}

//função para liberar a lista encadeada(caminho)
void liberaCaminho(NO* caminho) {
    NO* atual = caminho;
    while (atual != NULL) {
        NO* prox = atual->prox;
        free(atual);
        atual = prox;
    }
}

//------------------------------------------
// O EP consiste em implementar esta funcao
// e outras funcoes auxiliares que esta
// necessitar
//------------------------------------------

NO *caminho(int N, int A, int ijpeso[], int ganhos[], int inicio, int fim, int* dinheiro)
{
    NO* resp = NULL;
    
    int dinheiroTemp = *dinheiro;
    dinheiroTemp *= NEGATIVO;
    
    if (inicio > N || inicio < 1 || fim > N || fim < 1)
        return resp;
        
    int **MatrizADJ = constroiMatrizAdj(N);
    initMatrizAdj(MatrizADJ, ijpeso, A);
    
    //printMatrizAdj(MatrizADJ, N); // só para ajudar a visualizar a matriz
    resp = buscarCaminho(MatrizADJ, N, ganhos, inicio, fim, &dinheiroTemp);
    imprimirCaminho(resp);
    liberaMatrizAdj(MatrizADJ, N);
    
    *dinheiro = dinheiroTemp*NEGATIVO;
    
    return resp;
}

//---------------------------------------------------------
// use main() para fazer chamadas de teste ao seu programa
//---------------------------------------------------------
int main() {

  	// TESTE 0 - DISPONIBILIZADO NO ENUNCIADO DO EP
	int N = 9;
	int ganhos[] = {20, 10, 20, 200, 200, 200, 0, 40, 20}; // o ganho em cada cidade 
	int A = 11; 
	int ijpeso[] = {1,2,30, 1,3,20, 2,6,20, 2,7,50, 3,4,20, 3,7,80, 4,9,80, 5,8,10, 6,7,10, 7,8,10, 7,9,80};
	int inicio = 7;
	int fim = 4;
	int dinheiro = 50;

        NO *teste = NULL;
        teste = caminho(N, A, ijpeso, ganhos, inicio, fim, &dinheiro);
        liberaCaminho(teste); // para não esquecer de limpar a variável teste

    return 0;
}
// por favor nao inclua nenhum código abaixo da função main()
