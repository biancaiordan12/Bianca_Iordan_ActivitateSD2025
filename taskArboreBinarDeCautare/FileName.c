#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Cladire {
    int id;
    char* denumire;
    int anConstructie;
    struct Cladire* next;
} Cladire;

int hash(int an, int dim) {
    return an % dim;
}

void inserareCladire(Cladire** tabela, int dim, Cladire c) {
    int poz = hash(c.anConstructie, dim);
    Cladire* nou = (Cladire*)malloc(sizeof(Cladire));
    nou->id = c.id;
    nou->anConstructie = c.anConstructie;
    nou->denumire = (char*)malloc(strlen(c.denumire) + 1);
    strcpy(nou->denumire, c.denumire);
    nou->next = tabela[poz];
    tabela[poz] = nou;
}

void citireCladiriDinFisier(Cladire** tabela, int dim, const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    if (!f) {
        printf("Eroare la deschiderea fisierului!\n");
        return;
    }

    char linie[128];
    while (fgets(linie, sizeof(linie), f)) {
        Cladire c;
        char* token = strtok(linie, ",\n");
        c.id = atoi(token);

        token = strtok(NULL, ",\n");
        c.denumire = (char*)malloc(strlen(token) + 1);
        strcpy(c.denumire, token);

        token = strtok(NULL, ",\n");
        c.anConstructie = atoi(token);

        c.next = NULL;
        inserareCladire(tabela, dim, c);
        free(c.denumire);  
    }

    fclose(f);
}

void modificaAnConstructie(Cladire** tabela, int dim, int id, int anVechi, int anNou) {
    int poz = hash(anVechi, dim);
    Cladire* prev = NULL;
    Cladire* curent = tabela[poz];

    while (curent) {
        if (curent->id == id) {
            if (prev)
                prev->next = curent->next;
            else
                tabela[poz] = curent->next;

            curent->anConstructie = anNou;
            int pozNoua = hash(anNou, dim);
            curent->next = tabela[pozNoua];
            tabela[pozNoua] = curent;
            return;
        }
        prev = curent;
        curent = curent->next;
    }
}


typedef struct {
    char* nume;
    int varsta;
    int urgenta;
} Pacient;

typedef struct {
    Pacient* vector;
    int dim;
    int capacitate;
} Heap;

void swap(Pacient* a, Pacient* b) {
    Pacient temp = *a;
    *a = *b;
    *b = temp;
}

void inserareHeap(Heap* h, Pacient p) {
    if (h->dim == h->capacitate) return;
    h->vector[h->dim] = p;
    int i = h->dim++;
    while (i > 0 && h->vector[(i - 1) / 2].urgenta < h->vector[i].urgenta) {
        swap(&h->vector[i], &h->vector[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

Pacient extrageMax(Heap* h) {
    Pacient p = { NULL, 0, 0 };
    if (h->dim == 0) return p;

    p = h->vector[0];
    h->vector[0] = h->vector[--h->dim];
    int i = 0;

    while (2 * i + 1 < h->dim) {
        int maxIndex = i, st = 2 * i + 1, dr = 2 * i + 2;
        if (st < h->dim && h->vector[st].urgenta > h->vector[maxIndex].urgenta)
            maxIndex = st;
        if (dr < h->dim && h->vector[dr].urgenta > h->vector[maxIndex].urgenta)
            maxIndex = dr;
        if (maxIndex != i) {
            swap(&h->vector[i], &h->vector[maxIndex]);
            i = maxIndex;
        }
        else break;
    }

    return p;
}


typedef struct Nod {
    int id;
    struct Nod* st;
    struct Nod* dr;
} Nod;

Nod* inserareNod(Nod* rad, int id) {
    if (!rad) {
        Nod* nou = (Nod*)malloc(sizeof(Nod));
        nou->id = id;
        nou->st = nou->dr = NULL;
        return nou;
    }
    if (id < rad->id)
        rad->st = inserareNod(rad->st, id);
    else
        rad->dr = inserareNod(rad->dr, id);
    return rad;
}

int inaltime(Nod* rad) {
    if (!rad) return 0;
    int st = inaltime(rad->st);
    int dr = inaltime(rad->dr);
    return 1 + (st > dr ? st : dr);
}

int nrNoduri(Nod* rad) {
    if (!rad) return 0;
    return 1 + nrNoduri(rad->st) + nrNoduri(rad->dr);
}

Nod* subarboreMaiInalt(Nod* rad) {
    if (!rad) return NULL;
    return (inaltime(rad->st) > inaltime(rad->dr)) ? rad->st : rad->dr;
}

Nod* subarboreMaiMultiNoduri(Nod* rad) {
    if (!rad) return NULL;
    return (nrNoduri(rad->st) > nrNoduri(rad->dr)) ? rad->st : rad->dr;
}

Nod* stergeNod(Nod* rad, int id) {
    if (!rad) return NULL;
    if (id < rad->id) rad->st = stergeNod(rad->st, id);
    else if (id > rad->id) rad->dr = stergeNod(rad->dr, id);
    else {
        if (!rad->st) {
            Nod* temp = rad->dr;
            free(rad);
            return temp;
        }
        else if (!rad->dr) {
            Nod* temp = rad->st;
            free(rad);
            return temp;
        }
        Nod* succ = rad->dr;
        while (succ->st) succ = succ->st;
        rad->id = succ->id;
        rad->dr = stergeNod(rad->dr, succ->id);
    }
    return rad;
}

void afisareInordine(Nod* rad) {
    if (rad) {
        afisareInordine(rad->st);
        printf("%d ", rad->id);
        afisareInordine(rad->dr);
    }
}


int main() {
    int dim = 5;
    Cladire** tabela = (Cladire**)calloc(dim, sizeof(Cladire*));
    citireCladiriDinFisier(tabela, dim, "cladiri.txt");
    modificaAnConstructie(tabela, dim, 1, 1980, 2000);

    Heap h;
    h.capacitate = 10;
    h.vector = (Pacient*)malloc(sizeof(Pacient) * h.capacitate);
    h.dim = 0;
    Pacient p1 = { "Ion", 30, 2 };
    Pacient p2 = { "Maria", 40, 5 };
    Pacient p3 = { "George", 25, 3 };
    inserareHeap(&h, p1);
    inserareHeap(&h, p2);
    inserareHeap(&h, p3);
    Pacient p = extrageMax(&h);
    printf("\nPrimul pacient tratat: %s\n", p.nume);

    Nod* arbore = NULL;
    arbore = inserareNod(arbore, 5);
    arbore = inserareNod(arbore, 3);
    arbore = inserareNod(arbore, 7);
    arbore = stergeNod(arbore, 3);
    printf("\nParcurgere inordine arbore: ");
    afisareInordine(arbore);
    printf("\nÎnălțime subarbore dominant: %d\n", inaltime(subarboreMaiInalt(arbore)));
    printf("Nr. noduri subarbore dominant: %d\n", nrNoduri(subarboreMaiMultiNoduri(arbore)));

    return 0;
}
