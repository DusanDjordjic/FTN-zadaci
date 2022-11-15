#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Pogledaj vise na https://en.cppreference.com/w/c/error/assert
 * Sve u svemu to je standardizovani makro koji govori da li smo u debug modu ili ne */
#define NDEBUG
#define MAX_ARR_SIZE 20

typedef struct {
    char sifra[6];
    char zauzetost[9];
    unsigned int cena;
    double tezina;
} Sef;

FILE* fopen_safe(const char* name, const char* mode);
void sef_ucitaj(FILE* fp, Sef* niz, int* n, double max_tezina);
void sef_print(FILE* fp, Sef* sef);
int sef_broj_slobodnih(Sef* niz, int n);

int main(int argc, char* argv[])
{
    if (argc != 4) {
        perror("Pogresna upotreba: ./sefovi <ulazni_fajl> <max_tezina> <izlazni_fajl>\n");
        exit(EXIT_FAILURE);
    }

    /* Parsiranje argumeneta */

    /* Napomena: Nismo morali da pravimo
     * input_file_name i output_file_name promenljive,
     * vec smo mogli da koristimo odmah argv[1] i argv[3],
     * ali ovako je lepse :D */

    const char* input_file_name = argv[1];

    /* Pogledaj `man strtod` */
    double max_tezina = strtod(argv[2], NULL);

    if (max_tezina == 0) {
        perror("Pogresan argument za <max_tezinu>\n");
        exit(EXIT_FAILURE);
    }

    /* Pogledaj `man errno` */
    if (errno == ERANGE) {
        perror("Vrednost za <max_tezinu> je prevelika ili premala\n");
        exit(EXIT_FAILURE);
    }

    const char* output_file_name = argv[3];

    Sef niz[MAX_ARR_SIZE] = { 0 };
    int n = 0;

    /* Ucitavanje sefova */
    FILE* input_file = fopen_safe(input_file_name, "r");
    sef_ucitaj(input_file, niz, &n, max_tezina);
    fclose(input_file);

    /* Ispis sefova na stdout */
    /* fprintf prima FILE* u koji ce da ispise text
     * sko posaljemo stdout tj. Standard output stream
     * On ce nam to ispisati u terminalu */

    /* ifndef guards https://stackoverflow.com/questions/10077025/ifndef-syntax-for-include-guards-in-c */

#ifndef NDEBUG
    for (int i = 0; i < n; i++) {
        sef_print(stdout, niz + i);
    }
#endif

    /* Ispis sefova u fajl */
    FILE* output_file = fopen_safe(output_file_name, "w");

    fprintf(output_file, "Cene sefova nakon oporezivanja:\n\n");
    for (int i = 0; i < n; i++) {
        sef_print(output_file, niz + i);
    }
    fprintf(output_file, "\nBroj slobodnih sefova: %d", sef_broj_slobodnih(niz, n));

    fclose(output_file);
    return 0;
}

void sef_print(FILE* fp, Sef* sef)
{
    fprintf(fp, "%s %s %u %.1lf\n", sef->sifra, sef->zauzetost, sef->cena, sef->tezina);
}

int sef_broj_slobodnih(Sef* niz, int n)
{
    int brojac = 0;
    for (int i = 0; i < n; i++) {
        if (strcmp(niz[i].zauzetost, "slobodan") == 0) {
            brojac++;
        }
    }

    return brojac;
}

void sef_ucitaj(FILE* fp, Sef* niz, int* n, double max_tezina)
{
    while (*n < MAX_ARR_SIZE && fscanf(fp, "%s %s %u %lf", niz[*n].sifra, niz[*n].zauzetost, &niz[*n].cena, &niz[*n].tezina) != EOF) {
        if (strcmp(niz[*n].zauzetost, "zauzet") == 0 && niz[*n].tezina > max_tezina) {
            // Racunamo novu cenu kao 115% od stare
            unsigned int stara_cena = niz[*n].cena;
            niz[*n].cena = round(1.15 * niz[*n].cena);
        }
        (*n)++;
    }
}

FILE* fopen_safe(const char* name, const char* mode)
{
    FILE* fp = fopen(name, mode);
    if (fp == NULL) {
        perror("Greska pri otvaranju fajla\n");
        exit(EXIT_FAILURE);
    }

    return fp;
}
