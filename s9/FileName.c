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
    struct Nod* stanga;
    struct Nod* dreapta;
} Nod;

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
    printf("Nr. usi: %d\n", masina.nrUsi);
    printf("Pret: %.2f\n", masina.pret);
    printf("Model: %s\n", masina.model);
    printf("Nume sofer: %s\n", masina.numeSofer);
    printf("Serie: %c\n\n", masina.serie);
}

Nod* adaugaMasinaInArboreEchilibrat(Nod* radacina, Masina masinaNoua) {
    if (radacina == NULL) {
        Nod* nodNou = (Nod*)malloc(sizeof(Nod));
        nodNou->masina = masinaNoua;
        nodNou->stanga = NULL;
        nodNou->dreapta = NULL;
        return nodNou;
    }
    if (masinaNoua.id < radacina->masina.id)
        radacina->stanga = adaugaMasinaInArboreEchilibrat(radacina->stanga, masinaNoua);
    else
        radacina->dreapta = adaugaMasinaInArboreEchilibrat(radacina->dreapta, masinaNoua);
    return radacina;
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
    FILE* file = fopen(numeFisier, "r");
    if (!file) return NULL;

    Nod* radacina = NULL;
    while (!feof(file)) {
        Masina m = citireMasinaDinFisier(file);
        if (m.model != NULL && m.numeSofer != NULL)
            radacina = adaugaMasinaInArboreEchilibrat(radacina, m);
    }
    fclose(file);
    return radacina;
}

void parcurgerePreordine(Nod* radacina) {
    if (radacina) {
        afisareMasina(radacina->masina);
        parcurgerePreordine(radacina->stanga);
        parcurgerePreordine(radacina->dreapta);
    }
}

void parcurgereInordine(Nod* radacina) {
    if (radacina) {
        parcurgereInordine(radacina->stanga);
        afisareMasina(radacina->masina);
        parcurgereInordine(radacina->dreapta);
    }
}

void parcurgerePostordine(Nod* radacina) {
    if (radacina) {
        parcurgerePostordine(radacina->stanga);
        parcurgerePostordine(radacina->dreapta);
        afisareMasina(radacina->masina);
    }
}

void afisareMasiniDinArbore(Nod* radacina) {
    printf("Preordine:\n");
    parcurgerePreordine(radacina);
    printf("\nInordine:\n");
    parcurgereInordine(radacina);
    printf("\nPostordine:\n");
    parcurgerePostordine(radacina);
}

void dezalocareArboreDeMasini(Nod* radacina) {
    if (radacina) {
        dezalocareArboreDeMasini(radacina->stanga);
        dezalocareArboreDeMasini(radacina->dreapta);
        free(radacina->masina.model);
        free(radacina->masina.numeSofer);
        free(radacina);
    }
}

Masina getMasinaByID(Nod* radacina, int id) {
    if (!radacina) return (Masina) { 0 };
    if (radacina->masina.id == id)
        return radacina->masina;
    else if (id < radacina->masina.id)
        return getMasinaByID(radacina->stanga, id);
    else
        return getMasinaByID(radacina->dreapta, id);
}

int determinaNumarNoduri(Nod* radacina) {
    if (!radacina) return 0;
    return 1 + determinaNumarNoduri(radacina->stanga) + determinaNumarNoduri(radacina->dreapta);
}

float calculeazaPretTotal(Nod* radacina) {
    if (!radacina) return 0;
    return radacina->masina.pret + calculeazaPretTotal(radacina->stanga) + calculeazaPretTotal(radacina->dreapta);
}

float calculeazaPretulMasinilorUnuiSofer(Nod* radacina, const char* numeSofer) {
    if (!radacina) return 0;
    float suma = 0;
    if (strcmp(radacina->masina.numeSofer, numeSofer) == 0)
        suma += radacina->masina.pret;
    suma += calculeazaPretulMasinilorUnuiSofer(radacina->stanga, numeSofer);
    suma += calculeazaPretulMasinilorUnuiSofer(radacina->dreapta, numeSofer);
    return suma;
}

int calculeazaInaltimeArbore(Nod* radacina) {
    if (!radacina) return 0;
    int hStanga = calculeazaInaltimeArbore(radacina->stanga);
    int hDreapta = calculeazaInaltimeArbore(radacina->dreapta);
    return 1 + (hStanga > hDreapta ? hStanga : hDreapta);
}

int main() {
    Nod* arbore = citireArboreDeMasiniDinFisier("masini_arbore.txt");
    afisareMasiniDinArbore(arbore);

    printf("\nNumar de masini: %d\n", determinaNumarNoduri(arbore));
    printf("Pret total: %.2f\n", calculeazaPretTotal(arbore));
    printf("Inaltime arbore: %d\n", calculeazaInaltimeArbore(arbore));

    int idCautat = 2;
    Masina m = getMasinaByID(arbore, idCautat);
    if (m.model != NULL) {
        printf("\nMasina cu ID-ul %d:\n", idCautat);
        afisareMasina(m);
    }

    const char* sofer = "Sofer2";
    printf("\nPret total masini pentru %s: %.2f\n", sofer, calculeazaPretulMasinilorUnuiSofer(arbore, sofer));

    dezalocareArboreDeMasini(arbore);
    return 0;
}
