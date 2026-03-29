#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Produs Produs;
typedef struct Nod Nod;

struct Produs {
    int id;
    char* nume;
    float pret;
};

struct Nod {
    Produs info;
    Nod* next;
};

Produs initProdus(int id, const char* nume, float pret) {
    Produs p;
    p.id = id;
    p.pret = pret;
    p.nume = (char*)malloc(strlen(nume) + 1);
    strcpy(p.nume, nume);
    return p;
}

Produs copieProdus(Produs p) {
    return initProdus(p.id, p.nume, p.pret);
}

void afisProdus(Produs p) {
    printf("%d %s %.2f\n", p.id, p.nume, p.pret);
}

void push(Nod** varf, Produs p) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = copieProdus(p);
    nou->next = *varf;
    *varf = nou;
}

Produs pop(Nod** varf) {
    Produs p;
    p.id = -1;
    p.nume = NULL;
    p.pret = 0;

    if (*varf == NULL) return p;

    Nod* aux = *varf;
    p = aux->info;
    *varf = aux->next;
    free(aux);

    return p;
}

void dezalocStiva(Nod** varf) {
    while (*varf) {
        Produs p = pop(varf);
        free(p.nume);
    }
}

int main() {
    Nod* stiva = NULL;

    push(&stiva, initProdus(1, "Laptop", 3500));
    push(&stiva, initProdus(2, "Mouse", 150));
    push(&stiva, initProdus(3, "Tastatura", 300));

    while (stiva) {
        Produs p = pop(&stiva);
        afisProdus(p);
        free(p.nume);
    }

    return 0;
}
