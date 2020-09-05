#include "linkedlist.h"

#define T 2

Filme *linhaCatalogoToFilme(char *linha)
{
    int i = 0, j = 0;
    Filme *novo = (Filme *)malloc(sizeof(Filme));
    while (linha[i] != '/')
    {
        novo->titulo[j] = linha[i];
        i++;
        j++;
    }
    i++;
    novo->titulo[j] = '\0';
    j = 0;
    char *ano = (char *)malloc(sizeof(char) * 5);
    while (linha[i] != '/')
    {
        ano[j] = linha[i];
        i++;
        j++;
    }
    i++;
    ano[j] = '\0';
    novo->ano = atoi(ano);
    free(ano);
    j = 0;
    while (linha[i] != '/')
    {
        novo->diretor[j] = linha[i];
        i++;
        j++;
    }
    novo->diretor[j] = '\0';
    i++;
    j = 0;
    while (linha[i] != '/')
    {
        novo->genero[j] = linha[i];
        i++;
        j++;
    }
    novo->genero[j] = '\0';
    i++;
    j = 0;
    char *duracao = (char *)malloc(sizeof(char) * 4);
    while (linha[i] != '\0')
    {
        duracao[j] = linha[i];
        j++;
        i++;
    }
    duracao[j] = '\0';
    i++;
    j = 0;
    novo->duracao = atoi(duracao);
    free(duracao);
    return novo;
}

char *processarCatalogo(char *arvore, int t, int *contadorDeNomes, char *arquivoCatalogo)
{
    char linhaArquivo[172];
    FILE *fp = fopen(arquivoCatalogo, "r");
    int contador = 0;
    while (fgets(linhaArquivo, 172, fp))
    {
        linhaArquivo[strlen(linhaArquivo) - 1] = '\0';
        Filme *f1 = linhaCatalogoToFilme(linhaArquivo);
        arvore = insereBTree(arvore, f1, t, contadorDeNomes);
        free(f1);
    }
    fclose(fp);
    return arvore;
}

Filme *consoleToFilme()
{
    char linha[172];
    char titulo[81];
    char diretor[51];
    char genero[31];
    int ano;
    int duracao;

    printf("  Digite o titulo do filme: ");
    fgets(titulo, 81, stdin);
    titulo[strlen(titulo) - 1] = '\0';

    printf("  Digite o ano de lancamento: ");
    scanf("%d", &ano);
    getchar();

    printf("  Digite o diretor: ");
    fgets(diretor, 51, stdin);
    diretor[strlen(diretor) - 1] = '\0';

    printf("  Digite o genero: ");
    fgets(genero, 31, stdin);
    genero[strlen(genero) - 1] = '\0';

    printf("  Digite a duracao em minutos: ");
    scanf("%d", &duracao);
    getchar();

    sprintf(linha, "%s/%04d/%s/%s/%03d", titulo, ano, diretor, genero, duracao);
    Filme *novoFilme = linhaCatalogoToFilme(linha);
    return novoFilme;
}

Filme *buscaInformacoesSubordinadas(char *pagina, Filme *filme, int t)
{
    BTree *a = readBTreeFromFile(pagina, t);
    int i = 0;
    char *chaveFilmeAtual = buildChavePrimaria(a->filmes[i]);
    char *chaveFilmeBuscado = buildChavePrimaria(filme);

    while (strcmp(chaveFilmeAtual, chaveFilmeBuscado) != 0)
    {
        i++;
        free(chaveFilmeAtual);
        chaveFilmeAtual = buildChavePrimaria(a->filmes[i]);
    }
    free(chaveFilmeAtual);
    free(chaveFilmeBuscado);

    strcpy(filme->diretor, a->filmes[i]->diretor);
    strcpy(filme->genero, a->filmes[i]->genero);
    filme->ano = a->filmes[i]->ano;

    liberaBTreeEmMP(a, t);
    return filme;
}

