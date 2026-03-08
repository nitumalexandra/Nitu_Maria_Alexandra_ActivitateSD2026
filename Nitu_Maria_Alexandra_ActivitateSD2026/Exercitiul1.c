#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Student Student;

struct Student {
    int id;
    float medie;
    int varsta;
    char* nume;
    char an;
};

Student initializare(int id, float medie, int varsta, const char* nume, char an) {
    Student s;
    s.id = id;
    s.medie = medie;
    s.varsta = varsta;
    s.an = an;

    if (nume) {
        s.nume = (char*)malloc(strlen(nume) + 1);
        strcpy(s.nume, nume);
    }
    else {
        s.nume = NULL;
    }

    return s;
}

Student copie(Student s) {
    return initializare(s.id, s.medie, s.varsta, s.nume, s.an);
}

void afis(Student s) {
    printf("%d. %s an %c varsta %d medie %.2f\n",
        s.id, s.nume ? s.nume : "NULL", s.an, s.varsta, s.medie);
}

void afisVect(Student* v, int n) {
    for (int i = 0; i < n; i++) {
        afis(v[i]);
    }
}

void afisNumeVect(Student* v, int n) {
    for (int i = 0; i < n; i++) {
        printf("%s\n", v[i].nume ? v[i].nume : "NULL");
    }
}

Student* copieN(Student* v, int n, int m) {
    if (!v || m <= 0 || m > n) return NULL;

    Student* nou = (Student*)malloc(sizeof(Student) * m);
    for (int i = 0; i < m; i++) {
        nou[i] = copie(v[i]);
    }
    return nou;
}

void dezaloc(Student** v, int* n) {
    if (!v || !(*v)) {
        if (n) *n = 0;
        return;
    }

    for (int i = 0; i < *n; i++) {
        free((*v)[i].nume);
    }

    free(*v);
    *v = NULL;
    *n = 0;
}

void copieMedie(Student* v, int n, float prag, Student** rez, int* dim) {
    *rez = NULL;
    *dim = 0;

    if (!v || n <= 0) return;

    for (int i = 0; i < n; i++) {
        if (v[i].medie >= prag) {
            (*dim)++;
        }
    }

    if (*dim == 0) return;

    *rez = (Student*)malloc(sizeof(Student) * (*dim));

    int k = 0;
    for (int i = 0; i < n; i++) {
        if (v[i].medie >= prag) {
            (*rez)[k] = copie(v[i]);
            k++;
        }
    }
}

Student cauta(Student* v, int n, const char* nume) {
    Student s;
    s.id = -1;
    s.nume = NULL;
    s.medie = 0;
    s.varsta = 0;
    s.an = 0;

    if (!v || !nume) return s;

    for (int i = 0; i < n; i++) {
        if (v[i].nume && strcmp(v[i].nume, nume) == 0) {
            return copie(v[i]);
        }
    }

    return s;
}

int main() {
    Student s1 = initializare(1, 9.45f, 20, "Mara", '2');
    afis(s1);

    int n = 5;
    Student* v = (Student*)malloc(sizeof(Student) * n);

    v[0] = initializare(2, 8.10f, 19, "Andrei", '1');
    v[1] = initializare(3, 7.55f, 21, "Ioana", '3');
    v[2] = initializare(4, 9.90f, 20, "Radu", '2');
    v[3] = initializare(5, 6.40f, 22, "Bianca", '3');
    v[4] = initializare(6, 8.75f, 19, "Ioana", '1');

    printf("\nVector\n");
    afisVect(v, n);

    int m = 3;
    Student* primii = copieN(v, n, m);

    printf("\nPrimii %d:\n", m);
    if (primii) afisNumeVect(primii, m);
    dezaloc(&primii, &m);

    Student* filtr = NULL;
    int dim = 0;
    float prag = 8.50f;

    copieMedie(v, n, prag, &filtr, &dim);

    printf("\nMedia peste %.2f o au:\n", prag);
    if (filtr) afisNumeVect(filtr, dim);
    dezaloc(&filtr, &dim);

    const char* numeCautat = "Ioana";
    Student gasit = cauta(v, n, numeCautat);

    printf("\nNume cautat este: %s\n", numeCautat);
    if (gasit.id != -1) {
        printf("Gasit: %s\n", gasit.nume);
        free(gasit.nume);
    }
    else {
        printf("Nu exista\n");
    }

    free(s1.nume);
    dezaloc(&v, &n);

    return 0;
}