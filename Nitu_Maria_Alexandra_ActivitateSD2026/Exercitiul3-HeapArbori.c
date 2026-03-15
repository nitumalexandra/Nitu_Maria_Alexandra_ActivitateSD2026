#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Produs Produs;
typedef struct Nod Nod;
typedef struct Heap Heap;

struct Produs {
    int id;
    float pret;
    int stoc;
    char* denumire;
    char categorie;
};

struct Nod {
    Produs info;
    Nod* st;
    Nod* dr;
};

struct Heap {
    Produs* vector;
    int nrElemente;
};

Produs initializare(int id, float pret, int stoc, const char* denumire, char categorie) {
    Produs p;
    p.id = id;
    p.pret = pret;
    p.stoc = stoc;
    p.denumire = (char*)malloc(sizeof(char) * (strlen(denumire) + 1));
    strcpy(p.denumire, denumire);
    p.categorie = categorie;
    return p;
}

Produs copiaza(Produs p) {
    return initializare(p.id, p.pret, p.stoc, p.denumire, p.categorie);
}

void afisareProdus(Produs p) {
    printf("%d. Produsul %s din categoria %c are pretul %.2f si stocul %d.\n",
        p.id, p.denumire, p.categorie, p.pret, p.stoc);
}

void afisareVector(Produs* vector, int nrElemente) {
    for (int i = 0; i < nrElemente; i++) {
        afisareProdus(vector[i]);
    }
}

void dezalocareProdus(Produs* p) {
    if (p->denumire != NULL) {
        free(p->denumire);
        p->denumire = NULL;
    }
}

void dezalocareVector(Produs** vector, int* nrElemente) {
    for (int i = 0; i < *nrElemente; i++) {
        dezalocareProdus(&(*vector)[i]);
    }
    free(*vector);
    *vector = NULL;
    *nrElemente = 0;
}

Nod* inserareABC(Nod* radacina, Produs p) {
    if (radacina == NULL) {
        Nod* nou = (Nod*)malloc(sizeof(Nod));
        nou->info = copiaza(p);
        nou->st = NULL;
        nou->dr = NULL;
        return nou;
    }

    if (p.id < radacina->info.id) {
        radacina->st = inserareABC(radacina->st, p);
    }
    else if (p.id > radacina->info.id) {
        radacina->dr = inserareABC(radacina->dr, p);
    }

    return radacina;
}

void afisareInordine(Nod* radacina) {
    if (radacina) {
        afisareInordine(radacina->st);
        afisareProdus(radacina->info);
        afisareInordine(radacina->dr);
    }
}

Produs cautaDupaId(Nod* radacina, int id) {
    Produs p;
    p.id = -1;
    p.denumire = NULL;

    if (radacina == NULL) {
        return p;
    }

    if (id == radacina->info.id) {
        return copiaza(radacina->info);
    }
    else if (id < radacina->info.id) {
        return cautaDupaId(radacina->st, id);
    }
    else {
        return cautaDupaId(radacina->dr, id);
    }
}

void dezalocareABC(Nod** radacina) {
    if (*radacina) {
        dezalocareABC(&(*radacina)->st);
        dezalocareABC(&(*radacina)->dr);
        dezalocareProdus(&(*radacina)->info);
        free(*radacina);
        *radacina = NULL;
    }
}

void filtrareHeap(Heap heap, int pozitie) {
    int pozMax = pozitie;
    int fiuSt = 2 * pozitie + 1;
    int fiuDr = 2 * pozitie + 2;

    if (fiuSt < heap.nrElemente && heap.vector[fiuSt].pret > heap.vector[pozMax].pret) {
        pozMax = fiuSt;
    }

    if (fiuDr < heap.nrElemente && heap.vector[fiuDr].pret > heap.vector[pozMax].pret) {
        pozMax = fiuDr;
    }

    if (pozMax != pozitie) {
        Produs aux = heap.vector[pozitie];
        heap.vector[pozitie] = heap.vector[pozMax];
        heap.vector[pozMax] = aux;

        filtrareHeap(heap, pozMax);
    }
}

Heap initializareHeap(Produs* vector, int nrElemente) {
    Heap h;
    h.nrElemente = nrElemente;
    h.vector = (Produs*)malloc(sizeof(Produs) * nrElemente);

    for (int i = 0; i < nrElemente; i++) {
        h.vector[i] = copiaza(vector[i]);
    }

    for (int i = (h.nrElemente - 2) / 2; i >= 0; i--) {
        filtrareHeap(h, i);
    }

    return h;
}

void afisareHeap(Heap h) {
    for (int i = 0; i < h.nrElemente; i++) {
        afisareProdus(h.vector[i]);
    }
}

Produs extrageRadacina(Heap* h) {
    Produs rezultat;
    rezultat.id = -1;
    rezultat.denumire = NULL;

    if (h->nrElemente <= 0) {
        return rezultat;
    }

    rezultat = copiaza(h->vector[0]);

    dezalocareProdus(&h->vector[0]);
    h->vector[0] = h->vector[h->nrElemente - 1];
    h->nrElemente--;

    if (h->nrElemente > 0) {
        filtrareHeap(*h, 0);
    }

    return rezultat;
}

void dezalocareHeap(Heap* h) {
    for (int i = 0; i < h->nrElemente; i++) {
        dezalocareProdus(&h->vector[i]);
    }
    free(h->vector);
    h->vector = NULL;
    h->nrElemente = 0;
}

int main() {
    int nrProduse = 5;
    Produs* vector = (Produs*)malloc(sizeof(Produs) * nrProduse);

    vector[0] = initializare(10, 99.5f, 20, "Laptop", 'T');
    vector[1] = initializare(4, 15.0f, 100, "Caiet", 'B');
    vector[2] = initializare(7, 250.0f, 10, "Imprimanta", 'E');
    vector[3] = initializare(15, 120.0f, 5, "Monitor", 'T');
    vector[4] = initializare(2, 8.5f, 200, "Pix", 'B');

    printf("Vector initial:\n");
    afisareVector(vector, nrProduse);

    Nod* arbore = NULL;
    for (int i = 0; i < nrProduse; i++) {
        arbore = inserareABC(arbore, vector[i]);
    }

    printf("\nAfisare inordine din ABC:\n");
    afisareInordine(arbore);

    Produs cautat = cautaDupaId(arbore, 7);
    printf("\nProdus cautat dupa id=7:\n");
    if (cautat.id != -1) {
        afisareProdus(cautat);
        dezalocareProdus(&cautat);
    }
    else {
        printf("Nu a fost gasit.\n");
    }

    Heap heap = initializareHeap(vector, nrProduse);
    printf("\nHeap:\n");
    afisareHeap(heap);

    Produs extras = extrageRadacina(&heap);
    printf("\nProdus extras din heap:\n");
    if (extras.id != -1) {
        afisareProdus(extras);
        dezalocareProdus(&extras);
    }

    printf("\nHeap dupa extragere:\n");
    afisareHeap(heap);

    dezalocareHeap(&heap);
    dezalocareABC(&arbore);
    dezalocareVector(&vector, &nrProduse);

    return 0;
}