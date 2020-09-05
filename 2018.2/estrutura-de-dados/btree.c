#include "btree.h"

void fatalError(char *msg)
{
    printf("ERRO: %s\n", msg);
    exit(1);
}

void writeBTreeToFile(char *nomeFile, BTree *arvore, int t)
{
    char *nomeDiretorio = (char *)malloc(sizeof(char) * 15);
    sprintf(nomeDiretorio, "estrutura/%s", nomeFile);

    FILE *fp = fopen(nomeDiretorio, "wb");
    free(nomeDiretorio);
    if (!fp)
        fatalError("falha ao abrir arquivo em writeBTreeToFile");

    long desloc = 0l;
    int i;
    fseek(fp, 0L, SEEK_SET);

    fwrite(&(arvore->folha), sizeof(int), 1, fp);
    desloc += sizeof(int);
    fseek(fp, desloc, SEEK_SET);

    fwrite(&(arvore->nFilmes), sizeof(int), 1, fp);
    desloc += sizeof(int);
    fseek(fp, desloc, SEEK_SET);

    for (i = 0; i < (2 * t); i++)
    {
        fwrite(arvore->filhos[i], sizeof(char), 5, fp);
        desloc += sizeof(char) * 5;
        fseek(fp, desloc, SEEK_SET);
    }

    for (i = 0; i < arvore->nFilmes; i++)
    {
        if (arvore->filmes[i])
            fwrite(arvore->filmes[i], sizeof(Filme), 1, fp);
        desloc += sizeof(Filme);
        fseek(fp, desloc, SEEK_SET);
    }

    fclose(fp);
}

BTree *readBTreeFromFile(char *nome, int t)
{

    char *nomeDiretorio = (char *)malloc(sizeof(char) * 15);
    sprintf(nomeDiretorio, "estrutura/%s", nome);

    FILE *fp = fopen(nomeDiretorio, "rb");
    free(nomeDiretorio);
    if (!fp)
        fatalError("falha ao abrir o arquivo em readBTreeFromFile");

    BTree *novo = (BTree *)malloc(sizeof(BTree));
    novo->filhos = (char **)malloc(sizeof(char *) * (2 * t));
    novo->filmes = (Filme **)malloc(sizeof(Filme *) * ((2 * t) - 1));

    long desloc = 0L;
    int i;
    fseek(fp, desloc, SEEK_SET);

    fread(&(novo->folha), sizeof(int), 1, fp);
    desloc += sizeof(int);
    fseek(fp, desloc, SEEK_SET);

    fread(&(novo->nFilmes), sizeof(int), 1, fp);
    desloc += sizeof(int);
    fseek(fp, desloc, SEEK_SET);

    for (i = 0; i < (2 * t); i++)
    {
        novo->filhos[i] = (char *)malloc(sizeof(char) * 5);
        fread(novo->filhos[i], sizeof(char), 5, fp);
        desloc += sizeof(char) * 5;
        fseek(fp, desloc, SEEK_SET);
    }

    for (i = 0; i < novo->nFilmes; i++)
    {
        novo->filmes[i] = (Filme *)malloc(sizeof(Filme));
        fread(novo->filmes[i], sizeof(Filme), 1, fp);
        desloc += sizeof(Filme);
        fseek(fp, desloc, SEEK_SET);
    }
    for (i = i; i < ((2 * t) - 1); i++)
    {
        novo->filmes[i] = NULL;
    }

    fclose(fp);
    return novo;
}

BTree *criaBTreeEmMP(int t)
{
    BTree *novo = (BTree *)malloc(sizeof(BTree));
    novo->filmes = (Filme **)malloc(sizeof(Filme *) * ((2 * t) - 1));
    novo->filhos = (char **)malloc(sizeof(char *) * (2 * t));
    novo->folha = 1;
    novo->nFilmes = 0;
    int i;
    for (i = 0; i < (2 * t); i++)
    {
        novo->filhos[i] = (char *)malloc(sizeof(char) * 5);
        sprintf(novo->filhos[i], "null");
    }
    for (i = 0; i < ((2 * t) - 1); i++)
        novo->filmes[i] = NULL;

    return novo;
}

void liberaBTreeEmMP(BTree *a, int t)
{
    if (a)
    {
        int i;
        for (i = 0; i < ((2 * t) - 1); i++)
            free(a->filmes[i]);
        for (i = 0; i < (2 * t); i++)
            free(a->filhos[i]);
        free(a->filmes);
        free(a->filhos);
        free(a);
    }
}

