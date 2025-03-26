#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Masina {
    int id;
    char* model;
    float pret;
} Masina;

typedef struct Nod {
    Masina info;
    struct Nod* next;
} Nod;

// Functie de creare masina
Masina creareMasina(int id, const char* model, float pret) {
    Masina m;
    m.id = id;
    m.model = (char*)malloc(strlen(model) + 1);
    strcpy(m.model, model);
    m.pret = pret;
    return m;
}

// Functie de afisare masina
void afisareMasina(Masina m) {
    printf("ID: %d | Model: %s | Pret: %.2f\n", m.id, m.model, m.pret);
}

// Inserare sortata crescator dupa pret
void inserareSortata(Nod** cap, Masina m) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = creareMasina(m.id, m.model, m.pret);
    nou->next = NULL;

    if (*cap == NULL || m.pret < (*cap)->info.pret) {
        nou->next = *cap;
        *cap = nou;
    }
    else {
        Nod* curent = *cap;
        while (curent->next && curent->next->info.pret < m.pret)
            curent = curent->next;
        nou->next = curent->next;
        curent->next = nou;
    }
}

// Afisare lista
void afisareLista(Nod* cap) {
    while (cap) {
        afisareMasina(cap->info);
        cap = cap->next;
    }
}

// 1. Stergere nod la o pozitie data
void stergereLaPozitie(Nod** cap, int pozitie) {
    if (*cap == NULL || pozitie < 0) return;

    Nod* temp = *cap;

    if (pozitie == 0) {
        *cap = temp->next;
        free(temp->info.model);
        free(temp);
        return;
    }

    for (int i = 0; i < pozitie - 1 && temp != NULL; i++)
        temp = temp->next;

    if (temp == NULL || temp->next == NULL) return;

    Nod* nodDeSters = temp->next;
    temp->next = nodDeSters->next;
    free(nodDeSters->info.model);
    free(nodDeSters);
}

// 2. Inserare sortata – deja implementata mai sus

// 3. Salvare in vector elemente care au pret > valoare
Masina* salveazaInVector(Nod* cap, int* dim, float prag) {
    *dim = 0;
    Nod* temp = cap;

    while (temp) {
        if (temp->info.pret > prag)
            (*dim)++;
        temp = temp->next;
    }

    if (*dim == 0) return NULL;

    Masina* vector = (Masina*)malloc(sizeof(Masina) * (*dim));
    int k = 0;
    temp = cap;

    while (temp) {
        if (temp->info.pret > prag) {
            vector[k] = creareMasina(temp->info.id, temp->info.model, temp->info.pret);
            k++;
        }
        temp = temp->next;
    }
    return vector;
}

// 4. Interschimbare elemente de pe doua pozitii
void interschimbarePozitii(Nod* cap, int p1, int p2) {
    if (p1 == p2) return;

    Nod* nod1 = NULL;
    Nod* nod2 = NULL;
    int index = 0;

    while (cap) {
        if (index == p1) nod1 = cap;
        if (index == p2) nod2 = cap;
        cap = cap->next;
        index++;
    }

    if (nod1 && nod2) {
        Masina temp = nod1->info;
        nod1->info = nod2->info;
        nod2->info = temp;
    }
}

// Dezalocare
void dezalocareLista(Nod** cap) {
    while (*cap) {
        Nod* temp = *cap;
        *cap = (*cap)->next;
        free(temp->info.model);
        free(temp);
    }
}

int main() {
    Nod* lista = NULL;

    inserareSortata(&lista, creareMasina(1, "Dacia", 5000));
    inserareSortata(&lista, creareMasina(2, "BMW", 20000));
    inserareSortata(&lista, creareMasina(3, "Audi", 15000));
    inserareSortata(&lista, creareMasina(4, "Mercedes", 25000));

    printf("Lista sortata:\n");
    afisareLista(lista);

    printf("\nStergem elementul de pe pozitia 2:\n");
    stergereLaPozitie(&lista, 2);
    afisareLista(lista);

    printf("\nElemente cu pret > 10000:\n");
    int dim = 0;
    Masina* v = salveazaInVector(lista, &dim, 10000);
    for (int i = 0; i < dim; i++) {
        afisareMasina(v[i]);
        free(v[i].model);
    }
    free(v);

    printf("\nInterschimbam pozitiile 0 si 1:\n");
    interschimbarePozitii(lista, 0, 1);
    afisareLista(lista);

    dezalocareLista(&lista);
    return 0;
}
