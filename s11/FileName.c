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

typedef struct NodStiva {
	Masina masina;
	struct NodStiva* next;
} NodStiva;

typedef struct NodCoada {
	Masina masina;
	struct NodCoada* next;
} NodCoada;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	if (fgets(buffer, 100, file) == NULL) {
		Masina m = { 0 };
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

	aux = strtok(NULL, sep);
	m1.serie = aux[0];
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

void pushStack(NodStiva** varf, Masina masina) {
	NodStiva* nou = (NodStiva*)malloc(sizeof(NodStiva));
	nou->masina = masina;
	nou->next = *varf;
	*varf = nou;
}

Masina popStack(NodStiva** varf) {
	Masina m = (*varf)->masina;
	NodStiva* temp = *varf;
	*varf = (*varf)->next;
	free(temp);
	return m;
}

int emptyStack(NodStiva* varf) {
	return varf == NULL;
}

NodStiva* citireStackMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	if (!f) {
		printf("Eroare deschidere fisier\n");
		return NULL;
	}
	NodStiva* stiva = NULL;
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		if (m.id != 0)
			pushStack(&stiva, m);
	}
	fclose(f);
	return stiva;
}

void dezalocareStivaDeMasini(NodStiva** varf) {
	while (!emptyStack(*varf)) {
		Masina m = popStack(varf);
		free(m.model);
		free(m.numeSofer);
	}
}

int sizeStack(NodStiva* varf) {
	int count = 0;
	while (varf) {
		count++;
		varf = varf->next;
	}
	return count;
}

void enqueue(NodCoada** prim, NodCoada** ultim, Masina masina) {
	NodCoada* nou = (NodCoada*)malloc(sizeof(NodCoada));
	nou->masina = masina;
	nou->next = NULL;
	if (*ultim == NULL) {
		*prim = *ultim = nou;
	}
	else {
		(*ultim)->next = nou;
		*ultim = nou;
	}
}

Masina dequeue(NodCoada** prim, NodCoada** ultim) {
	Masina m = (*prim)->masina;
	NodCoada* temp = *prim;
	*prim = (*prim)->next;
	if (*prim == NULL)
		*ultim = NULL;
	free(temp);
	return m;
}

NodCoada* citireCoadaDeMasiniDinFisier(const char* numeFisier, NodCoada** ultim) {
	FILE* f = fopen(numeFisier, "r");
	if (!f) {
		printf("Eroare deschidere fisier\n");
		return NULL;
	}
	NodCoada* prim = NULL;
	*ultim = NULL;
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		if (m.id != 0)
			enqueue(&prim, ultim, m);
	}
	fclose(f);
	return prim;
}

void dezalocareCoadaDeMasini(NodCoada** prim, NodCoada** ultim) {
	while (*prim) {
		Masina m = dequeue(prim, ultim);
		free(m.model);
		free(m.numeSofer);
	}
}

Masina getMasinaByID_Stack(NodStiva* varf, int id) {
	while (varf) {
		if (varf->masina.id == id)
			return varf->masina;
		varf = varf->next;
	}
	Masina m = { 0 };
	return m;
}

Masina getMasinaByID_Coada(NodCoada* prim, int id) {
	while (prim) {
		if (prim->masina.id == id)
			return prim->masina;
		prim = prim->next;
	}
	Masina m = { 0 };
	return m;
}

float calculeazaPretTotal_Stack(NodStiva* varf) {
	float total = 0;
	while (varf) {
		total += varf->masina.pret;
		varf = varf->next;
	}
	return total;
}

float calculeazaPretTotal_Coada(NodCoada* prim) {
	float total = 0;
	while (prim) {
		total += prim->masina.pret;
		prim = prim->next;
	}
	return total;
}

int main() {
	// Test Stack
	NodStiva* stiva = citireStackMasiniDinFisier("masini.txt");
	printf("Stack - Masini citite:\n");
	NodStiva* temp = stiva;
	while (temp) {
		afisareMasina(temp->masina);
		temp = temp->next;
	}
	printf("Pret total stiva: %.2f\n", calculeazaPretTotal_Stack(stiva));

	NodCoada* ultim = NULL;
	NodCoada* coada = citireCoadaDeMasiniDinFisier("masini.txt", &ultim);
	printf("\nCoada - Masini citite:\n");
	NodCoada* temp2 = coada;
	while (temp2) {
		afisareMasina(temp2->masina);
		temp2 = temp2->next;
	}
	printf("Pret total coada: %.2f\n", calculeazaPretTotal_Coada(coada));

	int idCautat = 2;
	Masina m1 = getMasinaByID_Stack(stiva, idCautat);
	printf("\nCautare in stiva masina cu ID %d:\n", idCautat);
	if (m1.id != 0)
		afisareMasina(m1);

	Masina m2 = getMasinaByID_Coada(coada, idCautat);
	printf("\nCautare in coada masina cu ID %d:\n", idCautat);
	if (m2.id != 0)
		afisareMasina(m2);

	dezalocareStivaDeMasini(&stiva);
	dezalocareCoadaDeMasini(&coada, &ultim);

	return 0;
}