void alterarInformacoesSubordinadas(char *pagina, Filme *novoFilme, int t)
{
    BTree *a = readBTreeFromFile(pagina, t);
    int i = 0;
    char *chaveFilmeAtual = buildChavePrimaria(a->filmes[i]);
    char *chaveFilmeBuscado = buildChavePrimaria(novoFilme);

    while (strcmp(chaveFilmeAtual, chaveFilmeBuscado) != 0)
    {
        i++;
        free(chaveFilmeAtual);
        chaveFilmeAtual = buildChavePrimaria(a->filmes[i]);
    }
    free(chaveFilmeAtual);
    free(chaveFilmeBuscado);

    printf("  Digite o novo diretor: ");
    fgets(novoFilme->diretor, 51, stdin);
    novoFilme->diretor[strlen(novoFilme->diretor) - 1] = '\0';
    printf("  Digite o novo genero:  ");
    fgets(novoFilme->genero, 31, stdin);
    novoFilme->genero[strlen(novoFilme->genero) - 1] = '\0';
    printf("  Digite a nova duracao: ");
    scanf("%d", &(novoFilme->duracao));
    getchar();

    free(a->filmes[i]);
    a->filmes[i] = copiaFilme(novoFilme);
    writeBTreeToFile(pagina, a, t);
    liberaBTreeEmMP(a, t);
}

void buscaFilmesDeDiretor(char *nomearvore, char *diretor, ListaFilmes **lista, int t)
{
    BTree *a = readBTreeFromFile(nomearvore, t);
    int i;
    for (i = 0; i < a->nFilmes; i++)
    {
        if (!(a->folha))
        {
            buscaFilmesDeDiretor(a->filhos[i], diretor, lista, t);
        }
        if (strcmp(diretor, a->filmes[i]->diretor) == 0)
        {
            *lista = insereListaFilmes(copiaFilme(a->filmes[i]), *lista);
        }
    }
    if (!(a->folha))
        buscaFilmesDeDiretor(a->filhos[i], diretor, lista, t);
    liberaBTreeEmMP(a, t);
}

void buscaPorGenero(char *nomearvore, char *genero, ListaFilmes **lista, int t)
{
    BTree *a = readBTreeFromFile(nomearvore, t);
    int i;
    for (i = 0; i < a->nFilmes; i++)
    {
        if (!(a->folha))
        {
            buscaPorGenero(a->filhos[i], genero, lista, t);
        }
        if (strcmp(genero, a->filmes[i]->genero) == 0)
        {
            *lista = insereListaFilmes(copiaFilme(a->filmes[i]), *lista);
        }
    }
    if (!(a->folha))
        buscaPorGenero(a->filhos[i], genero, lista, t);
    liberaBTreeEmMP(a, t);
}

