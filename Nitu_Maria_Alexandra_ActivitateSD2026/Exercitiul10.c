#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Film Film;
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

struct Film {
	int id;
	char* titlu;
	char* gen;
	float durata;
};

struct NodPrincipal {
	Film info;
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

Film initFilm(int id, const char* titlu, const char* gen, float durata) {
	Film f;

	f.id = id;

	f.titlu = (char*)malloc(strlen(titlu) + 1);
	strcpy(f.titlu, titlu);

	f.gen = (char*)malloc(strlen(gen) + 1);
	strcpy(f.gen, gen);

	f.durata = durata;

	return f;
}

void inserareListaPrincipala(NodPrincipal** cap, Film f) {
	NodPrincipal* nou = (NodPrincipal*)malloc(sizeof(NodPrincipal));

	nou->info = f;
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

void afisareFilm(Film f) {
	printf("\n%d %s %s %.2f", f.id, f.titlu, f.gen, f.durata);
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
		afisareFilm(graf->info);

		printf("\nVecini:");

		NodSecundar* vecini = graf->vecini;

		while (vecini) {
			afisareFilm(vecini->info->info);
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

		afisareFilm(nodCurent->info);

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

	inserareListaPrincipala(&graf, initFilm(1, "Inception", "SF", 148));
	inserareListaPrincipala(&graf, initFilm(2, "Avatar", "Adventure", 162));
	inserareListaPrincipala(&graf, initFilm(3, "Titanic", "Drama", 195));
	inserareListaPrincipala(&graf, initFilm(4, "Joker", "Thriller", 122));
	inserareListaPrincipala(&graf, initFilm(5, "Gladiator", "Action", 155));
	inserareListaPrincipala(&graf, initFilm(6, "Interstellar", "SF", 169));

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