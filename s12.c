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

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

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

typedef struct NodListaPrincipala NodListaPrincipala;
typedef struct NodListaSecundara NodListaSecundara;

struct NodListaPrincipala {
	NodListaPrincipala* next;
	NodListaSecundara* vecini;
	Masina info;
};

struct NodListaSecundara {
	NodListaPrincipala* info;
	NodListaSecundara* next;
};

NodListaPrincipala* inserareListaPrincipala(NodListaPrincipala* lista, Masina m) {
	NodListaPrincipala* nou = (NodListaPrincipala*)malloc(sizeof(NodListaPrincipala));
	nou->info = m;
	nou->next = NULL;
	nou->vecini = NULL;
	if (!lista) {
		return nou;
	}
	else {
		NodListaPrincipala* aux = lista;
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = nou;
		return lista;
	}
}

NodListaSecundara* inserareListaSecundara(NodListaSecundara* lista, NodListaPrincipala* info) {
	NodListaSecundara* nou = (NodListaSecundara*)malloc(sizeof(NodListaSecundara));
	nou->info = info;
	nou->next = lista;
	return nou;
}

NodListaPrincipala* cautaNodDupaID(NodListaPrincipala* listaPrincipala, int id) {
	while (listaPrincipala && listaPrincipala->info.id != id) {
		listaPrincipala = listaPrincipala->next;
	}
	return listaPrincipala;
}

void inserareMuchie(NodListaPrincipala* listaPrincipala, int idStart, int idStop) {
	NodListaPrincipala* nodStart = cautaNodDupaID(listaPrincipala, idStart);
	NodListaPrincipala* nodStop = cautaNodDupaID(listaPrincipala, idStop);
	if (nodStart && nodStop) {
		nodStart->vecini = inserareListaSecundara(nodStart->vecini, nodStop);
		nodStop->vecini = inserareListaSecundara(nodStop->vecini, nodStart);
	}
}

NodListaPrincipala* citireNoduriMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	NodListaPrincipala* listaPrincipala = NULL;
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		listaPrincipala = inserareListaPrincipala(listaPrincipala, m);
	}
	fclose(f);
	return listaPrincipala;
}

void citireMuchiiDinFisier(const char* numeFisier, NodListaPrincipala* listaPrincipala) {
	FILE* f = fopen(numeFisier, "r");
	while (!feof(f)) {
		int idStart, idStop;
		fscanf(f, "%d %d", &idStart, &idStop);
		inserareMuchie(listaPrincipala, idStart, idStop);
	}
	fclose(f);
}

void stergereListaSecundara(NodListaSecundara** lista) {
	NodListaSecundara* aux;
	while (*lista) {
		aux = (*lista)->next;
		free(*lista);
		*lista = aux;
	}
}

void dezalocareNoduriGraf(NodListaPrincipala** listaPrincipala) {
	NodListaPrincipala* aux;
	while (*listaPrincipala) {
		aux = (*listaPrincipala)->next;
		stergereListaSecundara(&(*listaPrincipala)->vecini);
		if ((*listaPrincipala)->info.model) {
			free((*listaPrincipala)->info.model);
		}
		if ((*listaPrincipala)->info.numeSofer) {
			free((*listaPrincipala)->info.numeSofer);
		}
		free(*listaPrincipala);
		(*listaPrincipala) = aux;
	}
}

int main() {
	NodListaPrincipala* listaPrincipala = citireNoduriMasiniDinFisier("masini.txt");
	citireMuchiiDinFisier("muchii.txt", listaPrincipala);
	dezalocareNoduriGraf(&listaPrincipala);
	return 0;
}