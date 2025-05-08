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
	Masina info;
	struct Nod* st;
	struct Nod* dr;
} Nod;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	if (!fgets(buffer, 100, file)) {
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
	printf("Nr. usi: %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

Nod* adaugaMasinaInArbore(Nod* radacina, Masina m) {
	if (radacina == NULL) {
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = m;
		nou->st = nou->dr = NULL;
		return nou;
	}
	if (m.id < radacina->info.id)
		radacina->st = adaugaMasinaInArbore(radacina->st, m);
	else
		radacina->dr = adaugaMasinaInArbore(radacina->dr, m);
	return radacina;
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	if (!f) {
		printf("Eroare la deschiderea fisierului.\n");
		return NULL;
	}
	Nod* radacina = NULL;
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		if (m.model != NULL) 
			radacina = adaugaMasinaInArbore(radacina, m);
	}
	fclose(f);
	return radacina;
}

void inordine(Nod* rad) {
	if (rad) {
		inordine(rad->st);
		afisareMasina(rad->info);
		inordine(rad->dr);
	}
}

void preordine(Nod* rad) {
	if (rad) {
		afisareMasina(rad->info);
		preordine(rad->st);
		preordine(rad->dr);
	}
}

void postordine(Nod* rad) {
	if (rad) {
		postordine(rad->st);
		postordine(rad->dr);
		afisareMasina(rad->info);
	}
}

void afisareMasiniDinArbore(Nod* radacina) {
	printf("Parcurgere INORDINE:\n");
	inordine(radacina);
	printf("Parcurgere PREORDINE:\n");
	preordine(radacina);
	printf("Parcurgere POSTORDINE:\n");
	postordine(radacina);
}

void dezalocareArboreDeMasini(Nod* rad) {
	if (rad) {
		dezalocareArboreDeMasini(rad->st);
		dezalocareArboreDeMasini(rad->dr);
		free(rad->info.model);
		free(rad->info.numeSofer);
		free(rad);
	}
}

Masina getMasinaByID(Nod* rad, int id) {
	if (rad == NULL) {
		Masina m = { 0, 0, 0, NULL, NULL, 0 };
		return m;
	}
	if (rad->info.id == id)
		return rad->info;
	else if (id < rad->info.id)
		return getMasinaByID(rad->st, id);
	else
		return getMasinaByID(rad->dr, id);
}

int determinaNumarNoduri(Nod* rad) {
	if (rad == NULL)
		return 0;
	return 1 + determinaNumarNoduri(rad->st) + determinaNumarNoduri(rad->dr);
}

int calculeazaInaltimeArbore(Nod* rad) {
	if (rad == NULL)
		return 0;
	int hSt = calculeazaInaltimeArbore(rad->st);
	int hDr = calculeazaInaltimeArbore(rad->dr);
	return 1 + (hSt > hDr ? hSt : hDr);
}

float calculeazaPretTotal(Nod* rad) {
	if (rad == NULL)
		return 0;
	return rad->info.pret + calculeazaPretTotal(rad->st) + calculeazaPretTotal(rad->dr);
}

float calculeazaPretulMasinilorUnuiSofer(Nod* rad, const char* numeSofer) {
	if (rad == NULL)
		return 0;
	float suma = 0;
	if (strcmp(rad->info.numeSofer, numeSofer) == 0)
		suma += rad->info.pret;
	suma += calculeazaPretulMasinilorUnuiSofer(rad->st, numeSofer);
	suma += calculeazaPretulMasinilorUnuiSofer(rad->dr, numeSofer);
	return suma;
}

int main() {
	Nod* arbore = citireArboreDeMasiniDinFisier("masini.txt");

	printf("=== MASINI DIN ARBORE ===\n");
	afisareMasiniDinArbore(arbore);

	int idCautat = 2;
	Masina m = getMasinaByID(arbore, idCautat);
	printf("\n--- Masina cu ID %d ---\n", idCautat);
	afisareMasina(m);

	printf("Numar total noduri: %d\n", determinaNumarNoduri(arbore));
	printf("Inaltime arbore: %d\n", calculeazaInaltimeArbore(arbore));
	printf("Pret total masini: %.2f\n", calculeazaPretTotal(arbore));
	printf("Pret masini sofer 'Ion Popescu': %.2f\n", calculeazaPretulMasinilorUnuiSofer(arbore, "Ion Popescu"));

	dezalocareArboreDeMasini(arbore);
	return 0;
}
