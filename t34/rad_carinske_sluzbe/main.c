#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIFRA 6
#define MAX_ZEMLJA 11
#define MAX_VREDNOST 4
#define MAX_NIZ 30
#define MAX_ODB_ZEMLJA 11

typedef struct posiljka_st {
    char sifra[MAX_SIFRA];
    char zemlja[MAX_ZEMLJA];
    char vrednost[MAX_VREDNOST];
    double tezina;
    double cena;
} Posiljka;

void clear()
{
    while (fgetc(stdin) != '\n') { };
}

FILE* safe_open(const char* const filename, const char* const mode, const int error_code);
int posiljke_ucitaj(FILE* file, Posiljka* const niz, int n);
void posiljke_print(const Posiljka* const niz, const int len);
int main(int argc, char** argv)
{
    if (argc != 3) {
        fprintf(stderr, "Pogresan poziv %s <input_file> <output_file>\n", argv[0]);
        exit(1);
    }

    char* input_file_name = argv[1];
    char* output_file_name = argv[2];

    FILE* input_file = safe_open(input_file_name, "r", 2);

    Posiljka niz_posiljki[MAX_NIZ] = { 0 };
    int len = 0;

    len = posiljke_ucitaj(input_file, niz_posiljki, len);

    fclose(input_file);

    printf("Odaberite zemlju: ");
    char odabrana_zemlja[MAX_ODB_ZEMLJA] = { 0 };

    fgets(odabrana_zemlja, MAX_ODB_ZEMLJA, stdin);
    if (strlen(odabrana_zemlja) >= MAX_ODB_ZEMLJA - 1) {
        clear();
    }

    printf("%s\n", odabrana_zemlja);

    posiljke_print(niz_posiljki, len);

    return 0;
}

void posiljke_print(const Posiljka* const niz, const int len)
{
    for (int i = 0; i < len; i++) {
        printf("%s %s %s %.2lf %.2lf\n",
            niz[i].sifra,
            niz[i].zemlja,
            niz[i].vrednost,
            niz[i].tezina,
            niz[i].cena);
    }
}

int posiljke_ucitaj(FILE* file, Posiljka* const niz, int n)
{
    while (n < MAX_NIZ
        && fscanf(file, "%s %s %s %lf",
               niz[n].sifra,
               niz[n].vrednost,
               niz[n].zemlja,
               &niz[n].tezina)
            != EOF) {
        niz[n].cena = strtod(niz[n].vrednost, NULL);
        n++;
    }

    return n;
}
FILE* safe_open(const char* const filename, const char* const mode, const int error_code)
{
    FILE* file = fopen(filename, mode);
    if (file == NULL) {
        fprintf(stderr, "Greska pri otvaranju fajla %s\n", filename);
        exit(error_code);
    }

    return file;
}
