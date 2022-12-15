#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAZIV 21
#define MAX_VRSTA 21

typedef struct cvecara_st Cvecara;
typedef struct node_st Node;

struct cvecara_st {
    char naziv[MAX_NAZIV];
    char vrsta[MAX_VRSTA];
    double cena;
};

struct node_st {
    Cvecara cvecara;
    Node* next;
};

FILE* safe_open(const char* filepath, const char* mode, int error_code);
void ucitaj_listu(FILE* file, Node** head);
void print_cvecara(Cvecara cvecara);
void list_print(Node* head);
void list_add(Node** head, Cvecara cvecara);
void list_free(Node** head);
void najpovoljniji(Node* head, char* query);

int main(int argc, char** argv)
{
    if (argc != 3) {
        fprintf(stderr, "Greska pri pozivu. Primer %s <cvet> <input_file>\n", argv[0]);
        exit(1);
    }

    char* query_flower = argv[1];
    char* input_file_name = argv[2];
    FILE* input_file = safe_open(input_file_name, "r", 2);

    Node* head = NULL;

    ucitaj_listu(input_file, &head);
    fclose(input_file);

    najpovoljniji(head, query_flower);
    list_free(&head);
    return 0;
}

void najpovoljniji(Node* head, char* query)
{
    Node* best = NULL;

    while (head->next != NULL) {
        if (strcmp(head->cvecara.vrsta, query) == 0) {
            if (best == NULL) {
                best = head;
            } else {
                if (head->cvecara.cena < best->cvecara.cena) {
                    best = head;
                }
            }
        }
        printf("%.2lf %s %s\n", head->cvecara.cena, head->cvecara.vrsta, head->cvecara.naziv);
        head = head->next;
    }

    if (best != NULL) {
        printf("\nCvecara sa najpovoljnijim cvetom %s je %s, njena cena je %.2lf\n", best->cvecara.vrsta, best->cvecara.naziv, best->cvecara.cena);
    } else {
        printf("\nU trenutno raspolozivim cvecarama ne postoji cvet %s!\n", query);
    }
}

void ucitaj_listu(FILE* file, Node** head)
{
    Cvecara cvecara;
    while (fscanf(file, "%s %s %lf", cvecara.naziv, cvecara.vrsta, &cvecara.cena) != EOF) {
        list_add(head, cvecara);
    }
}

void print_cvecara(Cvecara cvecara)
{
    printf("%s %s %lf\n", cvecara.naziv, cvecara.vrsta, cvecara.cena);
}

void list_print(Node* head)
{
    while (head != NULL) {
        print_cvecara(head->cvecara);
        head = head->next;
    }
}

void list_add(Node** head, Cvecara cvecara)
{
    Node* new_node = malloc(sizeof(Node));
    new_node->next = NULL;
    new_node->cvecara = cvecara;

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

void list_free(Node** head)
{
    while (*head != NULL) {
        Node* tmp = *head;
        *head = (*head)->next;
        free(tmp);
    }
}

FILE* safe_open(const char* filepath, const char* mode, int error_code)
{
    FILE* file = fopen(filepath, mode);
    if (file == NULL) {
        fprintf(stderr, "Greska pri otvaranju fajla \"%s\"\n", filepath);
        exit(error_code);
    }

    return file;
}
