// Gustavo Rocha Barros 2310505
// Maria Luiza Dutra Gonzalez de Almeida 2310509
#include <stdio.h>
#include <stdlib.h>

struct arvbin
{ // criação do struct para árvore binária, contendo as informações para os nós (chave e ponteiros para filhos)
    int info;
    int qtd_nos;
    int altura;
    struct arvbin *esq;
    struct arvbin *dir;
};
typedef struct arvbin Arvbin;

Arvbin *cria_arvore();
Arvbin *cria_no(int info, int qtd_nos, int altura);
void insere_no(Arvbin **arvore, int info);
Arvbin *insere_alt(Arvbin *arvore);
Arvbin *conta_nos(Arvbin *arvore);
int verifica_ABB(Arvbin *arvore);
int verifica_val(Arvbin *no, int val);
void verifica_AVL(Arvbin *arvore);
void imprime_arvore(Arvbin *arvore);

Arvbin *cria_arvore()
{ // inicialização da árvore binária como vazia
    return NULL;
}

Arvbin *cria_no(int info, int qtd_nos, int altura)
{ // criação de um nó da árvore binária, contendo a chave e dois ponteiros para seus filhos à esquerda e direita
    Arvbin *no = (Arvbin *)malloc(sizeof(Arvbin));

    if (!no)
    {
        printf("Ocorreu um erro ao inicializar o nó!\n\n.");
        exit(1);
    }

    no->info = info;
    no->qtd_nos = qtd_nos;
    no->altura = altura;
    no->dir = NULL;
    no->esq = NULL;

    return no;
}

void insere_no(Arvbin **arvore, int info)
{                                                             // inserção do nó na árvore binária
    Arvbin **fila = (Arvbin **)malloc(40 * sizeof(Arvbin *)); // criação da fila de nós (FIFO)

    if (fila == NULL)
    {
        printf("Ocorreu um erro ao criar a fila.\n");
        exit(1);
    }

    int inicio = 0; // para marcar o início da fila
    int final = 0;  // para marcar o final da fila

    if (*arvore == NULL) // caso a árvore esteja vazia inicialmente
    {
        *arvore = cria_no(info, 0, 0); // aqui, a quantidade de nós das subárvores é dada como zero e a altura também é dada como zero por default (vai ser calculado depois)
        return;
    }

    fila[final++] = *arvore; // inserimos a raiz na fila

    while (inicio < final)
    {
        Arvbin *no = fila[inicio++]; // nó temporário que irá percorrer a fila do início

        if (no->esq == NULL)
        {                                  // caso o filho à esquerda do nó esteja vazio, é inserido o novo nó
            no->esq = cria_no(info, 0, 0); // para criar um novo nó, são passados sua chave e a quantidade de nós das subárvores como zero por default (vai ser inserida depois)
            break;
        }
        else
        { // caso o filho à esquerda do nó não esteja vazio, ele é inserido ao final da fila
            fila[final++] = no->esq;
        }

        if (no->dir == NULL)
        {                                  // caso o filho à direita do nó esteja vazio, é inserido o novo nó
            no->dir = cria_no(info, 0, 0); // para criar um novo nó, são passados sua chave e a quantidade de nós das subárvores como zero por default (vai ser inserida depois)
            break;
        }
        else
        { // caso o filho à direita do nó não esteja vazio, ele é inserido ao final da fila
            fila[final++] = no->dir;
        }
    }
    free(fila);
    *arvore = insere_alt(*arvore); // aqui é onde a altura de cada nó é inserida
}

