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

struct Nod {
    Masina masina;
    struct Nod* next;
};
typedef struct Nod Nod;

Masina citireMasinaDinFisier(FILE* file) {
    char buffer[100];
    char sep[3] = ",\n";
    if (!fgets(buffer, 100, file)) return (Masina) { 0 };

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

void afisareListaMasini(Nod* cap) {
    while (cap) {
        afisareMasina(cap->masina);
        cap = cap->next;
    }
}

Nod* adaugaMasinaInLista(Nod* cap, Masina masinaNoua) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->masina = masinaNoua;
    nou->next = NULL;
    if (!cap) return nou;
    Nod* temp = cap;
    while (temp->next) temp = temp->next;
    temp->next = nou;
    return cap;
}

Nod* adaugaLaInceputInLista(Nod* cap, Masina masinaNoua) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->masina = masinaNoua;
    nou->next = cap;
    return nou;
}

Nod* citireListaMasiniDinFisier(const char* numeFisier) {
    FILE* file = fopen(numeFisier, "r");
    if (!file) return NULL;
    Nod* cap = NULL;
    while (!feof(file)) {
        Masina m = citireMasinaDinFisier(file);
        cap = adaugaMasinaInLista(cap, m);
    }
    fclose(file);
    return cap;
}

void dezalocareListaMasini(Nod* cap) {
    while (cap) {
        Nod* temp = cap;
        cap = cap->next;
        free(temp->masina.model);
        free(temp->masina.numeSofer);
        free(temp);
    }
}

float calculeazaPretMediu(Nod* cap) {
    if (!cap) return 0;
    float suma = 0;
    int nr = 0;
    while (cap) {
        suma += cap->masina.pret;
        nr++;
        cap = cap->next;
    }
    return suma / nr;
}

Nod* stergeMasiniDinSeria(Nod* cap, char serieCautata) {
    while (cap && cap->masina.serie == serieCautata) {
        Nod* temp = cap;
        cap = cap->next;
        free(temp->masina.model);
        free(temp->masina.numeSofer);
        free(temp);
    }
    Nod* temp = cap;
    while (temp && temp->next) {
        if (temp->next->masina.serie == serieCautata) {
            Nod* deSters = temp->next;
            temp->next = deSters->next;
            free(deSters->masina.model);
            free(deSters->masina.numeSofer);
            free(deSters);
        }
        else {
            temp = temp->next;
        }
    }
    return cap;
}

float calculeazaPretulMasinilorUnuiSofer(Nod* cap, const char* numeSofer) {
    float suma = 0;
    while (cap) {
        if (strcmp(cap->masina.numeSofer, numeSofer) == 0) {
            suma += cap->masina.pret;
        }
        cap = cap->next;
    }
    return suma;
}

int main() {
    Nod* listaMasini = citireListaMasiniDinFisier("masini.txt");
    afisareListaMasini(listaMasini);
    printf("Pret mediu: %.2f\n", calculeazaPretMediu(listaMasini));
    listaMasini = stergeMasiniDinSeria(listaMasini, 'A');
    afisareListaMasini(listaMasini);
    printf("Pret total masini sofer X: %.2f\n", calculeazaPretulMasinilorUnuiSofer(listaMasini, "SoferX"));
    dezalocareListaMasini(listaMasini);
    return 0;
}
