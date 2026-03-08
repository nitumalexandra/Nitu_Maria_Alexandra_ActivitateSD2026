#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraCarte {
	int id;
	int nrPagini;
	float pret;
	char* titlu;
	char* autor;
	unsigned char categorie;
};

typedef struct StructuraCarte Carte;
typedef struct Nod Nod;
typedef struct ListaDubla ListaDubla;

struct Nod {
	Carte info;
	Nod* next;
	Nod* prev;
};

struct ListaDubla {
	Nod* prim;
	Nod* ultim;
};

Carte citireCarteDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;

	Carte c;

	aux = strtok(buffer, sep);
	c.id = atoi(aux);
	c.nrPagini = atoi(strtok(NULL, sep));
	c.pret = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	c.titlu = malloc(strlen(aux) + 1);
	strcpy(c.titlu, aux);

	aux = strtok(NULL, sep);
	c.autor = malloc(strlen(aux) + 1);
	strcpy(c.autor, aux);

	c.categorie = *strtok(NULL, sep);

	return c;
}

void afisareCarte(Carte carte) {
	printf("Id: %d\n", carte.id);
	printf("Pagini: %d\n", carte.nrPagini);
	printf("Pret: %.2f\n", carte.pret);
	printf("Titlu: %s\n", carte.titlu);
	printf("Autor: %s\n", carte.autor);
	printf("Categorie: %c\n\n", carte.categorie);
}

void afisareListaCarti(ListaDubla lista) {
	Nod* p = lista.prim;
	while (p) {
		afisareCarte(p->info);
		p = p->next;
	}
}

void afisareInversaListaCarti(ListaDubla lista) {
	Nod* p = lista.ultim;
	while (p) {
		afisareCarte(p->info);
		p = p->prev;
	}
}

void adaugaCarteInLista(ListaDubla* lista, Carte carteNoua) {
	Nod* nou = malloc(sizeof(Nod));
	nou->info = carteNoua;
	nou->next = nou->prev = NULL;

	if (lista->ultim) {
		nou->prev = lista->ultim;
		lista->ultim->next = nou;
		lista->ultim = nou;
	}
	else {
		lista->prim = lista->ultim = nou;
	}
}

ListaDubla* citireLDCartiDinFisier(const char* numeFisier) {

	FILE* file = fopen(numeFisier, "r");

	ListaDubla* lista = malloc(sizeof(ListaDubla));
	lista->prim = NULL;
	lista->ultim = NULL;

	if (file) {
		while (!feof(file)) {
			adaugaCarteInLista(lista, citireCarteDinFisier(file));
		}
		fclose(file);
	}
	return lista;
}

void dezalocareLDCarti(ListaDubla** lista) {

	Nod* p = (*lista)->prim;

	while (p) {
		free(p->info.titlu);
		free(p->info.autor);
		Nod* aux = p;
		p = p->next;
		free(aux);
	}
	free(*lista);
	*lista = NULL;
}

float calculeazaPretMediu(ListaDubla lista) {
	float suma = 0;
	int count = 0;
	Nod* p = lista.prim;

	while (p) {
		suma += p->info.pret;
		count++;
		p = p->next;
	}

	if (count > 0)
		return suma / count;
	else
		return 0;
}

void stergeCarteDupaID(ListaDubla* lista, int id) {

	if (lista->prim && lista->prim->info.id == id) {
		Nod* aux = lista->prim;
		lista->prim = aux->next;

		if (lista->prim)
			lista->prim->prev = NULL;
		else
			lista->ultim = NULL;

		free(aux->info.titlu);
		free(aux->info.autor);
		free(aux);

		return;
	}

	Nod* p = lista->prim;
	while (p && p->info.id != id) {
		p = p->next;
	}

	if (p) {
		p->prev->next = p->next;
		if (p->next)
			p->next->prev = p->prev;
		else
			lista->ultim = p->prev;
		free(p->info.titlu);
		free(p->info.autor);
		free(p);
	}
}

char* getAutorCarteScumpa(ListaDubla lista) {

	if (lista.prim) {

		float pretMax = lista.prim->info.pret;
		char* autor = lista.prim->info.autor;

		Nod* p = lista.prim->next;

		while (p) {

			if (p->info.pret > pretMax) {
				pretMax = p->info.pret;
				autor = p->info.autor;
			}
			p = p->next;
		}
		char* rezultat = malloc(strlen(autor) + 1);
		strcpy(rezultat, autor);
		return rezultat;
	}
	return NULL;
}

int main() {
	ListaDubla* lista = citireLDCartiDinFisier("carti.txt");

	afisareInversaListaCarti(*lista);
	printf("Pret mediu: %.2f\n", calculeazaPretMediu(*lista));

	printf("Autor carte scumpa: %s\n", getAutorCarteScumpa(*lista));
	stergeCarteDupaID(lista, 3);

	printf("\nLista dupa stergere:\n");

	afisareListaCarti(*lista);
	dezalocareLDCarti(&lista);

	return 0;
}