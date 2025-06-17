//Busca em largura com lista de adjacências (BFS)

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAXNUMVERTICES 100
#define INFINITO INT_MAX
#define TRUE 1
#define FALSE 0

//Tipos do programa
typedef int TipoValorVertice;
typedef int TipoPeso;

typedef struct TipoItem {
  TipoValorVertice Vertice;
  TipoPeso Peso;
} TipoItem;

typedef struct TipoCelula* TipoApontador;
typedef struct TipoCelula {
  TipoItem Item;
  TipoApontador Prox;
} TipoCelula;
typedef struct TipoLista {
  TipoApontador Primeiro, Ultimo;
} TipoLista;
typedef struct TipoFila {
  TipoApontador Frente, Tras;
} TipoFila;
typedef struct TipoGrafo {
  TipoLista Adj[MAXNUMVERTICES + 1];
  TipoValorVertice NumVertices;
  int NumArestas;
} TipoGrafo;
typedef enum { branco, cinza, preto } TipoCor;

// Lista vazia
void FLVazia(TipoLista *Lista) {
  Lista->Primeiro = (TipoApontador)malloc(sizeof(TipoCelula));
  Lista->Ultimo = Lista->Primeiro;
  Lista->Primeiro->Prox = NULL;
}

// Verifica se a lista está vazia
short ListaVazia(TipoLista Lista) {
  return (Lista.Primeiro == Lista.Ultimo);
}

// Insere item
void Insere(TipoItem *x, TipoLista *Lista) {
  Lista->Ultimo->Prox = (TipoApontador)malloc(sizeof(TipoCelula));
  Lista->Ultimo = Lista->Ultimo->Prox;
  Lista->Ultimo->Item = *x;
  Lista->Ultimo->Prox = NULL;
}

//Inicializa a fila
void FFVazia(TipoFila *Fila) {
  Fila->Frente = (TipoApontador)malloc(sizeof(TipoCelula));
  Fila->Tras = Fila->Frente;
  Fila->Frente->Prox = NULL;
}

//Verifica se a fila está vazia
short FilaVazia(TipoFila Fila) {
  return (Fila.Frente == Fila.Tras);
}

// Enfileira item no final da fila
void Enfileira(TipoItem x, TipoFila *Fila) {
  Fila->Tras->Prox = (TipoApontador)malloc(sizeof(TipoCelula));
  Fila->Tras = Fila->Tras->Prox;
  Fila->Tras->Item = x;
  Fila->Tras->Prox = NULL;
}

// Remove item da frente da fila
void Desenfileira(TipoFila *Fila, TipoItem *Item) {
  if (FilaVazia(*Fila)) {
    printf("Erro: fila está vazia\n");
    return;
  }
  TipoApontador q = Fila->Frente;
  Fila->Frente = Fila->Frente->Prox;
  *Item = Fila->Frente->Item;
  free(q);
}

// Insere aresta entre dois vértices
void InsereAresta(TipoValorVertice *V1, TipoValorVertice *V2, TipoPeso *Peso, TipoGrafo *Grafo) {
  TipoItem x;
  x.Vertice = *V2;
  x.Peso = *Peso;
  Insere(&x, &Grafo->Adj[*V1]);
}

// Inicializa todas as listas de adjacência
void FGVazio(TipoGrafo *Grafo) {
  for (int i = 0; i < Grafo->NumVertices; i++)
    FLVazia(&Grafo->Adj[i]);
}

// Verifica se lista de adjacência de um vértice está vazia
short ListaAdjVazia(TipoValorVertice *Vertice, TipoGrafo *Grafo) {
  return (Grafo->Adj[*Vertice].Primeiro == Grafo->Adj[*Vertice].Ultimo);
}

// Primeiro item da lista de adjacência
TipoApontador PrimeiroListaAdj(TipoValorVertice *Vertice, TipoGrafo *Grafo) {
  return (Grafo->Adj[*Vertice].Primeiro->Prox);
}

// Avança para próxima adjacência
void ProxAdj(TipoValorVertice *Vertice, TipoValorVertice *Adj,
             TipoPeso *Peso, TipoApontador *Prox, short *FimListaAdj) {
  *Adj = (*Prox)->Item.Vertice;
  *Peso = (*Prox)->Item.Peso;
  *Prox = (*Prox)->Prox;
  if (*Prox == NULL)
    *FimListaAdj = TRUE;
}

