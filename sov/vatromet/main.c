#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAZIV (20 + 1)

typedef struct vatromet_st {
    char naziv[MAX_NAZIV];
    unsigned glasnoca;
    unsigned cena;

    struct vatromet_st* levi;
    struct vatromet_st* desni;

} VATROMET;

FILE* safe_open(char* name, char* mode, int error_code)
{
    FILE* dat = fopen(name, mode);
    if (dat == NULL) {
        printf("Datoteku %s nije moguce otvoriti!\n", name);
        exit(error_code);
    }
    return dat;
}

void inicijalizacija(VATROMET** koren)
{
    *koren = NULL;
}

VATROMET* napravi_cvor(char* naziv, unsigned glasnoca, unsigned cena)
{
    VATROMET* novi = (VATROMET*)malloc(sizeof(VATROMET));
    if (novi == NULL) {
        printf("Nije moguce zauzeti memoriju!\n");
        exit(12);
    }
    strcpy(novi->naziv, naziv);
    novi->glasnoca = glasnoca;
    novi->cena = cena;

    novi->levi = NULL;
    novi->desni = NULL;
    return novi;
}

void dodaj_novi(VATROMET** koren, VATROMET* novi)
{
    if (*koren == NULL) {
        *koren = novi;
    } else { // u tekstu pise da sortiramo po ceni u rastucem poretku
        if ((*koren)->cena > novi->cena) {
            dodaj_novi(&(*koren)->levi, novi);
        } else {
            dodaj_novi(&(*koren)->desni, novi);
        }
    }
}

void ispisi_stablo(VATROMET* koren)
{
    if (koren != NULL) {
        ispisi_stablo(koren->levi);
        printf("%s %u %u\n", koren->naziv, koren->glasnoca, koren->cena);
        ispisi_stablo(koren->desni);
    }
}

//./a.out 110 vatrometi.txt izlaz.txt
int main(int brArg, char** argument)
{
    if (brArg != 4) {
        printf("Los unos argumenata!\n");
        exit(11);
    }
    FILE *in, *out;
    in = safe_open(argument[2], "r", 1);
    out = safe_open(argument[3], "w", 2);

    VATROMET* koren;
    inicijalizacija(&koren);

    fclose(in);
    fclose(out);
    return 0;
}
