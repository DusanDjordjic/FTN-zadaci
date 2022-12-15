#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* Makroi*/
#define MAX_ARR_SIZE 20
/* Deklaracije funkcija i tipova */

typedef struct {
    char grad[31];
    char zona[7];
    char registracija[9];
    unsigned int broj_provedenih_sati;
} parking;

// Saljemo const char* da ne bi slucajno promenili stringove
// Takodje je dobra praksa koristiti const da bi naglasili sta treba a sta ne treba da se menja

FILE* fopen_safe(const char* const filename, const char* const mode, const int error_code);
void ucitaj_parkinge(FILE* fp, parking* const niz_parkinga, int* const n);
void parking_print(const parking* const parking);
int total_parking_price_by_zone(const parking* const niz, const int n, const char* const zone, const int zone_price);

int main(int argc, char* argv[])
{

    /*
     * argc -> broj poslatih argumenata.
     *      Posto mi hocemo da nam neko posalje naziv fajla sa vozilima
     *      ocekujemo 2 argumenta jer je prvi argument uvek fajl koji izvrsavamo.
     *      Na primer `a.out vozila.txt`
     *      a.out (nas program) je prvi argument
     *      vozila.txt je drugi argument
     *
     * argv -> niz stringova, sadrzi poslate argumente
     *
     * */

    if (argc != 2) {
        fprintf(stderr, "Pogresan poziv: %s <ime_fajla>\n", argv[0]);
        exit(1);
    }

    FILE* parking_fp = fopen_safe(argv[1], "r", 2);

    // Svu memoriju popunimo sa 0
    parking niz[MAX_ARR_SIZE] = { 0 };
    // Mogli smo da uradimo i sledece
    // memset(niz, 0, sizeof(niz));

    // Broj clanova niza na pocetku je 0
    int n = 0;

    /*
     * Ispis ucitanih parkinga parkinga
     * Korisno za proveru da li se sve ucitalo kako treba
     *
     * */

    // for (int i = 0; i < n; i++) {
    //     parking_print(niz + i);
    // }

    // PAZNJA! saljemo adresu od n (&n) jer
    // hocemo da ga promenimo unutar funkcije ucitaj_parkinge.
    ucitaj_parkinge(parking_fp, niz, &n);

    printf("Ukupna cena parkinga za vozila u crvenoj zoni je %d dinara\n",
        total_parking_price_by_zone(niz, n, "crvena", 60));

    printf("Ukupna cena parkinga za vozila u zutoj zoni je %d dinara\n",
        total_parking_price_by_zone(niz, n, "zuta", 55));

    printf("Ukupna cena parkinga za vozila u zelenoj zoni je %d dinara\n",
        total_parking_price_by_zone(niz, n, "zelena", 40));

    fclose(parking_fp);
    return 0;
}

int total_parking_price_by_zone(const parking* const niz, const int n, const char* const zone, const int zone_price)
{
    int sum = 0;
    for (int i = 0; i < n; i++) {
        if (strcmp(niz[i].zona, zone) == 0) {
            sum += zone_price * niz[i].broj_provedenih_sati;
        }
    }

    return sum;
}

void parking_print(const parking* const parking)
{
    printf("Grad %s\nZona %s\nRegistracija %s\nBroj Sati %u\n\n",
        parking->grad,
        parking->zona,
        parking->registracija,
        parking->broj_provedenih_sati);
}

void ucitaj_parkinge(FILE* fp, parking* const niz_parkinga, int* n)
{

    // Ovde idemo dok je *n < MAX_ARR_SIZE jer je to
    // maksimalan broj elemenata koje niz moze da primi

    // Pogledaj `man fscanf`

    while (*n < MAX_ARR_SIZE
        && fscanf(fp, "%s %s %s %u",
               niz_parkinga[*n].grad,
               niz_parkinga[*n].zona,
               niz_parkinga[*n].registracija,
               &niz_parkinga[*n].broj_provedenih_sati)
            != EOF) {
        (*n)++;
    }
}

FILE* fopen_safe(const char* const filename, const char* const mode, const int error_code)
{
    FILE* fp = fopen(filename, mode);
    if (fp == NULL) {
        // perror ispisuje poslat string i ispisuje gresku na koju je naisao.
        // Izprisi ili promeni ime fajlu koji hoces da citas i videces sta ce ispisati
        perror("Greska pri otvaranju fajla");
        exit(error_code);
    }

    return fp;
}