char *buildChavePrimaria(Filme *filme)
{
    char *chavePrimaria = (char *)malloc(sizeof(char) * 86);
    if (filme)
        sprintf(chavePrimaria, "%s_%d", filme->titulo, filme->ano);
    else
        sprintf(chavePrimaria, "%s", "");
    return chavePrimaria;
}

Filme *copiaFilme(Filme *src)
{
    Filme *novo = (Filme *)malloc(sizeof(Filme));
    strcpy(novo->titulo, src->titulo);
    strcpy(novo->diretor, src->diretor);
    strcpy(novo->genero, src->genero);
    novo->ano = src->ano;
    novo->duracao = src->duracao;
    return novo;
}

char *inicializaBTree(void)
{
    return NULL;
}

char *criaBTree(int t, int *contadorDeNomes)
{
    BTree *novo = criaBTreeEmMP(t);
    char *novoNome = (char *)malloc(sizeof(char) * 5);
    sprintf(novoNome, "%04d", *contadorDeNomes);
    *contadorDeNomes += 1;
    writeBTreeToFile(novoNome, novo, t);
    liberaBTreeEmMP(novo, t);
    return novoNome;
}

void liberaBTree(int *contadorDeNomes)
{
    char *nomeDiretorio = (char *)malloc(sizeof(char) * 15);
    for (int i = 0; i < *contadorDeNomes; i++)
    {
        sprintf(nomeDiretorio, "estrutura/%04d", i);
        remove(nomeDiretorio);
    }
    free(nomeDiretorio);
    *contadorDeNomes = 0;
}

char *buscaBTree(char *nomearvore, Filme *filme, int t)
{
    if (!nomearvore)
        return NULL;

    BTree *arvore = readBTreeFromFile(nomearvore, t);
    if (!arvore)
        return NULL;

    int i = 0;
    char *chavebuscada = buildChavePrimaria(filme);
    char *chaveFilmeAtual = buildChavePrimaria(arvore->filmes[0]);

    while (i < arvore->nFilmes && strcmp(chavebuscada, chaveFilmeAtual) > 0)
    {
        i++;
        free(chaveFilmeAtual);
        if (i < ((2 * t) - 1))
            chaveFilmeAtual = buildChavePrimaria(arvore->filmes[i]);
        else
            chaveFilmeAtual = buildChavePrimaria(NULL);
    }

    if (i < arvore->nFilmes && strcmp(chavebuscada, chaveFilmeAtual) == 0)
    {
        free(chavebuscada);
        free(chaveFilmeAtual);
        liberaBTreeEmMP(arvore, t);

        char *resp = (char *)malloc(sizeof(char) * 5);
        strcpy(resp, nomearvore);

        return resp;
    }

    if (arvore->folha)
    {
        free(chavebuscada);
        free(chaveFilmeAtual);
        liberaBTreeEmMP(arvore, t);
        return NULL;
    }

    if (strcmp(arvore->filhos[i], "null") == 0)
    {
        free(chavebuscada);
        free(chaveFilmeAtual);
        liberaBTreeEmMP(arvore, t);
        return NULL;
    }

    free(chavebuscada);
    free(chaveFilmeAtual);

    char *resp = buscaBTree(arvore->filhos[i], filme, t);
    liberaBTreeEmMP(arvore, t);
    return resp;
}

static BTree *divisaoBTree(BTree *x, int i, BTree *y, int t, int *contadorDeNomes)
{
    char *nomeZ = criaBTree(t, contadorDeNomes);
    BTree *z = readBTreeFromFile(nomeZ, t);
    z->nFilmes = t - 1;
    z->folha = y->folha;
    int j;

    for (j = 0; j < t - 1; j++)
        z->filmes[j] = copiaFilme(y->filmes[j + t]);

    if (!(y->folha))
    {
        for (j = 0; j < t; j++)
        {
            strcpy(z->filhos[j], y->filhos[j + t]);
            strcpy(y->filhos[j + t], "null");
        }
    }

    y->nFilmes = t - 1;

    for (j = x->nFilmes; j >= i; j--)
        strcpy(x->filhos[j + 1], x->filhos[j]);

    strcpy(x->filhos[i], nomeZ);

    for (j = x->nFilmes; j >= i; j--)
        x->filmes[j] = copiaFilme(x->filmes[j - 1]);

    x->filmes[i - 1] = copiaFilme(y->filmes[t - 1]);
    x->nFilmes += 1;

    writeBTreeToFile(nomeZ, z, t);
    liberaBTreeEmMP(z, t);
    free(nomeZ);

    return x;
}

