#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ARR_SIZE 20
#define MAX_MODE_SIZE 21
#define MAX_VRSTA_SIZE 21

typedef struct {
    char model[MAX_MODE_SIZE];
    char vrsta[MAX_VRSTA_SIZE];
    unsigned int kubikaza;
    unsigned int budzet;
} automobil_st;

FILE* otvori_datoteku(const char* datoteka);
int ucitaj_automobile(FILE* fp, automobil_st* niz, int n);
void ispisi_automobil(automobil_st* automobil);

int main(int argc, char** argv)
{
    if (argc != 2) {
        printf("autoservis <ime_fajla>\n");
        exit(EXIT_FAILURE);
    }

    automobil_st niz[MAX_ARR_SIZE] = { 0 };
    int n = 0;

    FILE* fp = otvori_datoteku(argv[1]);

    n = ucitaj_automobile(fp, niz, n);
    for (int i = 0; i < n; i++) {
        if (strcmp("MALI", niz[i].vrsta) == 0) {
            if (niz[i].kubikaza < 1300) {
                if (niz[i].budzet >= 5000) {
                    ispisi_automobil(niz + i);
                }
            } else if (niz[i].kubikaza < 1800) {
                if (niz[i].budzet >= 8000) {
                    ispisi_automobil(niz + i);
                }
            } else {
                if (niz[i].budzet >= 12000) {
                    ispisi_automobil(niz + i);
                }
            }
        } else if (strcmp("VELIKI", niz[i].vrsta) == 0) {
            if (niz[i].kubikaza < 1300) {
                if (niz[i].budzet >= 10000) {
                    ispisi_automobil(niz + i);
                }
            } else if (niz[i].kubikaza < 1800) {
                if (niz[i].budzet >= 15000) {
                    ispisi_automobil(niz + i);
                }
            } else {
                if (niz[i].budzet >= 25000) {
                    ispisi_automobil(niz + i);
                }
            }

        } else {
            printf("ERROR: Uneli ste ne odgovarajucu vrstu servisa\n");
            printf("Moguci servisi su \"MALI\" i \"VELIKI\"\n");
            ispisi_automobil(niz + i);
        }
    }

    fclose(fp);
    return 0;
}

FILE* otvori_datoteku(const char* datoteka)
{
    FILE* f = fopen(datoteka, "rw");
    if (f == NULL) {
        perror("Greska prilikom otvaranja datoteke.\n");
        exit(EXIT_FAILURE);
    }

    return f;
}

int ucitaj_automobile(FILE* fp, automobil_st* niz, int n)
{
    while (n < MAX_ARR_SIZE && fscanf(fp, "%s %s %u %u", niz[n].model, niz[n].vrsta, &(niz[n].kubikaza), &(niz[n].budzet)) != EOF) {
        n++;
    }
    return n;
}

void ispisi_automobil(automobil_st* automobil)
{
    printf("Model: %s\nVrsta Servisa %s\nKubikaza %u\nBudzet vlasnika %u\n\n", automobil->model, automobil->vrsta, automobil->kubikaza, automobil->budzet);
}
