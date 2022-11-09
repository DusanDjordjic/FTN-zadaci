#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ARR_SIZE 20

typedef struct {
    char ime[16];
    unsigned int dan;
    unsigned int mesec;
    unsigned int broj_parcadi;
} rodjendan;

FILE* fopen_safe(const char* filename, const char* mode);
void ucitaj_rodjendane(FILE* fp, rodjendan* niz, int* n);
void rodjendan_print(rodjendan* rodj);

unsigned int ukupan_broj_parcadi(rodjendan* niz, int n);
void cestitaj_rodjendan(rodjendan* rodj);
void cestitaj_svima_rodjendan(rodjendan* niz, int n);
void testiraj_broj_torti(rodjendan* niz, int n);

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("Poziv ./divandan <ime_fajla>\n");
        exit(EXIT_FAILURE);
    }

    rodjendan niz[MAX_ARR_SIZE] = { 0 };
    int n = 0;
    FILE* fp = fopen_safe(argv[1], "r");

    ucitaj_rodjendane(fp, niz, &n);

    fclose(fp);

    for (int i = 0; i < n; i++) {
        rodjendan_print(niz + i);
    }

    testiraj_broj_torti(niz, n);
    cestitaj_svima_rodjendan(niz, n);

    return 0;
}

void testiraj_broj_torti(rodjendan* niz, int n)
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

void cestitaj_svima_rodjendan(rodjendan* niz, int n)
{
    for (int i = 0; i < n; i++) {
        cestitaj_rodjendan(niz + i);
    }
}

void cestitaj_rodjendan(rodjendan* rodj)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    if (rodj->mesec == tm.tm_mon + 1 && rodj->dan == tm.tm_mday) {
        printf("Sredan rodjendan %s\n", rodj->ime);
    }
}

unsigned int ukupan_broj_parcadi(rodjendan* niz, int n)
{
    unsigned int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += niz[i].broj_parcadi;
    }
    return sum;
}

void rodjendan_print(rodjendan* rodj)
{
    printf("Ime %s\nDan %d\nMesec %d\nBroj parcadi %d\n\n", rodj->ime, rodj->dan, rodj->mesec, rodj->broj_parcadi);
}

void ucitaj_rodjendane(FILE* fp, rodjendan* niz, int* n)
{
    while (*n < MAX_ARR_SIZE && fscanf(fp, "%s %d %d %d", niz[*n].ime, &niz[*n].dan, &niz[*n].mesec, &niz[*n].broj_parcadi) != EOF) {
        (*n)++;
    }
}

FILE* fopen_safe(const char* filename, const char* mode)
{
    FILE* fp = fopen(filename, mode);
    if (fp == NULL) {
        perror("Greska pri otvaranju fajla\n");
        exit(EXIT_FAILURE);
    }
    return fp;
}
