#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ARR_SIZE 20
#define MAX_ID_SIZE 11
#define MAX_POLAZAK_SIZE 4
#define MAX_DOLAZAK_SIZE 4

typedef struct {
    char id[MAX_ID_SIZE];
    char polazak[MAX_POLAZAK_SIZE];
    char dolazak[MAX_DOLAZAK_SIZE];
    unsigned int duzina;
    unsigned int broj_presedanja;
    double cena;
} Let;

FILE* otvori_datoteku(const char* const name, const char* mode, const int error_code);
int ucitaj_letove(FILE* fp, Let* const niz, int n);
int filtriraj_po_dolasku(const Let* let, const char* dolazak);

int main(int argc, char* argv[])
{
    if (argc != 3) {
        printf("Poziv: %s <fajl_sa_letovime> <id_leta>", argv[0]);
        exit(1);
    }

    Let niz[MAX_ARR_SIZE] = { 0 };
    int n = 0;

    FILE* input_file = otvori_datoteku(argv[1], "r", 2);
    n = ucitaj_letove(input_file, niz, n);
    fclose(input_file);

    char outfile_name[18];

    // sprintf(outfile_name, "letovi_ka_%s.txt", argv[2]);
    strcat(outfile_name, "letovi_ka_");
    strncat(outfile_name, argv[2], 3);
    strcat(outfile_name, ".txt");

    FILE* outfile = otvori_datoteku(outfile_name, "w", 3);

    int pronadjenih_letova = 0;

    for (int i = 0; i < n; i++) {
        if (filtriraj_po_dolasku(niz + i, argv[2]) == 0) {
            pronadjenih_letova++;
            fprintf(outfile, "%s %s %.2lf\n", niz[i].id, niz[i].polazak, niz[i].cena);
        }
    }

    if (pronadjenih_letova == 0) {
        fprintf(outfile, "Nema letova ka zadatom aeodromu!\n");
    }

    fclose(outfile);

    return 0;
}

int filtriraj_po_dolasku(const Let* let, const char* dolazak)
{
    return strncmp(let->dolazak, dolazak, MAX_DOLAZAK_SIZE);
}

int ucitaj_letove(FILE* fp, Let* const niz, int n)
{
    while (
        n < MAX_ARR_SIZE
        && fscanf(fp, "%s %s %s %u %u",
               niz[n].id,
               niz[n].polazak,
               niz[n].dolazak,
               &niz[n].duzina,
               &niz[n].broj_presedanja)
            != EOF) {

        // Izracunaj cenu leta
        if ((niz + n)->duzina < 5000) {
            (niz + n)->cena = (niz + n)->duzina * 0.5;
        } else if ((niz + n)->duzina < 10000) {
            (niz + n)->cena = (niz + n)->duzina * 0.75;
        } else {
            (niz + n)->cena = (niz + n)->duzina * 1.0;
        }

        (niz + n)->cena -= (niz + n)->broj_presedanja * 5;

        n++;
    }

    if (n == MAX_ARR_SIZE) {
        printf("Ucitano maksimalan broj letova %d\n", MAX_ARR_SIZE);
    }

    return n;
}

FILE* otvori_datoteku(const char* const name, const char* const mode, const int error_code)
{

    FILE* fp = fopen(name, mode);
    if (fp == NULL) {
        fprintf(stderr, "Greska pri otvaranju falja %s\n", name);
        exit(error_code);
    }

    return fp;
}