static BTree *insereNaoCompletoBTree(BTree *x, Filme *k, int t, int *contadorDeNomes)
{
    int i = x->nFilmes - 1;

    char *chaveNovoFilme = buildChavePrimaria(k);
    char *chaveFilmeAtual = buildChavePrimaria(x->filmes[i]);

    if (x->folha)
    {
        while ((i >= 0) && (strcmp(chaveNovoFilme, chaveFilmeAtual) < 0))
        {
            x->filmes[i + 1] = copiaFilme(x->filmes[i]);
            i--;
            if (i >= 0)
            {
                free(chaveFilmeAtual);
                chaveFilmeAtual = buildChavePrimaria(x->filmes[i]);
            }
        }
        x->filmes[i + 1] = copiaFilme(k);
        x->nFilmes += 1;
        free(chaveNovoFilme);
        free(chaveFilmeAtual);
        return x;
    }

    while ((i >= 0) && (strcmp(chaveNovoFilme, chaveFilmeAtual) < 0))
    {
        i--;
        if (i >= 0)
        {
            free(chaveFilmeAtual);
            chaveFilmeAtual = buildChavePrimaria(x->filmes[i]);
        }
    }
    i++;
    free(chaveFilmeAtual);
    if (i < ((2 * t) - 1))
        chaveFilmeAtual = buildChavePrimaria(x->filmes[i]);
    else
        chaveFilmeAtual = buildChavePrimaria(NULL);

    char *nomeFilhoAtual = (char *)malloc(sizeof(char) * 5);
    sprintf(nomeFilhoAtual, "%s", x->filhos[i]);
    BTree *filhoAtual = readBTreeFromFile(nomeFilhoAtual, t);

    if (filhoAtual->nFilmes == ((2 * t) - 1))
    {
        x = divisaoBTree(x, (i + 1), filhoAtual, t, contadorDeNomes);
        free(chaveFilmeAtual);
        chaveFilmeAtual = buildChavePrimaria(x->filmes[i]);
        if (strcmp(chaveNovoFilme, chaveFilmeAtual) > 0)
        {
            i++;
            free(chaveFilmeAtual);
            if (i < ((2 * t) - 1))
                chaveFilmeAtual = buildChavePrimaria(x->filmes[i]);
            else
                chaveFilmeAtual = buildChavePrimaria(NULL);
        }
    }
    writeBTreeToFile(nomeFilhoAtual, filhoAtual, t);
    liberaBTreeEmMP(filhoAtual, t);

    sprintf(nomeFilhoAtual, "%s", x->filhos[i]);
    filhoAtual = readBTreeFromFile(nomeFilhoAtual, t);
    filhoAtual = insereNaoCompletoBTree(filhoAtual, k, t, contadorDeNomes);
    writeBTreeToFile(nomeFilhoAtual, filhoAtual, t);
    free(nomeFilhoAtual);
    liberaBTreeEmMP(filhoAtual, t);
    filhoAtual = NULL;
    return x;
}

char *insereBTree(char *nomeT, Filme *k, int t, int *contadorDeNomes)
{
    char *respBusca = buscaBTree(nomeT, k, t);
    if (respBusca)
    {
        free(respBusca);
        return nomeT;
    }
    free(respBusca);

    if (!nomeT)
    {
        nomeT = criaBTree(t, contadorDeNomes);
        BTree *T = readBTreeFromFile(nomeT, t);
        T->filmes[0] = copiaFilme(k);
        T->nFilmes = 1;
        writeBTreeToFile(nomeT, T, t);
        liberaBTreeEmMP(T, t);
        return nomeT;
    }

    BTree *T = readBTreeFromFile(nomeT, t);
    if (T->nFilmes == ((2 * t) - 1))
    {
        // criei novo nó
        char *nomeS = criaBTree(t, contadorDeNomes);
        BTree *S = readBTreeFromFile(nomeS, t);

        S->nFilmes = 0;
        S->folha = 0;
        strcpy(S->filhos[0], nomeT);

        S = divisaoBTree(S, 1, T, t, contadorDeNomes);

        writeBTreeToFile(nomeS, S, t);
        liberaBTreeEmMP(S, t);
        writeBTreeToFile(nomeT, T, t);
        liberaBTreeEmMP(T, t);

        S = readBTreeFromFile(nomeS, t);
        S = insereNaoCompletoBTree(S, k, t, contadorDeNomes);
        writeBTreeToFile(nomeS, S, t);

        liberaBTreeEmMP(S, t);

        free(nomeT);
        return nomeS;
    }

    T = insereNaoCompletoBTree(T, k, t, contadorDeNomes);
    writeBTreeToFile(nomeT, T, t);
    liberaBTreeEmMP(T, t);
    return nomeT;
}

