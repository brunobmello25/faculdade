#include "btree.h"

typedef struct linkedlist
{
    Filme *info;
    struct linkedlist *prox;
} ListaFilmes;

ListaFilmes *inicializaListaFilmes();
ListaFilmes *insereListaFilmes(Filme *novoFilme, ListaFilmes *lista);
Filme *popInicioListaFilmes(ListaFilmes **lista);