int main(int argc, char **argv)
{
    if (argc < 2)
        fatalError("parametros de execução do programa invalidos\nLembre de passar o fator de ramificação como argumento (argv)");

    int t = atoi(argv[1]), contadorDeNomes = 0;
    char catalogo[16], *arvore;

    printf("Digite o nome do catalogo de filmes (maximo 15 caracteres): ");
    fgets(catalogo, 16, stdin);
    catalogo[strlen(catalogo) - 1] = '\0';

    arvore = inicializaBTree();
    arvore = processarCatalogo(arvore, t, &contadorDeNomes, catalogo);

    int op = 0;
    char opChar[3];
    while (op != -1)
    {
        op = 0;
        printf("-----------------------------\n");
        printf("Digite:\n");
        printf("    1 para imprimir o catalogo\n");
        printf("    2 para inserir um filme\n");
        printf("    3 para remover um filme do catalogo  -  NOT IMPLEMENTED\n");
        printf("    4 para buscar informacoes subordinadas\n");
        printf("    5 para alterar as informacoes subordinadas\n");
        printf("    6 para buscar todos filmes de um diretor\n");
        printf("    7 para buscar todos os filmes de uma categoria\n");
        printf("    8 para remover todos os filmes de uma categoria  -  NOT IMPLEMENTED\n");
        printf("   -1 para sair\n");
        printf("opcao: ");
        scanf("%d", &op);
        getchar();
        printf("-----------------------------\n");

        if (op == 1)
        {
            int contadorIndice = 0;
            imprimirBTree(arvore, t, &contadorIndice);
            printf("\n\n");
        }
        else if (op == 2)
        {
            // printf("\tLogica para leitura do filme do console\n");
            printf("Inserindo um novo filme:\n");
            Filme *novo = consoleToFilme();
            arvore = insereBTree(arvore, novo, t, &contadorDeNomes);
        }
        else if (op == 3)
        {
            printf("    Logica para remocao de filmes do catalogo ainda nao implementada\n");
        }
        else if (op == 4)
        {
            Filme *filmeInserido = (Filme *)malloc(sizeof(Filme));
            printf("Digite o titulo: ");
            fgets(filmeInserido->titulo, 81, stdin);
            filmeInserido->titulo[strlen(filmeInserido->titulo) - 1] = '\0';
            printf("Digite o ano: ");
            scanf("%d", &(filmeInserido->ano));
            getchar();

            char *noDoFilme = buscaBTree(arvore, filmeInserido, t);
            if (noDoFilme)
            {
                filmeInserido = buscaInformacoesSubordinadas(noDoFilme, filmeInserido, t);
                printf("Informacoes subordinadas de %s:\n", filmeInserido->titulo);
                printf("    Diretor:   %s\n", filmeInserido->diretor);
                printf("    Categoria: %s\n", filmeInserido->genero);
                printf("    duracao:   %d\n", filmeInserido->duracao);
                free(noDoFilme);
            }
            else
                printf("nao existe um filme com essa chave primaria no catalogo\n");
            free(filmeInserido);
        }
        else if (op == 5)
        {
            Filme *filmeInserido = (Filme *)malloc(sizeof(Filme));
            printf("Digite o titulo: ");
            fgets(filmeInserido->titulo, 81, stdin);
            filmeInserido->titulo[strlen(filmeInserido->titulo) - 1] = '\0';
            printf("Digite o ano: ");
            scanf("%d", &(filmeInserido->ano));
            getchar();

            char *noDoFilme = buscaBTree(arvore, filmeInserido, t);
            if (noDoFilme)
            {
                alterarInformacoesSubordinadas(noDoFilme, filmeInserido, t);
            }
            else
                printf("nao existe um filme com essa chave primaria no catalogo\n");
            free(filmeInserido);
        }
        else if (op == 6)
        {
            char diretor[51];
            printf("Digite um diretor: ");
            fgets(diretor, 51, stdin);
            diretor[strlen(diretor) - 1] = '\0';

            ListaFilmes *filmesDesseDiretor = inicializaListaFilmes();
            buscaFilmesDeDiretor(arvore, diretor, &filmesDesseDiretor, t);

            printf("\n-----------------------------\n");
            printf("Filmes do diretor %s:\n\n", diretor);
            Filme *popFilme = popInicioListaFilmes(&filmesDesseDiretor);
            while (popFilme)
            {
                printf("  %s\n", popFilme->titulo);
                printf("      %d | %s | %s | %d\n\n", popFilme->ano, popFilme->diretor, popFilme->genero, popFilme->duracao);
                free(popFilme);
                popFilme = popInicioListaFilmes(&filmesDesseDiretor);
            }
            free(filmesDesseDiretor);
        }
        else if (op == 7)
        {
            char genero[31];
            printf("Digite uma categoria: ");
            fgets(genero, 31, stdin);
            genero[strlen(genero) - 1] = '\0';

            ListaFilmes *filmesDesseGenero = inicializaListaFilmes();
            buscaPorGenero(arvore, genero, &filmesDesseGenero, t);

            printf("\n-----------------------------\n");
            printf("Filmes da categoria %s:\n\n", genero);
            Filme *popFilme = popInicioListaFilmes(&filmesDesseGenero);
            while (popFilme)
            {
                printf("  %s\n", popFilme->titulo);
                printf("      %d | %s | %s | %d\n\n", popFilme->ano, popFilme->diretor, popFilme->genero, popFilme->duracao);
                free(popFilme);
                popFilme = popInicioListaFilmes(&filmesDesseGenero);
            }
            free(filmesDesseGenero);
        }

        else if (op == 8)
        {
            printf("    Logica para remocao de filmes da arvore B ainda nao implementada\n");
        }
    }

    liberaBTree(&contadorDeNomes);

    return 0;
}