void imprimirBTree(char *nomeArvore, int t, int *contador)
{
    BTree *a = readBTreeFromFile(nomeArvore, t);
    int i;
    for (i = 0; i < a->nFilmes; i++)
    {
        if (!(a->folha))
        {
            char nomeFilho[5];
            sprintf(nomeFilho, "%s", a->filhos[i]);
            imprimirBTree(nomeFilho, t, contador);
        }
        *contador += 1;
        printf("%03d: %s\n", *contador, a->filmes[i]->titulo);
        printf("     %d | %s | %s | %d\n", a->filmes[i]->ano, a->filmes[i]->diretor, a->filmes[i]->genero, a->filmes[i]->duracao);
        printf("-------------------------------------------------------------------\n");
    }
    if (!(a->folha))
        imprimirBTree(a->filhos[i], t, contador);
    liberaBTreeEmMP(a, t);
}

// Nao concluida
static char *remover(char *nomearvore, Filme *filme, int t)
{
    if (!nomearvore)
        return nomearvore;
    BTree *arvore = readBTreeFromFile(nomearvore, t);

    int i = 0;
    printf("Removendo %s...\n", filme->titulo);
    char *chaveFilmeRemover = buildChavePrimaria(filme);
    char *chaveFilmeAtual = buildChavePrimaria(arvore->filmes[i]);
    while (i < arvore->nFilmes && (strcmp(chaveFilmeAtual, chaveFilmeRemover) < 0))
    {
        i++;
        free(chaveFilmeAtual);
        chaveFilmeAtual = buildChavePrimaria(arvore->filmes[i]);
    }
    if (i < arvore->nFilmes && strcmp(chaveFilmeAtual, chaveFilmeRemover) == 0) // CASOS 1, 2A, 2B, 2C
    {
        if (arvore->folha) // CASO 1
        {
            printf("CASO 1\n");
            int j;
            for (j = i; j < arvore->nFilmes - 1; j++)
            {
                free(arvore->filmes[j]);
                arvore->filmes[j] = copiaFilme(arvore->filmes[j + 1]);
            }
            arvore->nFilmes -= 1;
            writeBTreeToFile(nomearvore, arvore, t);
            free(chaveFilmeAtual);
            free(chaveFilmeRemover);
            liberaBTreeEmMP(arvore, t);
            return nomearvore;
        }

        char *nomeFilhoAtual = (char *)malloc(sizeof(char) * 5);
        sprintf(nomeFilhoAtual, "%s", arvore->filhos[i]);
        BTree *filhoAtual = readBTreeFromFile(nomeFilhoAtual, t);

        char *nomeProximoFilho = (char *)malloc(sizeof(char) * 5);
        sprintf(nomeProximoFilho, "%s", arvore->filhos[i + 1]);
        BTree *proximoFilho = readBTreeFromFile(nomeProximoFilho, t);

        if (!(arvore->folha) && filhoAtual->nFilmes >= t) // CASO 2A
        {
            while (!(filhoAtual->folha)) // encontrar o predecessor k' de k na árvore com raiz em filhoAtual
            {
                sprintf(nomeFilhoAtual, "%s", filhoAtual->filhos[filhoAtual->nFilmes]);
                liberaBTreeEmMP(filhoAtual, t);
                filhoAtual = readBTreeFromFile(nomeFilhoAtual, t);
            }
            Filme *temp = copiaFilme(filhoAtual->filmes[filhoAtual->nFilmes - 1]);

            liberaBTreeEmMP(filhoAtual, t);
            sprintf(nomeFilhoAtual, "%s", arvore->filhos[i]);
            nomeFilhoAtual = remover(nomeFilhoAtual, temp, t);

            filhoAtual = readBTreeFromFile(nomeFilhoAtual, t);
            free(filhoAtual->filmes[i]);
            filhoAtual->filmes[i] = temp;
            writeBTreeToFile(nomeFilhoAtual, filhoAtual, t);
            liberaBTreeEmMP(filhoAtual, t);
            free(temp);
            free(nomeFilhoAtual);

            return nomearvore;
        }
        if (!(arvore->folha) && proximoFilho->nFilmes >= t) // CASO 2B
        {
            while (!(proximoFilho->folha))
            {
                sprintf(nomeProximoFilho, "%s", proximoFilho->filhos[0]);
                liberaBTreeEmMP(proximoFilho, t);
                proximoFilho = readBTreeFromFile(nomeProximoFilho, t);
            }
            Filme *temp = copiaFilme(proximoFilho->filmes[0]);
            
            liberaBTreeEmMP(proximoFilho, t);

        }
    }
}

// Nao concluida
char *retira(char *arvore, Filme *filme, int t)
{
    if (!arvore)
        return arvore;
    char *respBusca = buscaBTree(arvore, filme, t);
    if (!respBusca)
        return arvore;
    free(respBusca);
}