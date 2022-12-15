#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAZIV 21
#define MAX_IZDAVAC 21
#define MAX_NIZ_KNJIGA 30
#define MAX_NIZ_DUZINACITANJA MAX_NIZ_KNJIGA

typedef struct {
    char naziv[MAX_NAZIV];
    char izdavac[MAX_IZDAVAC];
    unsigned int broj_strana;
} Knjiga;

typedef struct {
    double br_dana;
    char naziv[MAX_NAZIV];
} DuzinaCitanja;

FILE* safe_open(const char* const filepath, const char* const mode, const int error_code);
void ucitaj_knjige(FILE* file, Knjiga* const niz, int* const n);
void print_duzina_citanja(FILE* file, const DuzinaCitanja* const duzina_citanja);
void izracunaj_duzine_citanja(const Knjiga* const niz_knjiga, const int niz_knjiga_len, DuzinaCitanja* const niz_duzina_citanja, const double brzina_citanja);
void ispisi_duzine_citanja(FILE* output_file, const DuzinaCitanja* const niz_duzina_citanja, const int niz_duzina_citanja_len);

int main(int argc, char** argv)
{
    if (argc != 4) {
        fprintf(stderr, "Pogresan poziv. Primer: %s <broj_strana> <ulazna_dataoteka> <izlazna_datoteka>\n", argv[0]);
        exit(1);
    }

    char* endptr = argv[1];
    double brzina_citanja = strtod(argv[1], &endptr);

    if (*endptr != '\0') {
        fprintf(stderr, "Brzina citanja nije broj\n");
        exit(2);
    }

    if (brzina_citanja <= 0) {
        fprintf(stderr, "Brzina citanja mora biti veca od 0\n");
        exit(3);
    }

    const char* input_file_name = argv[2];
    const char* output_file_name = argv[3];

    Knjiga niz_knjiga[MAX_NIZ_KNJIGA] = { 0 };
    DuzinaCitanja niz_duzina_citanja[MAX_NIZ_DUZINACITANJA] = { 0 };
    int niz_knjiga_len = 0;

    FILE* input_file = safe_open(input_file_name, "r", 4);
    ucitaj_knjige(input_file, niz_knjiga, &niz_knjiga_len);
    fclose(input_file);

    izracunaj_duzine_citanja(niz_knjiga, niz_knjiga_len, niz_duzina_citanja, brzina_citanja);

    FILE* output_file = safe_open(output_file_name, "w", 5);

    int niz_duzina_citanja_len = niz_knjiga_len;
    ispisi_duzine_citanja(output_file, niz_duzina_citanja, niz_duzina_citanja_len);

    fclose(output_file);
    return 0;
}

void ispisi_duzine_citanja(FILE* output_file, const DuzinaCitanja* const niz_duzina_citanja, const int niz_duzina_citanja_len)
{
    for (int i = 0; i < niz_duzina_citanja_len; i++) {
        print_duzina_citanja(output_file, niz_duzina_citanja + i);
    }
}

void izracunaj_duzine_citanja(const Knjiga* const niz_knjiga, const int niz_knjiga_len, DuzinaCitanja* const niz_duzina_citanja, const double brzina_citanja)
{
    for (int i = 0; i < niz_knjiga_len; i++) {
        niz_duzina_citanja[i].br_dana = niz_knjiga[i].broj_strana / (brzina_citanja * 24);
        strncpy(niz_duzina_citanja[i].naziv, niz_knjiga[i].naziv, MAX_NAZIV);
    }
}

FILE* safe_open(const char* const filepath, const char* const mode, const int error_code)
{
    FILE* fp = fopen(filepath, mode);
    if (fp == NULL) {
        fprintf(stderr, "Greska pri otvaranju fajla \"%s\"\n", filepath);
        exit(error_code);
    }

    return fp;
}

void ucitaj_knjige(FILE* file, Knjiga* const niz, int* const n)
{
    while (*n < MAX_NIZ_KNJIGA
        && fscanf(file, "%s %s %u",
               niz[*n].naziv,
               niz[*n].izdavac,
               &niz[*n].broj_strana)
            != EOF) {
        *n += 1;
    }
}

void print_duzina_citanja(FILE* file, const DuzinaCitanja* const duzina_citanja)
{
    fprintf(file, "%.2lf %s\n", duzina_citanja->br_dana, duzina_citanja->naziv);
}
