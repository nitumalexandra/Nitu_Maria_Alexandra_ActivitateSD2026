#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Masina {
    int id;
    char* marca;
    float pret;
    char* combustibil;
    int anFabricatie;
};

typedef struct Masina Masina;
typedef struct Nod Nod;
typedef struct Lista Lista;

struct Nod {
    Masina m;
    Nod* next;
    Nod* prev;
};

struct Lista {
    Nod* prim;
    Nod* ultim;
};

Masina creareMasina(int id, const char* marca, float pret, const char* combustibil, int anFabricatie) {
    Masina m;

    m.id = id;

    m.marca = (char*)malloc(strlen(marca) + 1);
    strcpy(m.marca, marca);

    m.pret = pret;

    m.combustibil = (char*)malloc(strlen(combustibil) + 1);
    strcpy(m.combustibil, combustibil);

    m.anFabricatie = anFabricatie;

    return m;
}

void afisareMasina(Masina m) {
    printf("ID: %d\n", m.id);
    printf("Marca: %s\n", m.marca);
    printf("Pret: %.2f\n", m.pret);
    printf("Combustibil: %s\n", m.combustibil);
    printf("An fabricatie: %d\n\n", m.anFabricatie);
}

void adaugaMasina(Lista* l, Masina m) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));

    nou->m = m;
    nou->next = NULL;
    nou->prev = NULL;

    if (l->ultim != NULL) {
        nou->prev = l->ultim;
        l->ultim->next = nou;
        l->ultim = nou;
    }
    else {
        l->prim = nou;
        l->ultim = nou;
    }
}

void afisareLista(Lista l) {
    Nod* temp = l.prim;

    while (temp != NULL) {
        afisareMasina(temp->m);
        temp = temp->next;
    }
}

void afisareInversa(Lista l) {
    Nod* temp = l.ultim;

    while (temp != NULL) {
        afisareMasina(temp->m);
        temp = temp->prev;
    }
}

float pretMediu(Lista l) {
    Nod* temp = l.prim;
    float suma = 0;
    int count = 0;

    while (temp != NULL) {
        suma += temp->m.pret;
        count++;
        temp = temp->next;
    }

    if (count > 0) {
        return suma / count;
    }

    return 0;
}

Masina* masinaPretMaxim(Lista l) {
    if (l.prim == NULL) {
        return NULL;
    }

    Masina* maxim = &(l.prim->m);
    Nod* temp = l.prim->next;

    while (temp != NULL) {
        if (temp->m.pret > maxim->pret) {
            maxim = &(temp->m);
        }

        temp = temp->next;
    }

    return maxim;
}

void stergeMasina(Lista* l, int id) {
    Nod* temp = l->prim;

    while (temp != NULL && temp->m.id != id) {
        temp = temp->next;
    }

    if (temp != NULL) {
        if (temp == l->prim) {
            l->prim = temp->next;
        }

        if (temp == l->ultim) {
            l->ultim = temp->prev;
        }

        if (temp->prev != NULL) {
            temp->prev->next = temp->next;
        }

        if (temp->next != NULL) {
            temp->next->prev = temp->prev;
        }

        free(temp->m.marca);
        free(temp->m.combustibil);
        free(temp);
    }
}

int numarMasiniCombustibil(Lista l, const char* combustibil) {
    Nod* temp = l.prim;
    int count = 0;

    while (temp != NULL) {
        if (strcmp(temp->m.combustibil, combustibil) == 0) {
            count++;
        }

        temp = temp->next;
    }

    return count;
}

void dezalocareLista(Lista* l) {
    Nod* temp = l->prim;

    while (temp != NULL) {
        Nod* urmator = temp->next;

        free(temp->m.marca);
        free(temp->m.combustibil);
        free(temp);

        temp = urmator;
    }

    l->prim = NULL;
    l->ultim = NULL;
}

int main() {
    Lista l;
    l.prim = NULL;
    l.ultim = NULL;

    adaugaMasina(&l, creareMasina(1, "BMW", 18500.50, "Diesel", 2018));
    adaugaMasina(&l, creareMasina(2, "Audi", 21000.00, "Benzina", 2020));
    adaugaMasina(&l, creareMasina(3, "Dacia", 9500.75, "Benzina", 2019));
    adaugaMasina(&l, creareMasina(4, "Toyota", 24500.99, "Hibrid", 2021));
    adaugaMasina(&l, creareMasina(5, "Volkswagen", 16000.00, "Diesel", 2017));

    printf("Lista masini:\n");
    afisareLista(l);

    printf("Lista afisata invers:\n");
    afisareInversa(l);

    printf("Pret mediu: %.2f\n\n", pretMediu(l));

    Masina* masinaMaxima = masinaPretMaxim(l);
    if (masinaMaxima != NULL) {
        printf("Masina cu pretul cel mai mare:\n");
        afisareMasina(*masinaMaxima);
    }

    printf("Numar masini Diesel: %d\n\n",
        numarMasiniCombustibil(l, "Diesel"));

    stergeMasina(&l, 2);
    stergeMasina(&l, 4);

    printf("Lista dupa stergeri:\n");
    afisareLista(l);

    printf("Lista dupa stergeri afisata invers:\n");
    afisareInversa(l);

    dezalocareLista(&l);

    return 0;
}