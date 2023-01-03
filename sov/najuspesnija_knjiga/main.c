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
    struct knjiga_st* sledeci;
} Knjiga;

FILE* safe_open(const char* filepath, const char* mode, const int error_code);
void* safe_malloc(const unsigned long int size, const int error_code);
Knjiga* knjiga_create(const char* ime_pisca, const char* prezime_pisca, const char* naslov, uint cena, uint prodato);
void list_init(Knjiga** phead);
void list_push(Knjiga** phead, Knjiga* new);
void list_load(FILE* fp, Knjiga** phead);
void list_print(Knjiga* head);
void list_free(Knjiga** phead);
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
    list_init(&head);

    FILE* in = safe_open(argv[1], "r", 2);
    list_load(in, &head);
    fclose(in);
    ispisi_ukupne_zarade(head);

    list_free(&head);
    return 0;
}

void list_init(Knjiga** phead)
{
    *phead = NULL;
}

int list_order(const Knjiga* k1, const Knjiga* k2)
{
    return k1->cena - k2->cena;
}

void list_free(Knjiga** phead)
{
    Knjiga* tmp;
    while (*phead != NULL) {
        tmp = *phead;
        *phead = (*phead)->sledeci;
        free(tmp);
    }
}

void list_push(Knjiga** phead, Knjiga* new)
{
    if (*phead == NULL) {
        *phead = new;
        return;
    }

    if (list_order(new, *phead) <= 0) {
        new->sledeci = *phead;
        *phead = new;
        return;
    }

    Knjiga* tekuci = *phead;
    while (tekuci->sledeci != NULL && list_order(new, tekuci->sledeci) > 0) {
        tekuci = tekuci->sledeci;
    }

    new->sledeci = tekuci->sledeci;
    tekuci->sledeci = new;
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
    new->sledeci = NULL;

    return new;
}

void list_load(FILE* fp, Knjiga** phead)
{
    Knjiga tmp;
    Knjiga* new;
    while (fscanf(fp, "%s %s %s %u %u", tmp.ime_pisca, tmp.prezime_pisca, tmp.naslov_knjige, &tmp.cena, &tmp.prodato) != EOF) {
        new = knjiga_create(tmp.ime_pisca, tmp.prezime_pisca, tmp.naslov_knjige, tmp.cena, tmp.prodato);
        list_push(phead, new);
    }
}

void list_print(Knjiga* head)
{
    while (head != NULL) {
        knjiga_print(head);
        head = head->sledeci;
    }
}

void knjiga_print(Knjiga* k)
{
    printf("%s %s %s %u %u\n", k->ime_pisca, k->prezime_pisca, k->naslov_knjige, k->cena, k->prodato);
}

uint knjiga_ukupna_zarada(Knjiga* head)
{
    return head->cena * head->prodato;
}

void ispisi_ukupne_zarade(Knjiga* head)
{
    Knjiga* najbolja = NULL;
    uint najbolja_ukupna_zarada = 0;

    uint ukupna_zarada = 0;
    while (head != NULL) {
        ukupna_zarada = knjiga_ukupna_zarada(head);
        printf("%s %s %s %u\n", head->prezime_pisca, head->ime_pisca, head->naslov_knjige, ukupna_zarada);

        if (najbolja == NULL) {
            najbolja = head;
            najbolja_ukupna_zarada = ukupna_zarada;
        }

        if (ukupna_zarada > najbolja_ukupna_zarada) {
            najbolja = head;
            najbolja_ukupna_zarada = ukupna_zarada;
        }

        head = head->sledeci;
    }

    if (najbolja == NULL) {
        // Samo u slucaju da je lista prazna
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
