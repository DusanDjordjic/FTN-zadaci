#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PRUZAOC 21
#define MAX_USLUGA 11
#define MAX_FILEPATH (13 + MAX_USLUGA)
#define MAX_TELEFON 21

typedef struct usluga_st Usluga;
typedef struct node_st Node;

struct usluga_st {
    char pruzaoc[MAX_PRUZAOC];
    char usluga[MAX_USLUGA];
    char telefon[MAX_TELEFON];
};

struct node_st {
    Usluga usluga;
    Node* next;
};

FILE* safe_open(const char* filepath, const char* mode, const int error_code);
void ucitaj_usluge(FILE* file, Node** head);
void list_free(Node** head);
char desifruj_karakter(char c);
Usluga desifruj_broj_telefona(Usluga usluga);
void ispisi_desifrovano(Node* head, char* usl);

int main(int argc, char** argv)
{
    if (argc != 3) {
        fprintf(stderr, "Pogresan poziv. Primer %s <input_file> <service>\n", argv[0]);
        exit(1);
    }

    char* input_file_name = argv[1];
    char* service = argv[2];

    FILE* input_file = safe_open(input_file_name, "r", 2);
    Node* head = NULL;
    ucitaj_usluge(input_file, &head);
    fclose(input_file);
    ispisi_desifrovano(head, service);
    list_free(&head);

    return 0;
}

void ispisi_desifrovano(Node* head, char* service)
{
    char filepath[MAX_FILEPATH] = { 0 };
    strcat(filepath, "telefoni-");
    strncat(filepath, service, MAX_USLUGA);
    strcat(filepath, ".txt");

    FILE* output_file = safe_open(filepath, "w", 3);

    while (head != NULL) {
        if (strcmp(head->usluga.usluga, service) == 0) {
            Usluga u = desifruj_broj_telefona(head->usluga);
            fprintf(output_file, "%-12s %s\n", u.pruzaoc, u.telefon);
        }
        head = head->next;
    }

    fclose(output_file);
}

char desifruj_karakter(char c)
{
    // Stavi sve karaktere da budu uppercase
    if (c > 96 && c < 123) {
        c -= 32;
    }

    switch (c) {
    case 'A':
    case 'B':
    case 'C':
        return '2';
    case 'D':
    case 'E':
    case 'F':
        return '3';
    case 'G':
    case 'H':
    case 'I':
        return '4';
    case 'J':
    case 'K':
    case 'L':
        return '5';
    case 'M':
    case 'N':
    case 'O':
        return '6';
    case 'P':
    case 'Q':
    case 'R':
    case 'S':
        return '7';
    case 'T':
    case 'U':
    case 'V':
        return '8';
    case 'W':
    case 'X':
    case 'Y':
    case 'Z':
        return '9';
    default:
        return c;
    }
}

Usluga desifruj_broj_telefona(Usluga usluga)
{
    for (size_t i = 6; i < strlen(usluga.telefon); i++) {
        usluga.telefon[i] = desifruj_karakter(usluga.telefon[i]);
    }

    return usluga;
}

void list_free(Node** head)
{
    Node* tmp;
    while (*head != NULL) {
        tmp = *head;
        *head = (*head)->next;
        free(tmp);
    }
}

void list_push(Node** head, Usluga usluga)
{
    Node* new_node = malloc(sizeof(Node));
    new_node->usluga = usluga;
    new_node->next = NULL;

    if (*head == NULL) {
        *head = new_node;
        return;
    }

    Node* tmp = *head;

    while (tmp->next != NULL) {
        tmp = tmp->next;
    }

    tmp->next = new_node;
}

void ucitaj_usluge(FILE* file, Node** head)
{
    Usluga usluga;
    while (fscanf(file, "%s %s %s", usluga.pruzaoc, usluga.usluga, usluga.telefon) != EOF) {
        list_push(head, usluga);
    }
}

FILE* safe_open(const char* filepath, const char* mode, const int error_code)
{
    FILE* file = fopen(filepath, mode);
    if (file == NULL) {
        fprintf(stderr, "Greska pri otvaranju fajla \"%s\"\n", filepath);
        exit(error_code);
    }

    return file;
}
