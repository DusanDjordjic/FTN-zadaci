#include <stdio.h>
#include <stdlib.h>

#define MAX_DRZAVAAA 21
#define MAX_VRSTA_HRANE 21
#define MAX_NIZ 30

typedef struct tim_st {
    char drzava[MAX_DRZAVAAA];
    char vrsta_hrane[MAX_VRSTA_HRANE];
    double ocena_publike;
    double ocena_zirija;
} Tim;

FILE* safe_open(const char* const filename, const char* const mode, const int error_code);
int ucitaj_timove(FILE* file, Tim* const timovi, int n);
void print_tim(const Tim* const tim);
void sledeci_krug(const Tim* const timovi, const int len, const double prolaz);

int main(int argc, char** argv)
{
    if (argc != 3) {
        fprintf(stderr, "Greska pri pozivu: Primer %s <input_file> <bodovi>\n", argv[0]);
        exit(1);
    }

    char* input_file_name = argv[1];

    char* endptr = argv[2];
    double poeni_za_prolazak = strtod(argv[2], &endptr);

    if (*endptr != '\0') {
        printf("Broj poena nije realan broj\n");
        exit(2);
    }

    if (poeni_za_prolazak <= 0) {
        printf("Broj poena mora biti veci od 0\n");
        exit(3);
    }

    FILE* input_file = safe_open(input_file_name, "r", 4);
    Tim timovi[MAX_NIZ] = { 0 };

    int timovi_len = 0;
    timovi_len = ucitaj_timove(input_file, timovi, timovi_len);

    sledeci_krug(timovi, timovi_len, poeni_za_prolazak);

    fclose(input_file);
    return 0;
}

void sledeci_krug(const Tim* const timovi, const int len, const double prolaz)
{
    double ukupna_ocena;
    double prosecna_ocena = 0;
    int sledeci_krug = 0;
    for (int i = 0; i < len; i++) {
        ukupna_ocena = timovi[i].ocena_zirija + timovi[i].ocena_publike;
        if (ukupna_ocena >= prolaz) {
            prosecna_ocena += ukupna_ocena;
            sledeci_krug++;
            print_tim(timovi + i);
        }
    }

    prosecna_ocena /= sledeci_krug;
    printf("Prosecna ocena je %.2lf\n", prosecna_ocena);
}

void print_tim(const Tim* const tim)
{
    printf("%s %s %.2lf %.2lf\n", tim->drzava, tim->vrsta_hrane, tim->ocena_publike, tim->ocena_zirija);
}

FILE* safe_open(const char* const filename, const char* const mode, const int error_code)
{
    FILE* file = fopen(filename, mode);
    if (file == NULL) {
        fprintf(stderr, "Greska pri otvaranju fajla \"%s\"\n", filename);
        exit(error_code);
    }

    return file;
}

int ucitaj_timove(FILE* file, Tim* const timovi, int n)
{
    while (n < MAX_NIZ
        && fscanf(file, "%s %s %lf %lf",
               timovi[n].drzava,
               timovi[n].vrsta_hrane,
               &timovi[n].ocena_publike,
               &timovi[n].ocena_zirija)
            != EOF) {
        n++;
    }

    return n;
}
