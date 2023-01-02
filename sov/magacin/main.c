#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAZIV 10

typedef struct magacin_st {
    unsigned int sifra_magacina;
    unsigned int sifra_proizvoda;
    char naziv_proizvoda[MAX_NAZIV];
    unsigned int cena;
    struct magacin_st* sledeci;
} Magacin;

FILE* safe_open(const char* filepath, const char* mode, const int error_code);
void list_init(Magacin** pglava);
void list_load(FILE* in, Magacin** pglava);
void list_push(Magacin** pglava, Magacin* novi);
void list_print(const Magacin* glava);
void list_free(Magacin** pglava);
Magacin* magacin_create(unsigned int sifra_magacina, unsigned int sifra_proizvida, char* naziv_proizvida, unsigned int cena);
void run(Magacin* glava);

int main(int argc, char** argv)
{
    if (argc != 2) {
        printf("Pogresan poziv programa");
        exit(1);
    }

    FILE* in = safe_open(argv[1], "r", 2);

    Magacin* glava;
    list_init(&glava);

    list_load(in, &glava);
    fclose(in);

    list_print(glava);
    run(glava);
    list_free(&glava);
    return 0;
}

void run(Magacin* glava)
{
    unsigned int sifra_magacina, sifra_proizvoda;

    printf("\nUnesite sifru magacina: ");
    scanf("%u", &sifra_magacina);
    printf("Unesite sifru proizvoda: ");
    scanf("%u", &sifra_proizvoda);

    unsigned int suma_cena = 0;
    unsigned int brojac_proizvoda = 0;
    char* naziv_proizvoda = NULL;

    while (glava != NULL) {
        if (glava->sifra_proizvoda == sifra_proizvoda
            && glava->sifra_magacina == sifra_magacina) {

            if (naziv_proizvoda == NULL) {
                naziv_proizvoda = glava->naziv_proizvoda;
            }

            suma_cena += glava->cena;
            brojac_proizvoda++;
        }
        glava = glava->sledeci;
    }

    if (suma_cena == 0 && brojac_proizvoda == 0) {
        printf("Za unete podatke od strane korisnika ne postoje podaci.\n");
    } else {
        printf("\nUkupno proizvoda sa sifrom %u, pod nazivom %s u magacinu sa sifrom %u ima %u.\nUkupna vrednost proizvoda je %u dinara.\n", sifra_proizvoda, naziv_proizvoda, sifra_magacina, brojac_proizvoda, suma_cena);
    }
}

FILE* safe_open(const char* filepath, const char* mode, const int error_code)
{
    FILE* fp = fopen(filepath, mode);

    if (fp == NULL) {
        printf("Greska pri otvaraju datoteke \"%s\"\n", filepath);
        exit(error_code);
    }

    return fp;
}

void list_init(Magacin** pglava)
{
    *pglava = NULL;
}

void list_load(FILE* in, Magacin** pglava)
{
    unsigned int sifra_magacina, sifra_proizvida, cena;
    char naziv_proizvida[MAX_NAZIV];
    Magacin* novi;
    while (fscanf(in, "%u %u %s %u", &sifra_magacina, &sifra_proizvida, naziv_proizvida, &cena) != EOF) {
        novi = magacin_create(sifra_magacina, sifra_proizvida, naziv_proizvida, cena);
        list_push(pglava, novi);
    }
}

Magacin* magacin_create(unsigned int sifra_magacina, unsigned int sifra_proizvoda, char* naziv_proizvoda, unsigned int cena)
{
    Magacin* novi = (Magacin*)malloc(sizeof(Magacin));
    if (novi == NULL) {
        printf("Greska pri alokaciji memorije\n");
        exit(3);
    }

    (*novi).sifra_magacina = sifra_magacina;
    novi->sifra_proizvoda = sifra_proizvoda;
    strcpy(novi->naziv_proizvoda, naziv_proizvoda);
    novi->cena = cena;
    novi->sledeci = NULL;

    return novi;
}

void list_push(Magacin** pglava, Magacin* novi)
{
    if (*pglava == NULL) {
        *pglava = novi;
        return;
    }

    // Dodaj na pocetak
    if (novi->sifra_magacina < (*pglava)->sifra_magacina) {
        novi->sledeci = *pglava;
        *pglava = novi;
        return;
    }

    Magacin* tekuci = *pglava;
    while (tekuci->sledeci != NULL && novi->sifra_magacina > tekuci->sledeci->sifra_magacina) {
        tekuci = tekuci->sledeci;
    }

    novi->sledeci = tekuci->sledeci;
    tekuci->sledeci = novi;
}

void list_print(const Magacin* glava)
{

    while (glava != NULL) {
        printf("%u %u %s %u\n", glava->sifra_magacina, glava->sifra_proizvoda, glava->naziv_proizvoda, glava->cena);
        glava = glava->sledeci;
    }
}

void list_free(Magacin** pglava)
{
    Magacin* tekuci;
    while (*pglava != NULL) {
        tekuci = *pglava;
        *pglava = (*pglava)->sledeci;
        free(tekuci);
    }
}
