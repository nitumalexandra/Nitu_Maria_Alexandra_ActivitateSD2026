#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Produs {
    int id;
    char nume[100]; 
    float pret;
    int cantitate;
};
typedef struct Produs Produs;

typedef struct Nod Nod;
typedef struct Lista Lista;

struct Nod {
    Produs p;
    Nod* next;
    Nod* prev;
};

struct Lista {
    Nod* prim;
    Nod* ultim;
};

void adaugaProdus(Lista* l, Produs p) {
    Nod* n = malloc(sizeof(Nod));
    if (n == NULL) {
        printf("Eroare la alocarea memoriei pentru nod\n");
        return;
    }
    n->p = p;
    n->next = n->prev = NULL;
    if (l->ultim) {
        n->prev = l->ultim;
        l->ultim->next = n;
        l->ultim = n;
    }
    else {
        l->ultim = l->prim = n;
    }
}

void afisareProduse(Lista l) {
    Nod* p = l.prim;
    while (p) {
        printf("ID: %d, Nume: %s, Pret: %.2f, Cantitate: %d\n", p->p.id, p->p.nume, p->p.pret, p->p.cantitate);
        p = p->next;
    }
}

void stergeProdus(Lista* l, int id) {
    Nod* p = l->prim;
    while (p && p->p.id != id) p = p->next;
    if (p) {
        if (p == l->prim) l->prim = p->next;
        if (p == l->ultim) l->ultim = p->prev;
        if (p->prev) p->prev->next = p->next;
        if (p->next) p->next->prev = p->prev;

        free(p);
    }
}

float valoareStoc(Lista l) {
    float suma = 0;
    Nod* p = l.prim;
    while (p) {
        suma += p->p.pret * p->p.cantitate;
        p = p->next;
    }
    return suma;
}

void dezalocareLista(Lista* l) {
    Nod* p = l->prim;
    while (p) {
        Nod* temp = p;
        p = p->next;
        free(temp);
    }
    l->prim = l->ultim = NULL;
}

int main() {
    Lista lista;
    lista.prim = NULL;
    lista.ultim = NULL;

    Produs p1 = { 1, "Laptop", 3500.50, 10 };
    adaugaProdus(&lista, p1);
    Produs p2 = { 2, "Telefon", 1500.75, 20 };
    adaugaProdus(&lista, p2);
    Produs p3 = { 3, "Tableta", 800.00, 15 };
    adaugaProdus(&lista, p3);

    printf("Produse in magazin:\n");
    afisareProduse(lista);
    printf("\nValoarea totala a stocului: %.2f\n", valoareStoc(lista));
    printf("\nStergem produsul cu ID 2:\n");
    stergeProdus(&lista, 2);
    afisareProduse(lista);
    printf("\nValoarea totala a stocului dupa stergere: %.2f\n", valoareStoc(lista));

    dezalocareLista(&lista);

    return 0;
}