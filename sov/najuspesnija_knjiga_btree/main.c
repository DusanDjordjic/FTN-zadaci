#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_IME_PISCA (20 + 1)
#define MAX_PREZIME_PISCA MAX_IME_PISCA
#define MAX_NASLOV (30 + 1)

typedef unsigned int uint;

typedef struct knjiga_st {
    char ime_pisca[MAX_IME_PISCA];
    char prezime_pisca[MAX_PREZIME_PISCA];
    char naslov_knjige[MAX_NASLOV];
    uint cena;
    uint prodato;
    struct knjiga_st* levi;
    struct knjiga_st* desni;
} Knjiga;

FILE* safe_open(const char* filepath, const char* mode, const int error_code);
void* safe_malloc(const unsigned long int size, const int error_code);
Knjiga* knjiga_create(const char* ime_pisca, const char* prezime_pisca, const char* naslov, uint cena, uint prodato);
void tree_init(Knjiga** phead);
void tree_push(Knjiga** phead, Knjiga* new);
void tree_load(FILE* fp, Knjiga** phead);
void tree_print(Knjiga* head);
void tree_free(Knjiga** phead);
void knjiga_print(Knjiga* k);
uint knjiga_ukupna_zarada(Knjiga* k);
void ispisi_ukupne_zarade(Knjiga* head);

int main(int argc, char** argv)
{
    if (argc != 2) {
        printf("Greska pri pozivu programa\n");
        exit(1);
    }

    Knjiga* head;
    tree_init(&head);

    FILE* in = safe_open(argv[1], "r", 2);
    tree_load(in, &head);
    fclose(in);

    tree_print(head);
    ispisi_ukupne_zarade(head);
    tree_free(&head);
    return 0;
}

void tree_init(Knjiga** phead)
{
    *phead = NULL;
}

int tree_order(const Knjiga* k1, const Knjiga* k2)
{
    return k1->cena - k2->cena;
}

void tree_free(Knjiga** phead)
{
    if (*phead == NULL) {
        return;
    }

    tree_free(&(*phead)->levi);
    tree_free(&(*phead)->desni);
    free(*phead);
}

void tree_push(Knjiga** phead, Knjiga* new)
{
    if (*phead == NULL) {
        *phead = new;
        return;
    }

    if (tree_order(new, *phead) > 0)
        tree_push(&(*phead)->desni, new);
    else
        tree_push(&(*phead)->levi, new);
}

void* safe_malloc(const unsigned long int size, const int error_code)
{
    void* buffer = malloc(size);
    if (buffer == NULL) {
        printf("Greska pri alokaciji memorije\n");
        exit(error_code);
    }

    return buffer;
}

FILE* safe_open(const char* filepath, const char* mode, const int error_code)
{
    FILE* fp = fopen(filepath, mode);
    if (fp == NULL) {
        printf("Greska pri otvaranju fajla \"%s\"\n", filepath);
        exit(error_code);
    }

    return fp;
}

Knjiga* knjiga_create(const char* ime_pisca, const char* prezime_pisca, const char* naslov, uint cena, uint prodato)
{
    Knjiga* new = (Knjiga*)safe_malloc(sizeof(Knjiga), 3);
    strcpy(new->ime_pisca, ime_pisca);
    strcpy(new->prezime_pisca, prezime_pisca);
    strcpy(new->naslov_knjige, naslov);
    new->cena = cena;
    new->prodato = prodato;
    new->levi = NULL;
    new->desni = NULL;

    return new;
}

void tree_load(FILE* fp, Knjiga** phead)
{
    Knjiga tmp;
    Knjiga* new;
    while (fscanf(fp, "%s %s %s %u %u", tmp.ime_pisca, tmp.prezime_pisca, tmp.naslov_knjige, &tmp.cena, &tmp.prodato) != EOF) {
        new = knjiga_create(tmp.ime_pisca, tmp.prezime_pisca, tmp.naslov_knjige, tmp.cena, tmp.prodato);
        tree_push(phead, new);
    }
}

void tree_print(Knjiga* head)
{
    if (head == NULL) {
        return;
    }

    tree_print(head->levi);
    knjiga_print(head);
    tree_print(head->desni);
}

void knjiga_print(Knjiga* k)
{
    printf("%s %s %s %u\n", k->prezime_pisca, k->ime_pisca, k->naslov_knjige, knjiga_ukupna_zarada(k));
}

uint knjiga_ukupna_zarada(Knjiga* head)
{
    if (head == NULL)
        return 0;

    return head->cena * head->prodato;
}

Knjiga* najbolja_knjiga(Knjiga* head)
{
    if (head == NULL) {
        return NULL;
    }
    uint moja_zarada = knjiga_ukupna_zarada(head);

    Knjiga* naj_leva = najbolja_knjiga(head->levi);
    uint leva_zarada = knjiga_ukupna_zarada(naj_leva);

    Knjiga* naj_desna = najbolja_knjiga(head->desni);
    uint desna_zarada = knjiga_ukupna_zarada(naj_desna);

    if (moja_zarada >= leva_zarada && moja_zarada >= desna_zarada) {
        return head;
    }

    if (leva_zarada >= moja_zarada && leva_zarada >= desna_zarada) {
        return naj_leva;
    }

    if (desna_zarada >= moja_zarada && desna_zarada >= leva_zarada) {
        return naj_desna;
    }

    return NULL;
}

void ispisi_ukupne_zarade(Knjiga* head)
{
    Knjiga* najbolja = najbolja_knjiga(head);

    if (najbolja == NULL) {
        printf("Ne postoji knjiga koja je zaradila najvise novca\n");
        return;
    }

    printf("Knjiga koja je zaradila najvise novca je %s %s %s %u %u.\n",
        najbolja->ime_pisca,
        najbolja->prezime_pisca,
        najbolja->naslov_knjige,
        najbolja->cena,
        najbolja->prodato);
}
