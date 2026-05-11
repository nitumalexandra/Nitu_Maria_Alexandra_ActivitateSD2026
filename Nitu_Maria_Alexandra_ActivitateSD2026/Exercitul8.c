#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Aeroport Aeroport;
typedef struct NodPrincipal NodPrincipal;
typedef struct NodSecundar NodSecundar;

typedef struct Nod Nod;
typedef struct ListaDubla ListaDubla;

struct Nod {
	int cod;
	Nod* next;
	Nod* prev;
};

struct ListaDubla {
	Nod* first;
	Nod* last;
};

struct Aeroport {
	int cod;
	char* nume;
	char* tara;
	int nrTerminale;
};

struct NodPrincipal {
	Aeroport info;
	NodPrincipal* next;
	NodSecundar* vecini;
};

struct NodSecundar {
	NodPrincipal* info;
	NodSecundar* next;
};

void push(ListaDubla* ld, int cod) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->cod = cod;
	nou->next = ld->first;
	nou->prev = NULL;

	if (ld->first == NULL) {
		ld->first = ld->last = nou;
	}
	else {
		ld->first->prev = nou;
		ld->first = nou;
	}
}

int pop(ListaDubla* ld) {
	int cod = -1;

	if (ld->first) {
		cod = ld->first->cod;

		Nod* aux = ld->first;
		ld->first = ld->first->next;

		if (ld->first == NULL) {
			ld->last = NULL;
		}

		free(aux);
	}

	return cod;
}

Aeroport initAeroport(int cod, const char* nume, const char* tara, int nrTerminale) {
	Aeroport a;

	a.cod = cod;

	a.nume = (char*)malloc(strlen(nume) + 1);
	strcpy(a.nume, nume);

	a.tara = (char*)malloc(strlen(tara) + 1);
	strcpy(a.tara, tara);

	a.nrTerminale = nrTerminale;

	return a;
}

void inserareListaPrincipala(NodPrincipal** cap, Aeroport a) {
	NodPrincipal* nou = (NodPrincipal*)malloc(sizeof(NodPrincipal));

	nou->info = a;
	nou->next = NULL;
	nou->vecini = NULL;

	if (*cap) {
		NodPrincipal* aux = *cap;

		while (aux->next) {
			aux = aux->next;
		}

		aux->next = nou;
	}
	else {
		*cap = nou;
	}
}

void inserareListaSecundara(NodSecundar** cap, NodPrincipal* info) {
	NodSecundar* nou = (NodSecundar*)malloc(sizeof(NodSecundar));

	nou->info = info;
	nou->next = NULL;

	if (*cap) {
		NodSecundar* aux = *cap;

		while (aux->next) {
			aux = aux->next;
		}

		aux->next = nou;
	}
	else {
		*cap = nou;
	}
}

void afisareAeroport(Aeroport a) {
	printf("\n%d %s %s %d",
		a.cod,
		a.nume,
		a.tara,
		a.nrTerminale);
}

NodPrincipal* cautaDupaCod(NodPrincipal* graf, int cod) {
	while (graf && graf->info.cod != cod) {
		graf = graf->next;
	}

	return graf;
}

void adaugaMuchie(NodPrincipal* graf, int cod1, int cod2) {
	NodPrincipal* nod1 = cautaDupaCod(graf, cod1);
	NodPrincipal* nod2 = cautaDupaCod(graf, cod2);

	if (nod1 && nod2) {
		inserareListaSecundara(&nod1->vecini, nod2);
		inserareListaSecundara(&nod2->vecini, nod1);
	}
}

void afisareGraf(NodPrincipal* graf) {
	while (graf) {
		afisareAeroport(graf->info);

		printf("\nVecini:");

		NodSecundar* vecini = graf->vecini;

		while (vecini) {
			afisareAeroport(vecini->info->info);
			vecini = vecini->next;
		}

		printf("\n");

		graf = graf->next;
	}
}

void DFS(NodPrincipal* graf, int start) {
	int dim = 6;

	int* vizitat = (int*)malloc(sizeof(int) * dim);

	for (int i = 0; i < dim; i++) {
		vizitat[i] = 0;
	}

	ListaDubla stiva;
	stiva.first = NULL;
	stiva.last = NULL;

	push(&stiva, start);
	vizitat[start - 1] = 1;

	while (stiva.first) {
		int extras = pop(&stiva);

		NodPrincipal* nodCurent = cautaDupaCod(graf, extras);

		afisareAeroport(nodCurent->info);

		NodSecundar* vecini = nodCurent->vecini;

		while (vecini) {
			int codVecin = vecini->info->info.cod;

			if (vizitat[codVecin - 1] == 0) {
				push(&stiva, codVecin);
				vizitat[codVecin - 1] = 1;
			}

			vecini = vecini->next;
		}
	}

	free(vizitat);
}

int main() {
	NodPrincipal* graf = NULL;

	inserareListaPrincipala(&graf, initAeroport(1, "Otopeni", "Romania", 2));
	inserareListaPrincipala(&graf, initAeroport(2, "Heathrow", "UK", 5));
	inserareListaPrincipala(&graf, initAeroport(3, "Fiumicino", "Italia", 4));
	inserareListaPrincipala(&graf, initAeroport(4, "Barajas", "Spania", 3));
	inserareListaPrincipala(&graf, initAeroport(5, "Schiphol", "Olanda", 6));
	inserareListaPrincipala(&graf, initAeroport(6, "CharlesDeGaulle", "Franta", 4));

	adaugaMuchie(graf, 1, 2);
	adaugaMuchie(graf, 1, 3);
	adaugaMuchie(graf, 2, 4);
	adaugaMuchie(graf, 3, 5);
	adaugaMuchie(graf, 4, 6);
	adaugaMuchie(graf, 5, 6);

	afisareGraf(graf);

	printf("\nDFS:");
	DFS(graf, 1);

	return 0;
}