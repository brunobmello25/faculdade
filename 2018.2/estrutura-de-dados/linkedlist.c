#include "linkedlist.h"

ListaFilmes *inicializaListaFilmes()
{
    return NULL;
}

ListaFilmes *insereListaFilmes(Filme *novoFilme, ListaFilmes *lista)
{
    ListaFilmes *novo = (ListaFilmes *)malloc(sizeof(ListaFilmes));
    novo->prox = lista;
    novo->info = novoFilme;
    return novo;
}

Filme *popInicioListaFilmes(ListaFilmes **lista)
{
    if (!(*lista))
        return NULL;
    Filme *filmeRetirado = copiaFilme((*lista)->info);
    ListaFilmes *temp = *lista;
    *lista = (*lista)->prox;
    free(temp->info);
    free(temp);
    return filmeRetirado;
}