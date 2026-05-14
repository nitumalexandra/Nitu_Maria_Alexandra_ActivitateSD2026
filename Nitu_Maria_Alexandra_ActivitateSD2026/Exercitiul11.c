#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Joc Joc;
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

struct Joc {
	int id;
	char* denumire;
	char* platforma;
	float rating;
};

struct NodPrincipal {
	Joc info;
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
		Nod* aux = ld->first;
		id = aux->id;

		ld->first = ld->first->next;

		if (ld->first == NULL) {
			ld->last = NULL;
		}
		else {
			ld->first->prev = NULL;
		}

		free(aux);
	}

	return id;
}

Joc initJoc(int id, const char* denumire, const char* platforma, float rating) {
	Joc j;

	j.id = id;

	j.denumire = (char*)malloc(strlen(denumire) + 1);
	strcpy(j.denumire, denumire);

	j.platforma = (char*)malloc(strlen(platforma) + 1);
	strcpy(j.platforma, platforma);

	j.rating = rating;

	return j;
}

void inserareListaPrincipala(NodPrincipal** cap, Joc j) {
	NodPrincipal* nou = (NodPrincipal*)malloc(sizeof(NodPrincipal));

	nou->info = j;
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

void afisareJoc(Joc j) {
	printf("\n%d %s %s %.2f", j.id, j.denumire, j.platforma, j.rating);
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
		afisareJoc(graf->info);

		printf("\nJocuri similare:");

		NodSecundar* vecini = graf->vecini;

		while (vecini) {
			afisareJoc(vecini->info->info);
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

		if (nodCurent) {
			afisareJoc(nodCurent->info);

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
	}

	free(vizitat);
}

void dezalocareGraf(NodPrincipal** graf) {
	while (*graf) {
		NodPrincipal* auxPrincipal = *graf;

		free(auxPrincipal->info.denumire);
		free(auxPrincipal->info.platforma);

		NodSecundar* vecini = auxPrincipal->vecini;

		while (vecini) {
			NodSecundar* auxVecin = vecini;
			vecini = vecini->next;
			free(auxVecin);
		}

		*graf = (*graf)->next;
		free(auxPrincipal);
	}
}

int main() {
	NodPrincipal* graf = NULL;

	inserareListaPrincipala(&graf, initJoc(1, "Minecraft", "PC", 9.5));
	inserareListaPrincipala(&graf, initJoc(2, "Terraria", "PC", 9.0));
	inserareListaPrincipala(&graf, initJoc(3, "FIFA", "PlayStation", 8.2));
	inserareListaPrincipala(&graf, initJoc(4, "NBA2K", "Xbox", 8.0));
	inserareListaPrincipala(&graf, initJoc(5, "Valorant", "PC", 8.7));
	inserareListaPrincipala(&graf, initJoc(6, "CounterStrike", "PC", 9.1));

	adaugaMuchie(graf, 1, 2);
	adaugaMuchie(graf, 3, 4);
	adaugaMuchie(graf, 5, 6);
	adaugaMuchie(graf, 1, 5);
	adaugaMuchie(graf, 2, 6);
	adaugaMuchie(graf, 4, 5);

	printf("\nAFISARE GRAF:");
	afisareGraf(graf);

	printf("\nDFS pornind de la jocul cu id-ul 1:");
	DFS(graf, 1);

	dezalocareGraf(&graf);

	return 0;
}