// Buscar em largura (BFS) 
void VisitaBfs(TipoValorVertice u, TipoGrafo *Grafo,
               int *Dist, TipoCor *Cor, int *Antecessor) {
  TipoFila Fila;
  TipoItem Item;
  FFVazia(&Fila);
  Cor[u] = cinza;
  Dist[u] = 0;
  Antecessor[u] = -1;
  Item.Vertice = u;
  Item.Peso = 0;
  Enfileira(Item, &Fila);

  while (!FilaVazia(Fila)) {
    Desenfileira(&Fila, &Item);
    u = Item.Vertice;

    if (!ListaAdjVazia(&u, Grafo)) {
      TipoApontador Aux = PrimeiroListaAdj(&u, Grafo);
      short FimListaAdj = FALSE;
      TipoValorVertice v;
      TipoPeso Peso;

      while (!FimListaAdj) {
        ProxAdj(&u, &v, &Peso, &Aux, &FimListaAdj);
        if (Cor[v] == branco) {
          Cor[v] = cinza;
          Dist[v] = Dist[u] + 1;
          Antecessor[v] = u;
          TipoItem Novo;
          Novo.Vertice = v;
          Novo.Peso = Peso;
          Enfileira(Novo, &Fila);
        }
      }
    }

    Cor[u] = preto;
  }
}

// Imprime caminho mais curto
void ImprimeCaminho(TipoValorVertice Origem, TipoValorVertice v,
                    TipoGrafo *Grafo, int *Dist, TipoCor *Cor, int *Antecessor) {
  if (Origem == v) {
    printf("%d", Origem);
  } else if (Antecessor[v] == -1) {
    printf("Nao existe caminho de %d ate %d", Origem, v);
  } else {
    ImprimeCaminho(Origem, Antecessor[v], Grafo, Dist, Cor, Antecessor);
    printf(" -> %d", v);
  }
}

// Imprime todas as listas do grafo
void ImprimeGrafo(TipoGrafo *Grafo) {
  for (int i = 0; i < Grafo->NumVertices; i++) {
    printf("Vertice %d: ", i);
    TipoApontador Aux = Grafo->Adj[i].Primeiro->Prox;
    while (Aux != NULL) {
      printf("%d (%d)  ", Aux->Item.Vertice, Aux->Item.Peso);
      Aux = Aux->Prox;
    }
    printf("\n");
  }
}

int main() {
  TipoGrafo G1, G2;
  TipoPeso Peso = 1;
  int i;

  // Grafo 1
  G1.NumVertices = 5;
  G1.NumArestas = 0;
  FGVazio(&G1);
  int arestas1[][2] = { {0,1}, {1,2}, {2,3}, {3,4} };
  for (i = 0; i < 4; i++) {
    InsereAresta(&arestas1[i][0], &arestas1[i][1], &Peso, &G1);
    InsereAresta(&arestas1[i][1], &arestas1[i][0], &Peso, &G1);
  }

  int Dist[MAXNUMVERTICES + 1], Antecessor[MAXNUMVERTICES + 1];
  TipoCor Cor[MAXNUMVERTICES + 1];

  printf("Grafo 1:\n\n");
  ImprimeGrafo(&G1);
  for (i = 0; i < G1.NumVertices; i++) {
    Cor[i] = branco; Dist[i] = INFINITO; Antecessor[i] = -1;
  }
  VisitaBfs(0, &G1, Dist, Cor, Antecessor);
  printf("Caminho de 0 até 4: ");
  ImprimeCaminho(0, 4, &G1, Dist, Cor, Antecessor);
  printf("\n\n");

  // Grafo 2
  G2.NumVertices = 6;
  G2.NumArestas = 0;
  FGVazio(&G2);
  int arestas2[][2] = {
    {0,1}, {1,5},             
    {0,2}, {2,3}, {3,4}, {4,5} 
  };
  for (i = 0; i < 6; i++) {
    InsereAresta(&arestas2[i][0], &arestas2[i][1], &Peso, &G2);
    InsereAresta(&arestas2[i][1], &arestas2[i][0], &Peso, &G2);
  }

  printf("Grafo 2:\n\n");
  ImprimeGrafo(&G2);
  for (i = 0; i < G2.NumVertices; i++) {
    Cor[i] = branco; Dist[i] = INFINITO; Antecessor[i] = -1;
  }
  VisitaBfs(0, &G2, Dist, Cor, Antecessor);

  printf("Caminho de 0 até 5: ");
  ImprimeCaminho(0, 5, &G2, Dist, Cor, Antecessor);
  printf("\n");

  printf("Caminho de 0 até 3: ");
  ImprimeCaminho(0, 3, &G2, Dist, Cor, Antecessor);
  printf("\n");

  printf("Caminho de 2 até 5: ");
  ImprimeCaminho(2, 5, &G2, Dist, Cor, Antecessor);
  printf("\n");

  printf("Caminho de 3 até 0: ");
  ImprimeCaminho(3, 0, &G2, Dist, Cor, Antecessor);
  printf("\n");

  return 0;
}