Arvbin *insere_alt(Arvbin *arvore) // inserção das alturas na árvore
{
    if (arvore == NULL)
    { // condição base da recursão e retorno de árvore vazia
        return NULL;
    }

    arvore->esq = insere_alt(arvore->esq); // recursão para que todos os nós sejam acessados
    arvore->dir = insere_alt(arvore->dir);

    if (arvore->esq == NULL && arvore->dir == NULL)
    { // caso o nó não tenha filhos, a altura dele é 0
        arvore->altura = 0;
        return arvore;
    }

    int alt_esq, alt_dir;
    alt_dir = alt_esq = 0; // as alturas são inicializadas como 0
    if (arvore->esq != NULL)
    { // caso o nó da direção analizada não seja nulo, sua altura é salva
        alt_esq = arvore->esq->altura;
    }
    if (arvore->dir != NULL)
    {
        alt_dir = arvore->dir->altura;
    }

    if (alt_dir > alt_esq)
    { // a maior altura é selecionada e adicionada 1 para ser a altura do nó
        arvore->altura = alt_dir + 1;
    }
    else
    {
        arvore->altura = alt_esq + 1;
    }

    return arvore;
}

Arvbin *conta_nos(Arvbin *arvore)
{ // para contar a quantidade de nós nas subárvores abaixo de um determinado nó
    int qtd_dir, qtd_esq;

    if (arvore == NULL)
    { // condição base da recursão e retorno de árvore vazia
        return NULL;
    }

    arvore->dir = conta_nos(arvore->dir); // recursão para acessar todos os nós
    arvore->esq = conta_nos(arvore->esq);

    if (arvore->dir == NULL)
    { // caso não tenha nós à direita, sua quantidade é 0
        qtd_dir = 0;
    }
    else
    { // caso tenha, a quantidade de nós é a do nó a direita mais 1 para representar o próprio nó à direita
        qtd_dir = arvore->dir->qtd_nos + 1;
    }

    if (arvore->esq == NULL)
    { // segue a mesma lógica dos nós à direita
        qtd_esq = 0;
    }
    else
    {
        qtd_esq = arvore->esq->qtd_nos + 1;
    }
    arvore->qtd_nos = qtd_dir + qtd_esq; // a quantidade de nós abaixo dele é a soma dos à esqueda e à direita

    return arvore;
}

int verifica_ABB(Arvbin *arvore)
{                                                             // verifica se a árvore é ABB
    Arvbin **fila = (Arvbin **)malloc(40 * sizeof(Arvbin *)); // criação da fila de nós (FIFO)

    if (fila == NULL)
    {
        printf("Ocorreu um erro ao criar a fila.\n");
        exit(1);
    }

    int inicio = 0; // para marcar o início da fila
    int final = 0;  // para marcar o final da fila

    if (arvore == NULL) // caso a árvore esteja vazia inicialmente
    {
        return 2;
    }

    fila[final++] = arvore; // inserimos a raiz na fila

    while (inicio < final)
    {
        Arvbin *no = fila[inicio++]; // nó temporário que irá percorrer a fila do início

        if (verifica_val(arvore, no->info) == 0)
        { // verifica se o valor do nó atual não se encontra nos padrões de árvore ABB
            free(fila);
            return 0;
        }

        if (no->esq != NULL)
        { // caso o filho à esquerda do nó não esteja vazio, ele é inserido ao final da fila
            fila[final++] = no->esq;
        }

        if (no->dir != NULL)
        { // caso o filho à direita do nó não esteja vazio, ele é inserido ao final da fila
            fila[final++] = no->dir;
        }
    }
    free(fila);
    return 1;
}

int verifica_val(Arvbin *no, int val)
{ // função auxiliar para comparar todos os nós das subárvores da direita e da esquerda de um certo nó
    if (no == NULL)
    { // ao chegar em um nó nulo, retorna-se zero
        return 0;
    }
    if (no->info == val)
    { // ao chegar no nó desejado, retorna-se ele mesmo
        return val;
    }

    int aux_dir, aux_esq;
    aux_esq = verifica_val(no->esq, val); // a pós-ordem é utilizada para acessar os nós da árvore e retornar um valor
    aux_dir = verifica_val(no->dir, val);

    if (aux_esq == val && no->info > aux_esq)
    {                   // caso aux_esq seja igual a val, ele é o número do nó que queremos analisar. assim, analisamos se ele é menor que o nó atual
        return aux_esq; // retornamos o valor do nó analisado caso ele esteja nos padrões da árvore ABB
    }
    if (aux_dir == val && no->info < aux_dir)
    { // mesma lógica do que a análise à esquerda
        return aux_dir;
    }
    return 0; // caso chegue aqui, o nó não foi encontrado ou não se aplica nos padrões da árvore ABB, retornando 0
}

