#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
} Masina;

typedef struct Nod {
	Masina masina;
	struct Nod* next;
} Nod;

typedef struct {
	Nod** vectorListe;
	int dim;
} HashTable;


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

void afisareMasina(Masina m) {
	printf("ID: %d | Nr usi: %d | Pret: %.2f | Model: %s | Sofer: %s | Serie: %c\n",
		m.id, m.nrUsi, m.pret, m.model, m.numeSofer, m.serie);
}

HashTable initializareHashTable(int dimensiune) {
	HashTable ht;
	ht.dim = dimensiune;
	ht.vectorListe = (Nod**)malloc(sizeof(Nod*) * dimensiune);
	for (int i = 0; i < dimensiune; i++)
		ht.vectorListe[i] = NULL;
	return ht;
}

int calculeazaHash(int id, int dim) {
	return id % dim;
}

void inserareMasinaInTabela(HashTable hash, Masina m) {
	int poz = calculeazaHash(m.id, hash.dim);
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->masina = m;
	nou->next = hash.vectorListe[poz];
	hash.vectorListe[poz] = nou;
}

HashTable citireMasiniDinFisier(const char* numeFisier, int dimTabela) {
	HashTable ht = initializareHashTable(dimTabela);
	FILE* f = fopen(numeFisier, "r");
	if (!f) {
		printf("Eroare deschidere fisier!\n");
		return ht;
	}
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		if (m.model != NULL)
			inserareMasinaInTabela(ht, m);
	}
	fclose(f);
	return ht;
}

void afisareTabelaDeMasini(HashTable ht) {
	for (int i = 0; i < ht.dim; i++) {
		printf("Cluster %d:\n", i);
		Nod* p = ht.vectorListe[i];
		if (!p) {
			printf("  (gol)\n");
		}
		while (p) {
			printf("  ");
			afisareMasina(p->masina);
			p = p->next;
		}
	}
}

void dezalocareTabelaDeMasini(HashTable* ht) {
	for (int i = 0; i < ht->dim; i++) {
		Nod* p = ht->vectorListe[i];
		while (p) {
			Nod* aux = p;
			p = p->next;
			free(aux->masina.model);
			free(aux->masina.numeSofer);
			free(aux);
		}
	}
	free(ht->vectorListe);
	ht->vectorListe = NULL;
	ht->dim = 0;
}

float* calculeazaPreturiMediiPerClustere(HashTable ht, int* nrClustere) {
	float* medii = (float*)malloc(sizeof(float) * ht.dim);
	*nrClustere = 0;

	for (int i = 0; i < ht.dim; i++) {
		Nod* p = ht.vectorListe[i];
		int cnt = 0;
		float suma = 0;
		while (p) {
			suma += p->masina.pret;
			cnt++;
			p = p->next;
		}
		if (cnt > 0) {
			medii[*nrClustere] = suma / cnt;
			(*nrClustere)++;
		}
	}
	return medii;
}

Masina getMasinaDupaID(HashTable ht, int idCautat) {
	int poz = calculeazaHash(idCautat, ht.dim);
	Nod* p = ht.vectorListe[poz];
	while (p) {
		if (p->masina.id == idCautat)
			return p->masina;
		p = p->next;
	}
	Masina gol = { 0, 0, 0, NULL, NULL, 0 };
	return gol;
}

int main() {
	HashTable ht = citireMasiniDinFisier("masini.txt", 5);

	printf("\n=== Tabela de dispersie ===\n");
	afisareTabelaDeMasini(ht);

	int nr;
	float* medii = calculeazaPreturiMediiPerClustere(ht, &nr);
	printf("\n=== Preturi medii per cluster ===\n");
	for (int i = 0; i < nr; i++)
		printf("Cluster activ %d: %.2f\n", i, medii[i]);
	free(medii);

	printf("\n=== Cautam masina cu ID = 3 ===\n");
	Masina m = getMasinaDupaID(ht, 3);
	if (m.model)
		afisareMasina(m);
	else
		printf("Masina nu a fost gasita.\n");

	dezalocareTabelaDeMasini(&ht);
	return 0;
}
