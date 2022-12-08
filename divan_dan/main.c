#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ARR_SIZE 20
#define MAX_IME_SIZE 16

typedef struct {
    char ime[MAX_IME_SIZE];
    unsigned int dan;
    unsigned int mesec;
    unsigned int broj_parcadi;
} rodjendan;

FILE* fopen_safe(const char* const filename, const char* const mode, const int error_code);
void ucitaj_rodjendane(FILE* fp, rodjendan* const niz, int* n);
void rodjendan_print(const rodjendan* const rodj);
unsigned int ukupan_broj_parcadi(const rodjendan* const niz, const int n);
void cestitaj_rodjendan(const rodjendan* const rodj);
void cestitaj_svima_rodjendan(const rodjendan* const niz, const int n);
void testiraj_broj_torti(const rodjendan* const niz, const int n);

int main(int argc, char* argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Poziv: %s <ime_fajla>\n", argv[0]);
        exit(1);
    }

    rodjendan niz[MAX_ARR_SIZE] = { 0 };
    int n = 0;

    FILE* fp = fopen_safe(argv[1], "r", 2);
    ucitaj_rodjendane(fp, niz, &n);
    fclose(fp);

    for (int i = 0; i < n; i++) {
        rodjendan_print(niz + i);
    }

    testiraj_broj_torti(niz, n);
    cestitaj_svima_rodjendan(niz, n);

    return 0;
}

void testiraj_broj_torti(const rodjendan* const niz, const int n)
{
    unsigned int t;
    printf("Unesite broj torti (1 torta = 10 parcadi): ");
    scanf("%u", &t);

    unsigned int ukupno = ukupan_broj_parcadi(niz, n);

    if (t * 10 < ukupno) {
        printf("%u totri nije dovoljno da svi budu srecni :(\n", t);
    } else {
        printf("%u totri je dovoljno da svi budu srecni :)\n", t);
    }
}

void cestitaj_svima_rodjendan(const rodjendan* const niz, const int n)
{
    for (int i = 0; i < n; i++) {
        cestitaj_rodjendan(niz + i);
    }
}

void cestitaj_rodjendan(const rodjendan* const rodj)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    if (rodj->mesec == tm.tm_mon + 1 && rodj->dan == tm.tm_mday) {
        printf("Sredan rodjendan %s\n", rodj->ime);
    }
}

unsigned int ukupan_broj_parcadi(const rodjendan* const niz, const int n)
{
    unsigned int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += niz[i].broj_parcadi;
    }
    return sum;
}

void rodjendan_print(const rodjendan* const rodj)
{
    printf("Ime %s\nDan %d\nMesec %d\nBroj parcadi %d\n\n", rodj->ime, rodj->dan, rodj->mesec, rodj->broj_parcadi);
}

void ucitaj_rodjendane(FILE* fp, rodjendan* const niz, int* n)
{
    while (*n < MAX_ARR_SIZE && fscanf(fp, "%s %d %d %d", niz[*n].ime, &niz[*n].dan, &niz[*n].mesec, &niz[*n].broj_parcadi) != EOF) {
        (*n)++;
    }
}

FILE* fopen_safe(const char* const filename, const char* const mode, const int error_code)
{
    FILE* fp = fopen(filename, mode);
    if (fp == NULL) {
        perror("Greska pri otvaranju fajla\n");
        exit(error_code);
    }
    return fp;
}
