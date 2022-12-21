#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VRSTA 21
#define MAX_GRAD 3

typedef struct meso_st Meso;

struct meso_st {
    char grad[MAX_GRAD];
    char vrsta[MAX_VRSTA];
    double cena;
    Meso* next;
};

FILE* safe_open(const char* filepath, const char* mode, const int error_code);
void* safe_malloc(const size_t size, const int error_code);
Meso* meso_new();
void meso_populate(Meso* new, char* grad, char* vrsta, double cena);
void meso_push_list(Meso** head, Meso* new);
void meso_read_list(FILE* in, Meso** head);
Meso* meso_najpovoljnije(Meso* head, const char* type);
void meso_save_list(FILE* out, Meso* head);
void meso_save_item(FILE* out, Meso* head);
void meso_free_list(Meso** head);

int main(int argc, char** argv)
{
    if (argc != 4) {
        fprintf(stderr, "Error: %s <vrsta_mesa> <input_file> <output_file>", argv[0]);
        exit(1);
    }

    Meso* head = NULL;
    char* meso_type = argv[1];
    char* in_fp = argv[2];
    char* out_fp = argv[3];

    FILE* in = safe_open(in_fp, "r", 2);
    meso_read_list(in, &head);
    fclose(in);

    FILE* out = safe_open(out_fp, "w", 4);
    meso_save_list(out, head);

    Meso* best = meso_najpovoljnije(head, meso_type);

    if (best == NULL) {
        fprintf(out, "\nNigde nema '%s'\n", meso_type);
    } else {
        fprintf(out, "\nNajpovoljnije '%s' je:\n", meso_type);
        meso_save_item(out, best);
    }

    fclose(out);
    meso_free_list(&head);

    return 0;
}

void* safe_malloc(const size_t size, const int error_code)
{
    void* buffer = malloc(size);
    if (buffer == 0) {
        fprintf(stderr, "Greska pri alokaciji memorije\n");
        exit(error_code);
    }

    return buffer;
}

Meso* meso_new()
{
    return safe_malloc(sizeof(Meso), 3);
}

void meso_populate(Meso* new, char* grad, char* vrsta, double cena)
{
    strncpy(new->vrsta, vrsta, MAX_VRSTA);
    strncpy(new->grad, grad, MAX_GRAD);
    new->cena = cena;
    new->next = NULL;
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

void meso_read_list(FILE* in, Meso** head)
{
    char grad[MAX_GRAD] = { 0 };
    char vrsta[MAX_VRSTA] = { 0 };
    double cena = 0;

    while (fscanf(in, "%s %s %lf", grad, vrsta, &cena) != EOF) {
        Meso* new = meso_new();
        meso_populate(new, grad, vrsta, cena);
        meso_push_list(head, new);
    }
}

void meso_push_list(Meso** head, Meso* new)
{
    new->next = NULL;
    if (*head == NULL) {
        *head = new;
        return;
    }

    Meso* tmp = *head;
    while (tmp->next != NULL) {
        tmp = tmp->next;
    }

    tmp->next = new;
}

Meso* meso_najpovoljnije(Meso* head, const char* type)
{
    Meso* best = NULL;
    while (head != NULL) {
        if (strcmp(head->vrsta, type) == 0) {
            if (best == NULL) {
                best = head;
            } else {
                if (head->cena < best->cena) {
                    best = head;
                }
            }
        }
        head = head->next;
    }

    return best;
}

void meso_save_list(FILE* out, Meso* head)
{
    while (head != NULL) {
        meso_save_item(out, head);
        head = head->next;
    }
}

void meso_save_item(FILE* out, Meso* head)
{
    fprintf(out, "%-6.2lf %-2s %s\n", head->cena, head->grad, head->vrsta);
}

void meso_free_list(Meso** head)
{
    Meso* tmp;
    while (*head != NULL) {
        tmp = *head;
        *head = (*head)->next;
        free(tmp);
    }
}