void verifica_AVL(Arvbin *arvore)
{                                                             // verifica se a árvore é AVL
    Arvbin **fila = (Arvbin **)malloc(40 * sizeof(Arvbin *)); // criação da fila de nós (FIFO)

    if (fila == NULL)
    {
        printf("Ocorreu um erro ao criar a fila.\n");
        exit(1);
    }

    int inicio = 0; // para marcar o início da fila
    int final = 0;  // para marcar o final da fila
    int dif_altura = 0;
    int alt_esq = 0;
    int alt_dir = 0;

    if (arvore == NULL) // caso a árvore esteja vazia inicialmente
    {
        printf("\nA árvore não existe! Verifique se ela foi inicializada corretamente\n");
        return;
    }

    if (verifica_ABB(arvore) == 0) // caso a árvore não seja ABB
    {
        printf("\nA árvore não é uma árvore binária de busca, logo ela não pode ser AVL!\n");
        free(fila);
        return;
    }

    fila[final++] = arvore; // inserimos a raiz na fila

    while (inicio < final) // utilizamos um loop para verificar se a árvore é balanceada
    {
        Arvbin *no = fila[inicio++]; // nó temporário que irá percorrer a fila do início

        if (no->esq == NULL) // caso não tenha nós à esquerda, a altura será 0
        {
            alt_esq = 0;
        }
        else
        { // caso tenha, a altura será a do respectivo nó
            alt_esq = no->esq->altura;
            fila[final++] = no->esq;
        }

        if (no->dir == NULL) // segue a mesma lógica do que a análise para a esquerda
        {
            alt_dir = 0;
        }
        else
        {
            alt_dir = no->dir->altura;
            fila[final++] = no->dir;
        }

        dif_altura = alt_dir - alt_esq; // a diferença de alturas determina se a árvore está balanceada

        if (abs(dif_altura) > 1) // se ela não estiver balanceada, altura em módulo maior que 1, ela não é AVL
        {
            free(fila);
            printf("\nA árvore não é AVL!\n");
            return;
        }
    }
    free(fila);
    printf("\nA árvore é AVL!\n");
}

void imprime_arvore(Arvbin *arvore)
{ // impressão de todos os nós da árvore em modelo simétrico
    if (arvore != NULL)
    {
        imprime_arvore(arvore->esq); // prioridade para os nós da esquerda

        printf("%d -- qtd: %d\n", arvore->info, arvore->qtd_nos);

        imprime_arvore(arvore->dir); // chamada dos nós da direita, após os da esquerda e da raiz
    }
}

int main(void)
{
    int vetor[10] = {15, 17, 3, 5, 2, 20, 25, 13, 10, 16};
    Arvbin *arvoreBi = cria_arvore();

    for (int i = 0; i < 10; i++)
    {
        insere_no(&arvoreBi, vetor[i]); // realiza a inserção dos nós na árvore
    }

    imprime_arvore(arvoreBi); // realiza o algoritmo de listagem dos nós
    putchar('\n');
    arvoreBi = conta_nos(arvoreBi);
    imprime_arvore(arvoreBi);
    putchar('\n');

    if (verifica_ABB(arvoreBi) == 0)
    {
        printf("\nA árvore não é uma árvore binária de busca!\n");
    }
    else if (verifica_ABB(arvoreBi) == 1)
    {
        printf("\nA árvore é uma árvore binária de busca!\n");
    }
    else
    {
        printf("\nA árvore não existe! Verifique se ela foi inicializada corretamente\n");
    }

    verifica_AVL(arvoreBi);

    return 0;
}