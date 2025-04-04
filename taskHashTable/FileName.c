#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char* denumire;
    int anConstructie;
    int nrEtaje;
} Cladire;

typedef struct Nod {
    Cladire cladire;
    struct Nod* next;
} Nod;

typedef struct {
    Nod** vectorListe;
    int dim;
} HashTable;


HashTable initializareHashTable(int dim) {
    HashTable ht;
    ht.dim = dim;
    ht.vectorListe = (Nod**)malloc(sizeof(Nod*) * dim);
    for (int i = 0; i < dim; i++)
        ht.vectorListe[i] = NULL;
    return ht;
}


int hashFunction(int anConstructie, int dim) {
    return anConstructie % dim;
}


void inserareCladire(HashTable ht, Cladire c) {
    int poz = hashFunction(c.anConstructie, ht.dim);
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->cladire.id = c.id;
    nou->cladire.nrEtaje = c.nrEtaje;
    nou->cladire.anConstructie = c.anConstructie;
    nou->cladire.denumire = (char*)malloc(strlen(c.denumire) + 1);
    strcpy(nou->cladire.denumire, c.denumire);
    nou->next = ht.vectorListe[poz];
    ht.vectorListe[poz] = nou;
}


void afisareCladiriDinCluster(HashTable ht, int an) {
    int poz = hashFunction(an, ht.dim);
    Nod* p = ht.vectorListe[poz];
    printf("Cladiri din anul %d:\n", an);
    while (p) {
        if (p->cladire.anConstructie == an)
            printf("  ID: %d | Denumire: %s | Etaje: %d\n", p->cladire.id, p->cladire.denumire, p->cladire.nrEtaje);
        p = p->next;
    }
}


void stergeCladireCuAn(HashTable* ht, int id, int an) {
    int poz = hashFunction(an, ht->dim);
    Nod* p = ht->vectorListe[poz];
    Nod* prev = NULL;

    while (p) {
        if (p->cladire.id == id && p->cladire.anConstructie == an) {
            if (prev)
                prev->next = p->next;
            else
                ht->vectorListe[poz] = p->next;
            free(p->cladire.denumire);
            free(p);
            printf("Cladirea cu ID %d a fost stearsa din clusterul %d.\n", id, an);
            return;
        }
        prev = p;
        p = p->next;
    }
}


void stergeCladireDoarID(HashTable* ht, int id) {
    for (int i = 0; i < ht->dim; i++) {
        Nod* p = ht->vectorListe[i];
        Nod* prev = NULL;

        while (p) {
            if (p->cladire.id == id) {
                if (prev)
                    prev->next = p->next;
                else
                    ht->vectorListe[i] = p->next;

                free(p->cladire.denumire);
                free(p);
                printf("Cladirea cu ID %d a fost stearsa din clusterul %d.\n", id, i);
                return;
            }
            prev = p;
            p = p->next;
        }
    }
}

Cladire* vectorCladiriAn(HashTable ht, int an, int* nr) {
    *nr = 0;
    int poz = hashFunction(an, ht.dim);
    Nod* p = ht.vectorListe[poz];

    while (p) {
        if (p->cladire.anConstructie == an)
            (*nr)++;
        p = p->next;
    }

    if (*nr == 0) return NULL;

    Cladire* vector = (Cladire*)malloc(sizeof(Cladire) * (*nr));
    int i = 0;
    p = ht.vectorListe[poz];
    while (p) {
        if (p->cladire.anConstructie == an) {
            vector[i].id = p->cladire.id;
            vector[i].nrEtaje = p->cladire.nrEtaje;
            vector[i].anConstructie = an;
            vector[i].denumire = (char*)malloc(strlen(p->cladire.denumire) + 1);
            strcpy(vector[i].denumire, p->cladire.denumire);
            i++;
        }
        p = p->next;
    }
    return vector;
}


void modificaAnConstructie(HashTable* ht, int id, int vechiAn, int nouAn) {
    int poz = hashFunction(vechiAn, ht->dim);
    Nod* p = ht->vectorListe[poz];
    Nod* prev = NULL;

    while (p) {
        if (p->cladire.id == id) {
            if (prev)
                prev->next = p->next;
            else
                ht->vectorListe[poz] = p->next;

            p->cladire.anConstructie = nouAn;
            int nouPoz = hashFunction(nouAn, ht->dim);
            p->next = ht->vectorListe[nouPoz];
            ht->vectorListe[nouPoz] = p;

            printf("Anul pentru cladirea cu ID %d a fost modificat în %d.\n", id, nouAn);
            return;
        }
        prev = p;
        p = p->next;
    }
}


int main() {
    HashTable ht = initializareHashTable(10);

   
    Cladire c1 = { 1, "Bloc A", 1980, 10 };
    Cladire c2 = { 2, "Casa B", 1995, 1 };
    Cladire c3 = { 3, "Turn C", 1977, 20 };
    Cladire c4 = { 4, "Vila D", 1980, 2 };

    inserareCladire(ht, c1);
    inserareCladire(ht, c2);
    inserareCladire(ht, c3);
    inserareCladire(ht, c4);

    printf("\n--- Afisare cluster 1980 ---\n");
    afisareCladiriDinCluster(ht, 1980);

    printf("\n--- Stergere cu ID și an ---\n");
    stergeCladireCuAn(&ht, 4, 1980);
    afisareCladiriDinCluster(ht, 1980);

    printf("\n--- Stergere doar cu ID ---\n");
    stergeCladireDoarID(&ht, 3);

    printf("\n--- Deep copy in vector (1980) ---\n");
    int nr = 0;
    Cladire* vec = vectorCladiriAn(ht, 1980, &nr);
    for (int i = 0; i < nr; i++) {
        printf("Cladire vector: %s (%d)\n", vec[i].denumire, vec[i].anConstructie);
        free(vec[i].denumire);
    }
    free(vec);

    printf("\n--- Modificare an pentru ID 2 ---\n");
    modificaAnConstructie(&ht, 2, 1995, 1985);
    afisareCladiriDinCluster(ht, 1985);

    return 0;
}
