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

//Nod lista dublu inlantuita
typedef struct Nod {
	Masina masina;
	struct Nod* next;
	struct Nod* prev;
} Nod;

//Lista dublu inlantuita
typedef struct {
	Nod* first;
	Nod* last;
	int nrNoduri;
} ListaDubla;

//Nod lista simplu inlantuita (pentru cerinta 4)
typedef struct NodSimplu {
	Masina masina;
	struct NodSimplu* next;
} NodSimplu;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	if (!fgets(buffer, 100, file)) {
		Masina gol = { 0, 0, 0, NULL, NULL, 0 };
		return gol;
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

//Afisare lista crescator (cerinta 3)
void afisareListaDeLaInceput(ListaDubla lista) {
	Nod* p = lista.first;
	while (p) {
		afisareMasina(p->masina);
		p = p->next;
	}
}

//Afisare lista descrescator (cerinta 3)
void afisareListaDeLaSfarsit(ListaDubla lista) {
	Nod* p = lista.last;
	while (p) {
		afisareMasina(p->masina);
		p = p->prev;
	}
}

//Inserare sortata crescator dupa pret (cerinta 2)
void inserareSortata(ListaDubla* lista, Masina m) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->masina = m;
	nou->next = NULL;
	nou->prev = NULL;

	if (!lista->first) {
		lista->first = lista->last = nou;
	}
	else {
		Nod* p = lista->first;
		while (p && p->masina.pret < m.pret)
			p = p->next;
		if (!p) {
			nou->prev = lista->last;
			lista->last->next = nou;
			lista->last = nou;
		}
		else if (!p->prev) {
			nou->next = p;
			p->prev = nou;
			lista->first = nou;
		}
		else {
			nou->next = p;
			nou->prev = p->prev;
			p->prev->next = nou;
			p->prev = nou;
		}
	}
	lista->nrNoduri++;
}

ListaDubla citireDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	ListaDubla lista = { NULL, NULL, 0 };
	if (f) {
		while (!feof(f)) {
			Masina m = citireMasinaDinFisier(f);
			if (m.model != NULL)
				inserareSortata(&lista, m);
		}
		fclose(f);
	}
	return lista;
}

// Cerinta 1: Sterge un nod de pe o pozitie data
void stergePozitie(ListaDubla* lista, int poz) {
	if (poz < 0 || poz >= lista->nrNoduri) return;
	Nod* p = lista->first;
	int i = 0;
	while (p && i < poz) {
		p = p->next;
		i++;
	}
	if (!p) return;
	if (p->prev) p->prev->next = p->next;
	else lista->first = p->next;
	if (p->next) p->next->prev = p->prev;
	else lista->last = p->prev;
	free(p->masina.model);
	free(p->masina.numeSofer);
	free(p);
	lista->nrNoduri--;
}

// Cerinta 4: Copiere in lista simpla cu deep copy
void copiereInListaSimpla(ListaDubla lista, NodSimplu** cap, float prag) {
	Nod* p = lista.first;
	while (p) {
		if (p->masina.pret > prag) {
			NodSimplu* nou = (NodSimplu*)malloc(sizeof(NodSimplu));
			nou->masina.id = p->masina.id;
			nou->masina.nrUsi = p->masina.nrUsi;
			nou->masina.pret = p->masina.pret;
			nou->masina.serie = p->masina.serie;
			nou->masina.model = malloc(strlen(p->masina.model) + 1);
			strcpy(nou->masina.model, p->masina.model);
			nou->masina.numeSofer = malloc(strlen(p->masina.numeSofer) + 1);
			strcpy(nou->masina.numeSofer, p->masina.numeSofer);
			nou->next = *cap;
			*cap = nou;
		}
		p = p->next;
	}
}

// Cerinta 5: Interschimbare informatii intre 2 pozitii
void interschimbarePozitii(ListaDubla lista, int poz1, int poz2) {
	if (poz1 == poz2) return;
	Nod* n1 = lista.first;
	Nod* n2 = lista.first;
	int i = 0;
	while (n1 && i < poz1) { n1 = n1->next; i++; }
	i = 0;
	while (n2 && i < poz2) { n2 = n2->next; i++; }
	if (n1 && n2) {
		Masina aux = n1->masina;
		n1->masina = n2->masina;
		n2->masina = aux;
	}
}

void afisareListaSimpla(NodSimplu* cap) {
	while (cap) {
		afisareMasina(cap->masina);
		cap = cap->next;
	}
}

void dezalocareDubla(ListaDubla* lista) {
	Nod* p = lista->first;
	while (p) {
		Nod* aux = p;
		p = p->next;
		free(aux->masina.model);
		free(aux->masina.numeSofer);
		free(aux);
	}
	lista->first = lista->last = NULL;
	lista->nrNoduri = 0;
}

void dezalocareSimpla(NodSimplu** cap) {
	while (*cap) {
		NodSimplu* aux = *cap;
		*cap = (*cap)->next;
		free(aux->masina.model);
		free(aux->masina.numeSofer);
		free(aux);
	}
}

int main() {
	ListaDubla lista = citireDinFisier("masini.txt");

	// Cerinta 3 – Afisare crescator
	printf("=== Lista crescator ===\n");
	afisareListaDeLaInceput(lista);

	// Cerinta 3 – Afisare descrescator
	printf("\n=== Lista descrescator ===\n");
	afisareListaDeLaSfarsit(lista);

	// Cerinta 1 – Sterge de pe pozitia 2
	printf("\n=== Dupa stergere pozitia 2 ===\n");
	stergePozitie(&lista, 2);
	afisareListaDeLaInceput(lista);

	// Cerinta 5 – Interschimbam pozitiile 0 si 1
	printf("\n=== Interschimbam pozitiile 0 si 1 ===\n");
	interschimbarePozitii(lista, 0, 1);
	afisareListaDeLaInceput(lista);

	// Cerinta 4 – Copiem in lista simpla masinile cu pret > 25000
	printf("\n=== Lista simpla (pret > 25000) ===\n");
	NodSimplu* listaSimpla = NULL;
	copiereInListaSimpla(lista, &listaSimpla, 25000);
	afisareListaSimpla(listaSimpla);

	dezalocareSimpla(&listaSimpla);
	dezalocareDubla(&lista);

	return 0;
}
