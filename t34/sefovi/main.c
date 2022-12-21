#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Pogledaj vise na https://en.cppreference.com/w/c/error/assert
 * Sve u svemu to je standardizovani makro koji govori da li smo u debug modu ili ne */
#define NDEBUG
#define MAX_ARR_SIZE 20
#define MAX_SIFRA 6
#define MAX_ZAU 9

typedef struct {
    char sifra[MAX_SIFRA];
    char zauzetost[MAX_ZAU];
    unsigned int cena;
    double tezina;
} Sef;

FILE* fopen_safe(const char* const name, const char* const mode, const int error_code);
void sef_ucitaj(FILE* fp, Sef* const niz, int* const n, const double max_tezina);
void sef_print(FILE* fp, const Sef* const sef);
int sef_broj_slobodnih(const Sef* const niz, const int n);

int main(int argc, char* argv[])
{
    if (argc != 4) {
        fprintf(stderr, "Pogresna upotreba: %s <ulazni_fajl> <max_tezina >= 0> <izlazni_fajl>\n", argv[0]);
        exit(1);
    }

    /* Parsiranje argumeneta */

    /* Napomena: Nismo morali da pravimo
     * input_file_name i output_file_name promenljive,
     * vec smo mogli da koristimo odmah argv[1] i argv[3],
     * ali ovako je lepse :D */

    const char* input_file_name = argv[1];
    const char* output_file_name = argv[3];

    /* Pogledaj `man strtod` */
    const double max_tezina = strtod(argv[2], NULL);

    if (max_tezina < 5) {
        printf("Greska");
        exit(2);
    }

    if (max_tezina <= 0) {
        fprintf(stderr, "Pogresan argument za <max_tezinu >= 0> \n");
        exit(2);
    }

    /* Pogledaj `man errno` */
    if (errno == ERANGE) {
        fprintf(stderr, "Vrednost za <max_tezinu> je prevelika ili premala\n");
        exit(3);
    }

    Sef niz[MAX_ARR_SIZE];
    int n = 0;

    /* Ucitavanje sefova */
    FILE* input_file = fopen_safe(input_file_name, "r", 4);
    sef_ucitaj(input_file, niz, &n, max_tezina);
    fclose(input_file);

    /* Ispis sefova na stdout */
    /* fprintf prima FILE* u koji ce da ispise text
     * ako posaljemo stdout tj. Standard output stream
     * On ce nam to ispisati u terminalu */

    /* Ispis sefova u fajl */
    FILE* output_file = fopen_safe(output_file_name, "w", 5);

    fprintf(output_file, "Cene sefova nakon oporezivanja:\n\n");
    for (int i = 0; i < n; i++) {
        sef_print(output_file, niz + i);
    }

    fprintf(output_file, "\nBroj slobodnih sefova: %d", sef_broj_slobodnih(niz, n));

    fclose(output_file);
    return 0;
}

void sef_print(FILE* fp, const Sef* const sef)
{
    fprintf(fp, "%s %s %u %.1lf\n", sef->sifra, sef->zauzetost, sef->cena, sef->tezina);
}

int sef_broj_slobodnih(const Sef* const niz, const int n)
{
    int brojac = 0;
    for (int i = 0; i < n; i++) {
        if (strcmp(niz[i].zauzetost, "slobodan") == 0) {
            brojac++;
        }
    }

    return brojac;
}

void sef_ucitaj(FILE* fp, Sef* const niz, int* const n, const double max_tezina)
{
    while (*n < MAX_ARR_SIZE && fscanf(fp, "%s %s %u %lf", niz[*n].sifra, niz[*n].zauzetost, &niz[*n].cena, &niz[*n].tezina) != EOF) {
        if (strcmp(niz[*n].zauzetost, "zauzet") == 0 && niz[*n].tezina > max_tezina) {
            // Racunamo novu cenu kao 115% od stare
            niz[*n].cena = round(1.15 * niz[*n].cena);
        }
        (*n)++;
    }
}

FILE* fopen_safe(const char* const name, const char* const mode, const int error_code)
{
    FILE* fp = fopen(name, mode);
    if (fp == NULL) {
        fprintf(stderr, "Greska pri otvaranju fajla \"%s\"\n", name);
        exit(error_code);
    }

    return fp;
}
