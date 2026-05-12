#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Muzeu Muzeu;
typedef struct NodPrincipal NodPrincipal;
typedef struct NodSecundar NodSecundar;
typedef struct Nod Nod;
typedef struct ListaDubla ListaDubla;

struct Nod {
	int id;
	Nod* next;
	Nod* prev;
};

struct ListaDubla {
	Nod* first;
	Nod* last;
};

struct Muzeu {
	int id;
	char* denumire;
	char* oras;
	float pretBilet;
};

struct NodPrincipal {
	Muzeu info;
	NodPrincipal* next;
	NodSecundar* vecini;
};

struct NodSecundar {
	NodPrincipal* info;
	NodSecundar* next;
};

void push(ListaDubla* ld, int id) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->id = id;
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
	int id = -1;

	if (ld->first) {
		id = ld->first->id;
		Nod* aux = ld->first;
		ld->first = ld->first->next;

		if (ld->first == NULL) {
			ld->last = NULL;
		}

		free(aux);
	}

	return id;
}

Muzeu initMuzeu(int id, const char* denumire, const char* oras, float pretBilet) {
	Muzeu m;

	m.id = id;

	m.denumire = (char*)malloc(strlen(denumire) + 1);
	strcpy(m.denumire, denumire);

	m.oras = (char*)malloc(strlen(oras) + 1);
	strcpy(m.oras, oras);

	m.pretBilet = pretBilet;

	return m;
}

void inserareListaPrincipala(NodPrincipal** cap, Muzeu m) {
	NodPrincipal* nou = (NodPrincipal*)malloc(sizeof(NodPrincipal));

	nou->info = m;
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

void afisareMuzeu(Muzeu m) {
	printf("\n%d %s %s %.2f",
		m.id,
		m.denumire,
		m.oras,
		m.pretBilet);
}

NodPrincipal* cautaDupaId(NodPrincipal* graf, int id) {
	while (graf && graf->info.id != id) {
		graf = graf->next;
	}

	return graf;
}

void adaugaMuchie(NodPrincipal* graf, int id1, int id2) {
	NodPrincipal* nod1 = cautaDupaId(graf, id1);
	NodPrincipal* nod2 = cautaDupaId(graf, id2);

	if (nod1 && nod2) {
		inserareListaSecundara(&nod1->vecini, nod2);
		inserareListaSecundara(&nod2->vecini, nod1);
	}
}

void afisareGraf(NodPrincipal* graf) {
	while (graf) {
		afisareMuzeu(graf->info);

		printf("\nVecini:");

		NodSecundar* vecini = graf->vecini;

		while (vecini) {
			afisareMuzeu(vecini->info->info);
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

		NodPrincipal* nodCurent = cautaDupaId(graf, extras);

		afisareMuzeu(nodCurent->info);

		NodSecundar* vecini = nodCurent->vecini;

		while (vecini) {
			int idVecin = vecini->info->info.id;

			if (vizitat[idVecin - 1] == 0) {
				push(&stiva, idVecin);
				vizitat[idVecin - 1] = 1;
			}

			vecini = vecini->next;
		}
	}

	free(vizitat);
}

int main() {
	NodPrincipal* graf = NULL;

	inserareListaPrincipala(&graf, initMuzeu(1, "Antipa", "Bucuresti", 35));
	inserareListaPrincipala(&graf, initMuzeu(2, "Luvru", "Paris", 90));
	inserareListaPrincipala(&graf, initMuzeu(3, "Prado", "Madrid", 70));
	inserareListaPrincipala(&graf, initMuzeu(4, "Uffizi", "Florenta", 65));
	inserareListaPrincipala(&graf, initMuzeu(5, "Vatican", "Roma", 80));
	inserareListaPrincipala(&graf, initMuzeu(6, "Rijksmuseum", "Amsterdam", 75));

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