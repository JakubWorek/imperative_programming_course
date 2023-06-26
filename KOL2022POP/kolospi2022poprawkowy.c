#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* name;
    char* surname;
    char pesel[12];
} Person;

typedef struct {
    Person** people;
    int size;
} PersonList;

void init_pl(PersonList *p_pl) {
    p_pl->people = NULL;
    p_pl->size = 0;
}

void free_person(Person *person) {
    free(person->name);
    free(person->surname);
    free(person);
}

void free_pl(PersonList *p_pl) {
    for (int i = 0; i < p_pl->size; i++) {
        free_person(p_pl->people[i]);
    }
    free(p_pl->people);
    p_pl->people = NULL;
    p_pl->size = 0;
}

void fill_person_data(Person *p_p, const char *pesel, const char *name, const char *surname) {
    strncpy(p_p->pesel, pesel, sizeof(p_p->pesel) - 1);
    p_p->pesel[sizeof(p_p->pesel) - 1] = '\0';

    p_p->name = strdup(name);
    p_p->surname = strdup(surname);
}

Person *find(const PersonList *p_pl, const char *pesel) {
    for (int i = 0; i < p_pl->size; i++) {
        if (strcmp(p_pl->people[i]->pesel, pesel) == 0) {
            return p_pl->people[i];
        }
    }
    return NULL;
}

int add(PersonList *p_pl, const char *pesel, const char *name, const char *surname) {
    Person *existing_person = find(p_pl, pesel);
    if (existing_person != NULL) {
        fill_person_data(existing_person, pesel, name, surname);
        return 1;
    }

    Person *new_person = (Person*)malloc(sizeof(Person));
    if (new_person == NULL) {
        return 0; // Failed to allocate memory
    }

    fill_person_data(new_person, pesel, name, surname);

    Person **new_people = (Person**)realloc(p_pl->people, (p_pl->size + 1) * sizeof(Person*));
    if (new_people == NULL) {
        free_person(new_person);
        return 0; // Failed to allocate memory
    }

    p_pl->people = new_people;
    p_pl->people[p_pl->size] = new_person;
    p_pl->size++;

    return 1;
}

void merge(PersonList *p_target, const PersonList *p_src) {
    for (int i = 0; i < p_src->size; i++) {
        Person *src_person = p_src->people[i];
        Person *target_person = find(p_target, src_person->pesel);

        if (target_person != NULL) {
            fill_person_data(target_person, src_person->pesel, src_person->name, src_person->surname);
        } else {
            add(p_target, src_person->pesel, src_person->name, src_person->surname);
        }
    }
}

int main() {
    printf("Hello, World!\n");
    return 0;
}
