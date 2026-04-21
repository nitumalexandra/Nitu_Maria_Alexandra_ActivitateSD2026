#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Angajat {
    int id;
    char* nume;
    float salariu;
    char* departament;
    char* dataAngajarii;
};
typedef struct Angajat Angajat;

typedef struct Nod Nod;
typedef struct Lista Lista;

struct Nod {
    Angajat a;
    Nod* next;
    Nod* prev;
};

struct Lista {
    Nod* prim;
    Nod* ultim;
};

Angajat citireAngajat(FILE* f) {
    char buf[200];
    char* tok;
    Angajat a;
    fgets(buf, 200, f);
    tok = strtok(buf, ",\n");
    a.id = atoi(tok);
    tok = strtok(NULL, ",\n");
    a.nume = _strdup(tok);
    tok = strtok(NULL, ",\n");
    a.salariu = atof(tok);
    tok = strtok(NULL, ",\n");
    a.departament = _strdup(tok);
    tok = strtok(NULL, ",\n");
    a.dataAngajarii = _strdup(tok);
    return a;
}

void afisareAngajat(Angajat a) {
    printf("ID: %d\nNume: %s\nSalariu: %.2f\nDepartament: %s\nData angajarii: %s\n\n", a.id, a.nume, a.salariu, a.departament, a.dataAngajarii);
}

void afisareLista(Lista l) {
    Nod* p = l.prim;
    while (p) {
        afisareAngajat(p->a);
        p = p->next;
    }
}

void afisareInversa(Lista l) {
    Nod* p = l.ultim;
    while (p) {
        afisareAngajat(p->a);
        p = p->prev;
    }
}

void adaugaAngajat(Lista* l, Angajat a) {
    Nod* n = malloc(sizeof(Nod));
    n->a = a;
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

Lista* citireLista(const char* fisier) {
    FILE* f = fopen(fisier, "r");
    if (f) {
        Lista* l = malloc(sizeof(Lista));
        l->prim = l->ultim = NULL;
        while (!feof(f)) {
            adaugaAngajat(l, citireAngajat(f));
        }
        fclose(f);
        return l;
    }
    return NULL;
}

float salariuMediu(Lista l) {
    float suma = 0;
    int count = 0;
    Nod* p = l.prim;
    while (p) {
        suma += p->a.salariu;
        count++;
        p = p->next;
    }
    return count > 0 ? suma / count : 0;
}

void stergeAngajat(Lista* l, int id) {
    Nod* p = l->prim;
    while (p && p->a.id != id) p = p->next;
    if (p) {
        if (p == l->prim) l->prim = p->next;
        if (p == l->ultim) l->ultim = p->prev;
        if (p->prev) p->prev->next = p->next;
        if (p->next) p->next->prev = p->prev;
        free(p->a.nume);
        free(p->a.departament);
        free(p->a.dataAngajarii);
        free(p);
    }
}

Angajat* angajatMaxSalariu(Lista l) {
    if (l.prim) {
        Angajat* max = &l.prim->a;
        Nod* p = l.prim->next;
        while (p) {
            if (p->a.salariu > max->salariu) {
                max = &p->a;
            }
            p = p->next;
        }
        return max;
    }
    return NULL;
}

int main() {
    Lista* l = citireLista("angajatiEx5.txt");
    afisareInversa(*l);
    printf("Salariu mediu: %.2f\n", salariuMediu(*l));

    Angajat* maxSalariu = angajatMaxSalariu(*l);
    if (maxSalariu) {
        printf("Angajatul cu cel mai mare salariu:\n");
        afisareAngajat(*maxSalariu);
    }

    stergeAngajat(l, 2);
    stergeAngajat(l, 4);

    afisareInversa(*l);
    free(l);
    return 0;
}