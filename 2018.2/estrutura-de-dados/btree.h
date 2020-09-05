#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct structfilme
{
    char titulo[81];
    char diretor[51];
    char genero[31];
    int ano;
    int duracao;
} Filme;

typedef struct structbtree
{
    int nFilmes;
    int folha;
    Filme **filmes;
    char **filhos;
} BTree;

void fatalError(char *msg);
void writeBTreeToFile(char *nome, BTree *arvore, int t);
BTree *readBTreeFromFile(char *nome, int t);
BTree *criaBTreeEmMP(int t);
void liberaBTreeEmMP(BTree *a, int t);
char *buildChavePrimaria(Filme *filme);
Filme *copiaFilme(Filme *src);

char *inicializaBTree(void);
char *criaBTree(int t, int *contadorDeNomes);
void liberaBTree(int *contadorDeNomes);
char *buscaBTree(char *nomearvore, Filme *filme, int t);
char *insereBTree(char *nomeT, Filme *k, int t, int *contadorDeNomes);
void imprimirBTree(char *nomeArvore, int t, int *contador);