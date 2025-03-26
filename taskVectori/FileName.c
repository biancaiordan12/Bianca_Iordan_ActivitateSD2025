
#include <stdio.h>
#include <malloc.h>


// Structura Inventar
struct Inventar {
    int id;
    char* denumire;
    int cantitate;
    float pretUnitar;
    char categorie;
};

// Initializare structura
struct Inventar initializare(int id, const char* denumire, int cantitate, float pret, char categorie) {
    struct Inventar i;
    i.id = id;
    i.denumire = (char*)malloc(strlen(denumire) + 1);
    strcpy(i.denumire, denumire);
    i.cantitate = cantitate;
    i.pretUnitar = pret;
    i.categorie = categorie;
    return i;
}

// Afisare
void afisare(struct Inventar i) {
    printf("ID: %d | Denumire: %s | Cantitate: %d | Pret: %.2f | Categorie: %c\n",
        i.id, i.denumire, i.cantitate, i.pretUnitar, i.categorie);
}

// Afisare vector
void afisareVector(struct Inventar* vector, int nrElemente) {
    for (int i = 0; i < nrElemente; i++) {
        afisare(vector[i]);
    }
}

// Copiere primele N elemente
struct Inventar* copiazaPrimeleNElemente(struct Inventar* vector, int nrElemente, int nrElementeCopiate) {
    if (nrElementeCopiate > nrElemente) return NULL;

    struct Inventar* vectorNou = (struct Inventar*)malloc(sizeof(struct Inventar) * nrElementeCopiate);
    for (int i = 0; i < nrElementeCopiate; i++) {
        vectorNou[i] = initializare(vector[i].id, vector[i].denumire, vector[i].cantitate, vector[i].pretUnitar, vector[i].categorie);
    }
    return vectorNou;
}

// Dezalocare vector
void dezalocare(struct Inventar** vector, int* nrElemente) {
    if (*vector != NULL) {
        for (int i = 0; i < *nrElemente; i++) {
            free((*vector)[i].denumire);
        }
        free(*vector);
        *vector = NULL;
        *nrElemente = 0;
    }
}

// Copiaza in vectorNou elementele cu pret > prag
void copiazaAnumiteElemente(struct Inventar* vector, char nrElemente, float prag, struct Inventar** vectorNou, int* dimensiune) {
    *dimensiune = 0;

    // determinam cate elemente respecta conditia
    for (int i = 0; i < nrElemente; i++) {
        if (vector[i].pretUnitar > prag)
            (*dimensiune)++;
    }

    *vectorNou = (struct Inventar*)malloc(sizeof(struct Inventar) * (*dimensiune));
    int k = 0;
    for (int i = 0; i < nrElemente; i++) {
        if (vector[i].pretUnitar > prag) {
            (*vectorNou)[k] = initializare(vector[i].id, vector[i].denumire, vector[i].cantitate, vector[i].pretUnitar, vector[i].categorie);
            k++;
        }
    }
}

// Returneaza primul obiect cu denumirea care contine o subsecventa data
struct Inventar getPrimulElementConditionat(struct Inventar* vector, int nrElemente, const char* conditie) {
    for (int i = 0; i < nrElemente; i++) {
        if (strstr(vector[i].denumire, conditie) != NULL) {
            return vector[i]; // shallow copy, dar OK pt afișare
        }
    }
    // daca nu gaseste, returneaza unul gol
    return initializare(-1, "NEEXISTENT", 0, 0, '-');
}

// ======================= MAIN ==========================

int main() {
    int nrElemente = 5;
    struct Inventar* vector = (struct Inventar*)malloc(sizeof(struct Inventar) * nrElemente);

    // Inițializare
    vector[0] = initializare(1, "Pix albastru", 30, 2.5, 'B');
    vector[1] = initializare(2, "Caiet A5", 50, 6.0, 'S');
    vector[2] = initializare(3, "Creion mecanic", 20, 3.0, 'B');
    vector[3] = initializare(4, "Acuarele", 15, 10.0, 'A');
    vector[4] = initializare(5, "Agenda 2024", 12, 15.5, 'S');

    printf("=== Vector Initial ===\n");
    afisareVector(vector, nrElemente);

    // Copiere primele 3
    printf("\n=== Copiem primele 3 ===\n");
    struct Inventar* primii = copiazaPrimeleNElemente(vector, nrElemente, 3);
    afisareVector(primii, 3);
    dezalocare(&primii, &(int){3});

    // Copiere obiecte cu pret > 5
    printf("\n=== Copiem cu pret > 5 ===\n");
    struct Inventar* peste5 = NULL;
    int dimPeste5 = 0;
    copiazaAnumiteElemente(vector, nrElemente, 5.0, &peste5, &dimPeste5);
    afisareVector(peste5, dimPeste5);
    dezalocare(&peste5, &dimPeste5);

    // Cautam prima denumire care conține "Pix"
    printf("\n=== Cautam obiect ce contine 'Pix' ===\n");
    struct Inventar gasit = getPrimulElementConditionat(vector, nrElemente, "Pix");
    afisare(gasit);

    // Dezalocare totala
    dezalocare(&vector, &nrElemente);

    return 0;
}
