#include <iostream>
#include <time.h>
#include <chrono>

#define RED 1
#define BLACK 0

using namespace std;
using namespace std::chrono;

struct NO
{
    int info;
    struct NO *esq;
    struct NO *dir;
    int cor;
};
typedef struct NO *ArvLLRB;

int wentDownOnInsertion = 0; 
int wentDownOnSearch = 0; 
int wentDownOnDelete = 0; 
int rightRotations = 0; 
int leftRotations = 0; 

ArvLLRB *cria_ArvLLRB() {
    ArvLLRB *raiz = (ArvLLRB *)malloc(sizeof(ArvLLRB));
    if (raiz != NULL) {
        *raiz = NULL;
    }
    
    return raiz;
}

void libera_NO(struct NO *no) {
    if (no == NULL) {
        return;
    }
    libera_NO(no->esq);
    libera_NO(no->dir);
    free(no);
    no = NULL;
}

void libera_ArvLLRB(ArvLLRB *raiz) {
    if (raiz == NULL) {
        return;
    }
    libera_NO(*raiz);
    free(raiz);
}

int consulta_ArvLLRB(ArvLLRB *raiz, int valor) {
    if (raiz == NULL) {
        return 0;
    }

    struct NO *atual = *raiz;
    
    while (atual != NULL) {
        if (valor == atual->info) {
            return 1;
        }

        if (valor > atual->info) {
            wentDownOnSearch++;
            atual = atual->dir;
        } else {
            wentDownOnSearch++;
            atual = atual->esq;
        }
    }

    return 0;
}

struct NO *rotacionaEsquerda(struct NO *A) {
    leftRotations++;
    struct NO *B = A->dir;
    A->dir = B->esq;
    B->esq = A;
    B->cor = A->cor;
    A->cor = RED;
    return B;
}

struct NO *rotacionaDireita(struct NO *A) {
    rightRotations++;
    struct NO *B = A->esq;
    A->esq = B->dir;
    B->dir = A;
    B->cor = A->cor;
    A->cor = RED;
    return B;
}

int cor(struct NO *H) {
    if (H == NULL) {
        return BLACK;
    } else {
        return H->cor;
    }
}

void trocaCor(struct NO *H) {
    H->cor = !H->cor;

    if (H->esq != NULL) {
        H->esq->cor = !H->esq->cor;
    }

    if (H->dir != NULL) {
        H->dir->cor = !H->dir->cor;
    }
}

struct NO *insereNO(struct NO *H, int valor, int *resp) {
    if (H == NULL) {
        struct NO *novo;
        novo = (struct NO *)malloc(sizeof(struct NO));

        if (novo == NULL) {
            *resp = 0;
            return NULL;
        }

        novo->info = valor;
        novo->cor = RED;
        novo->dir = NULL;
        novo->esq = NULL;
        *resp = 1;
        return novo;
    }

    if (valor == H->info) {
        *resp = 0;
    } else {
        if (valor < H->info) {
            wentDownOnInsertion++;
            H->esq = insereNO(H->esq, valor, resp);
        } else {
            wentDownOnInsertion++;
            H->dir = insereNO(H->dir, valor, resp);
        }
    }

    if (cor(H->dir) == RED && cor(H->esq) == BLACK) {
        H = rotacionaEsquerda(H);
    }

    if (cor(H->esq) == RED && cor(H->esq->esq) == RED) {
        H = rotacionaDireita(H);
    }

    if (cor(H->esq) == RED && cor(H->dir) == RED) {
        trocaCor(H);
    }

    return H;
}

int insere_ArvLLRB(ArvLLRB *raiz, int valor) {
    int resp;

    *raiz = insereNO(*raiz, valor, &resp);
    if ((*raiz) != NULL) {
        (*raiz)->cor = BLACK;
    }

    return resp;
}

struct NO *balancear(struct NO *H) {
    if (cor(H->dir) == RED) {
        H = rotacionaEsquerda(H);
    }

