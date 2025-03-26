#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
    int id;
    int nrUsi;
    float pret;
    char* model;
    char* numeSofer;
    unsigned char serie;
};
typedef struct StructuraMasina Masina;

typedef struct Nod {
    Masina masina;
    struct Nod* next;
    struct Nod* prev;
} Nod;

typedef struct ListaDubla {
    Nod* head;
    Nod* tail;
} ListaDubla;

Masina citireMasinaDinFisier(FILE* file) {
    char buffer[100];
    char sep[3] = ",\n";
    if (fgets(buffer, 100, file) == NULL) {
        Masina m = { 0, 0, 0, NULL, NULL, 0 };
        return m;
    }
    char* aux;
    Masina m1;
    aux = strtok(buffer, sep);
    m1.id = atoi(aux);
    m1.nrUsi = atoi(strtok(NULL, sep));
    m1.pret = atof(strtok(NULL, sep));
    aux = strtok(NULL, sep);
    m1.model = malloc(strlen(aux) + 1);
    strcpy(m1.model, aux);
    aux = strtok(NULL, sep);
    m1.numeSofer = malloc(strlen(aux) + 1);
    strcpy(m1.numeSofer, aux);
    m1.serie = *strtok(NULL, sep);
    return m1;
}

void afisareMasina(Masina masina) {
    printf("Id: %d\n", masina.id);
    printf("Nr. usi : %d\n", masina.nrUsi);
    printf("Pret: %.2f\n", masina.pret);
    printf("Model: %s\n", masina.model);
    printf("Nume sofer: %s\n", masina.numeSofer);
    printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasini(ListaDubla lista) {
    Nod* current = lista.head;
    while (current) {
        afisareMasina(current->masina);
        current = current->next;
    }
}

void adaugaMasinaInLista(ListaDubla* lista, Masina masinaNoua) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->masina = masinaNoua;
    nou->next = NULL;
    nou->prev = lista->tail;

    if (lista->tail) {
        lista->tail->next = nou;
    }
    else {
        lista->head = nou;
    }
    lista->tail = nou;
}

void adaugaLaInceputInLista(ListaDubla* lista, Masina masinaNoua) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->masina = masinaNoua;
    nou->prev = NULL;
    nou->next = lista->head;

    if (lista->head) {
        lista->head->prev = nou;
    }
    else {
        lista->tail = nou;
    }
    lista->head = nou;
}

ListaDubla citireLDMasiniDinFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    ListaDubla lista = { NULL, NULL };

    if (!f) {
        printf("Eroare la deschiderea fisierului!\n");
        return lista;
    }

    while (!feof(f)) {
        Masina m = citireMasinaDinFisier(f);
        if (m.model != NULL) // verificare citire valida
            adaugaMasinaInLista(&lista, m);
    }

    fclose(f);
    return lista;
}

void dezalocareLDMasini(ListaDubla* lista) {
    Nod* current = lista->head;
    while (current) {
        Nod* temp = current;
        current = current->next;
        free(temp->masina.model);
        free(temp->masina.numeSofer);
        free(temp);
    }
    lista->head = NULL;
    lista->tail = NULL;
}

float calculeazaPretMediu(ListaDubla lista) {
    float suma = 0;
    int count = 0;
    Nod* current = lista.head;
    while (current) {
        suma += current->masina.pret;
        count++;
        current = current->next;
    }
    return count > 0 ? suma / count : 0;
}

void stergeMasinaDupaID(ListaDubla* lista, int id) {
    Nod* current = lista->head;
    while (current) {
        if (current->masina.id == id) {
            if (current->prev)
                current->prev->next = current->next;
            else
                lista->head = current->next;

            if (current->next)
                current->next->prev = current->prev;
            else
                lista->tail = current->prev;

            free(current->masina.model);
            free(current->masina.numeSofer);
            free(current);
            return;
        }
        current = current->next;
    }
}

char* getNumeSoferMasinaScumpa(ListaDubla lista) {
    if (!lista.head) return NULL;

    Nod* current = lista.head;
    Nod* masinaScumpa = lista.head;
    while (current) {
        if (current->masina.pret > masinaScumpa->masina.pret)
            masinaScumpa = current;
        current = current->next;
    }

    char* rezultat = malloc(strlen(masinaScumpa->masina.numeSofer) + 1);
    strcpy(rezultat, masinaScumpa->masina.numeSofer);
    return rezultat;
}

int main() {
    ListaDubla lista = citireLDMasiniDinFisier("masini.txt");
    afisareListaMasini(lista);

    printf("Pret mediu: %.2f\n", calculeazaPretMediu(lista));

    char* sofer = getNumeSoferMasinaScumpa(lista);
    if (sofer) {
        printf("Soferul celei mai scumpe masini este: %s\n", sofer);
        free(sofer);
    }

    stergeMasinaDupaID(&lista, 2);
    printf("\nDupa stergere:\n");
    afisareListaMasini(lista);

    dezalocareLDMasini(&lista);
    return 0;
}
