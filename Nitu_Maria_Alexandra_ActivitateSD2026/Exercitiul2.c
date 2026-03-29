#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct structuracarte {
	int id;
	int nrpagini;
	float pret;
	char* titlu;
	char* autor;
	unsigned char categorie;
};

typedef struct structuracarte carte;
typedef struct nod nod;
typedef struct listadubla listadubla;

struct nod {
	carte info;
	nod* next;
	nod* prev;
};

struct listadubla {
	nod* prim;
	nod* ultim;
};

carte citirecartedinfisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;

	carte c;

	aux = strtok(buffer, sep);
	c.id = atoi(aux);
	c.nrpagini = atoi(strtok(NULL, sep));
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

void afisarecarte(carte carte) {
	printf("id: %d\n", carte.id);
	printf("pagini: %d\n", carte.nrpagini);
	printf("pret: %.2f\n", carte.pret);
	printf("titlu: %s\n", carte.titlu);
	printf("autor: %s\n", carte.autor);
	printf("categorie: %c\n\n", carte.categorie);
}

void afisarelistacarti(listadubla lista) {
	nod* p = lista.prim;
	while (p) {
		afisarecarte(p->info);
		p = p->next;
	}
}

void afisareinversalistacarti(listadubla lista) {
	nod* p = lista.ultim;
	while (p) {
		afisarecarte(p->info);
		p = p->prev;
	}
}

void adaugacarteinlista(listadubla* lista, carte cartenoua) {
	nod* nou = malloc(sizeof(nod));
	nou->info = cartenoua;
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

listadubla* citireldcartidinfisier(const char* numefisier) {

	FILE* file = fopen(numefisier, "r");

	listadubla* lista = malloc(sizeof(listadubla));
	lista->prim = NULL;
	lista->ultim = NULL;

	if (file) {
		while (!feof(file)) {
			adaugacarteinlista(lista, citirecartedinfisier(file));
		}
		fclose(file);
	}
	return lista;
}

void dezalocareldcarti(listadubla** lista) {

	nod* p = (*lista)->prim;

	while (p) {
		free(p->info.titlu);
		free(p->info.autor);
		nod* aux = p;
		p = p->next;
		free(aux);
	}
	free(*lista);
	*lista = NULL;
}

float calculeazapretmediu(listadubla lista) {
	float suma = 0;
	int count = 0;
	nod* p = lista.prim;

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

void stergecartedupaid(listadubla* lista, int id) {

	if (lista->prim && lista->prim->info.id == id) {
		nod* aux = lista->prim;
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

	nod* p = lista->prim;
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

char* getautorcartescumpa(listadubla lista) {

	if (lista.prim) {

		float pretmax = lista.prim->info.pret;
		char* autor = lista.prim->info.autor;

		nod* p = lista.prim->next;

		while (p) {

			if (p->info.pret > pretmax) {
				pretmax = p->info.pret;
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
	listadubla* lista = citireldcartidinfisier("cartiex2.txt");

	afisareinversalistacarti(*lista);
	printf("pret mediu: %.2f\n", calculeazapretmediu(*lista));

	printf("autor carte scumpa: %s\n", getautorcartescumpa(*lista));
	stergecartedupaid(lista, 3);

	printf("\nlista dupa stergere:\n");

	afisarelistacarti(*lista);
	dezalocareldcarti(&lista);

	return 0;
}