    if (H->esq != NULL && cor(H->dir) == RED && cor(H->esq->esq) == RED) {
        H = rotacionaDireita(H);
    }

    if (cor(H->esq) == RED && cor(H->dir) == RED) {
        trocaCor(H);
    }

    return H;
}

struct NO *move2EsqRED(struct NO *H) {
    trocaCor(H);

    if (cor(H->dir->esq) == RED) {
        H->dir = rotacionaDireita(H->dir);
        H = rotacionaEsquerda(H);
        trocaCor(H);
    }

    return H;
}

struct NO *move2DirRED(struct NO *H) {
    trocaCor(H);

    if (cor(H->esq->esq) == RED) {
        H = rotacionaDireita(H);
        trocaCor(H);
    }

    return H;
}

struct NO *removerMenor(struct NO *H) {
    if (H->esq == NULL) {
        free(H);
        return NULL;
    }

    if (cor(H->esq) == BLACK && cor(H->esq->esq) == BLACK) {
        H = move2EsqRED(H);
    }

    H->esq = removerMenor(H->esq);
    return balancear(H);
}

struct NO *procuraMenor(struct NO *atual) {
    struct NO *no1 = atual;
    struct NO *no2 = atual->esq;

    while (no2 != NULL) {
        no1 = no2;
        no2 = no2->esq;
    }

    return no1;
}

struct NO *remove_NO(struct NO *H, int valor) {
    if (valor < H->info) {
        wentDownOnDelete++;
        if (cor(H->esq) == BLACK && cor(H->esq->esq) == BLACK) {
            H = move2EsqRED(H);
        }

        H->esq = remove_NO(H->esq, valor);
    } else {
        wentDownOnDelete++;
        if (cor(H->esq) == RED) {
            H = rotacionaDireita(H);
        }

        if (valor == H->info && (H->dir == NULL)) {
            free(H);
            return NULL;
        }

        if (cor(H->dir) == BLACK && cor(H->dir->esq) == BLACK) {
            H = move2DirRED(H);
        }

        if (valor == H->info) {
            struct NO *x = procuraMenor(H->dir);
            H->info = x->info;
            H->dir = removerMenor(H->dir);
        } else {
            H->dir = remove_NO(H->dir, valor);
        }
    }

    return balancear(H);
}

int remove_ArvLLRB(ArvLLRB *raiz, int valor) {
    if (consulta_ArvLLRB(raiz, valor)) {
        struct NO *h = *raiz;
        *raiz = remove_NO(h, valor);

        if (*raiz != NULL) {
            (*raiz)->cor = BLACK;
        }

        return 1;
    } else {
        return 0;
    }
}

int main() {
    auto start = high_resolution_clock::now();

    srand(time(NULL));

    ArvLLRB* root = cria_ArvLLRB();

    int treeSize = 100;
    for (int i = 0 ; i < treeSize ; i++) {
       int randomNumber = rand() % 100000 + 1;
       insere_ArvLLRB(root, randomNumber);
    }

    int randomNumberToSearch = rand() % 100000 + 1;
    consulta_ArvLLRB(root, randomNumberToSearch);
    
    int randomNumberToDelete = rand() % 100000 + 1;
    remove_ArvLLRB(root, randomNumberToDelete);

    cout << "Quantidade de vezes que desceu na insercao: " << wentDownOnInsertion;
    cout << "\nQuantidade de vezes que desceu na busca: " << wentDownOnSearch;
    cout << "\nQuantidade de vezes que desceu na remocao: " << wentDownOnDelete;
    cout << "\nQuantidade rotacoes a direita: " << rightRotations;
    cout << "\nQuantidade rotacoes a esquerda: " << leftRotations;
    
    libera_ArvLLRB(root);

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);

    cout << "\nTempo de execucao: " << duration.count() << " microsegundos";

    return 0;
}