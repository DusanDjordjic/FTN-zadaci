#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ARR_SIZE 20

typedef struct {
    char id[11];
    char polazak[4];
    char dolazak[4];
    unsigned int duzina;
    unsigned int broj_presedanja;
    double cena;
} Let;

FILE* otvori_datoteku(const char* name, const char* mode);
int ucitaj_letove(Let* niz, FILE* fp, int n);
int filtriraj_po_dolasku(Let* let, const char* dolazak);

int main(int argc, char* argv[])
{
    Let niz[MAX_ARR_SIZE] = { 0 };
    int n = 0;
    if (argc != 3) {
        printf("Koriscenje: ./let <fajl_sa_letovime> <id_leta>");
        exit(EXIT_FAILURE);
    }

    FILE* input_file = otvori_datoteku(argv[1], "r");
    n = ucitaj_letove(niz, input_file, n);
    fclose(input_file);

    char outfile_name[18];
    sprintf(outfile_name, "letovi_ka_%s.txt", argv[2]);

    FILE* outfile = otvori_datoteku(outfile_name, "w");

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

int filtriraj_po_dolasku(Let* let, const char* dolazak)
{
    return strncmp(let->dolazak, dolazak, 4);
}

int ucitaj_letove(Let* niz, FILE* fp, int n)
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

FILE* otvori_datoteku(const char* name, const char* mode)
{

    FILE* fp = fopen(name, mode);
    if (fp == NULL) {
        perror("Greska pri otvaranju falja\n");
        exit(EXIT_FAILURE);
    }

    return fp;
}
