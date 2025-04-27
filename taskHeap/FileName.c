#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

typedef struct {
    char nume[50];
    int varsta;
    int gradUrgenta; // Cu cât mai mare, cu atât mai urgent
} Pacient;

typedef struct {
    Pacient heap[MAX];
    int size;
} MaxHeap;

void swap(Pacient* a, Pacient* b) {
    Pacient temp = *a;
    *a = *b;
    *b = temp;
}

void insert(MaxHeap* h, Pacient p) {
    if (h->size >= MAX) {
        printf("Eroare: Heap plin!\n");
        return;
    }
    h->heap[h->size] = p;
    int i = h->size;
    h->size++;

    // Heapify-up
    while (i != 0 && h->heap[(i - 1) / 2].gradUrgenta < h->heap[i].gradUrgenta) {
        swap(&h->heap[i], &h->heap[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

Pacient extractMax(MaxHeap* h) {
    if (h->size <= 0) {
        Pacient p = { "", -1, -1 }; // Pacient invalid
        return p;
    }
    if (h->size == 1) {
        h->size--;
        return h->heap[0];
    }

    Pacient root = h->heap[0];
    h->heap[0] = h->heap[h->size - 1];
    h->size--;

    // Heapify-down
    int i = 0;
    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int largest = i;

        if (left < h->size && h->heap[left].gradUrgenta > h->heap[largest].gradUrgenta)
            largest = left;
        if (right < h->size && h->heap[right].gradUrgenta > h->heap[largest].gradUrgenta)
            largest = right;

        if (largest != i) {
            swap(&h->heap[i], &h->heap[largest]);
            i = largest;
        }
        else {
            break;
        }
    }
    return root;
}

void afisarePacienti(MaxHeap h) {
    printf("\nLista pacienti in asteptare:\n");
    for (int i = 0; i < h.size; i++) {
        printf("%s | Varsta: %d | Grad urgenta: %d\n", h.heap[i].nume, h.heap[i].varsta, h.heap[i].gradUrgenta);
    }
}

int main() {
    MaxHeap h;
    h.size = 0;

    int optiune;
    do {
        printf("\n=== Meniu UPU ===\n");
        printf("1. Adaugare pacient\n");
        printf("2. Preluare pacient cu cea mai mare urgenta\n");
        printf("3. Afisare lista pacienti\n");
        printf("4. Iesire\n");
        printf("Optiune: ");
        scanf_s("%d", &optiune);

        if (optiune == 1) {
            Pacient p;

            printf("Nume: ");
            while (getchar() != '\n'); // <<< Asta adaug-o aici
            scanf_s(" %[^\n]", p.nume, (unsigned)_countof(p.nume));

            printf("Varsta: ");
            scanf_s("%d", &p.varsta);

            int ambulanta;
            printf("A venit cu ambulanta? (1 - Da, 0 - Nu): ");
            scanf_s("%d", &ambulanta);

            printf("Grad de urgenta (1-10): ");
            scanf_s("%d", &p.gradUrgenta);

            if (ambulanta) {
                p.gradUrgenta += 5;
                if (p.gradUrgenta > 15) p.gradUrgenta = 15;
            }

            insert(&h, p);
            printf("Pacient adaugat cu succes!\n");
        }

        else if (optiune == 2) {
            Pacient p = extractMax(&h);
            if (p.gradUrgenta != -1) {
                printf("\nSe preia pacientul: %s, varsta %d, grad urgenta %d\n", p.nume, p.varsta, p.gradUrgenta);
            }
            else {
                printf("\nNu exista pacienti in asteptare.\n");
            }
        }
        else if (optiune == 3) {
            afisarePacienti(h);
        }
        else if (optiune == 4) {
            printf("Iesire din program.\n");
        }
        else {
            printf("Optiune invalida!\n");
        }
    } while (optiune != 4);

    return 0;
}
