#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "phonebook.h"

#define DATA_FILE "C:\\Users\\Admin\\Documents\\contacts.csv"

void saveContacts(Contact *contacts, int count)
{
    FILE *fp = fopen(DATA_FILE, "w");
    if (!fp) return;

    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s,%s\n", contacts[i].name, contacts[i].phone_number);
    }

    fclose(fp);
}

void loadContacts(Contact **contacts, int *count, int *capacity)
{
    FILE *fp = fopen(DATA_FILE, "r");
    if (!fp) {
        *contacts = NULL;
        *count = 0;
        *capacity = 0;
        return;
    }

    char line[256];
    *count = 0;
    *capacity = 10;  // better default
    *contacts = malloc(sizeof(Contact) * (*capacity));
    if (!*contacts) {
        fclose(fp);
        *count = 0;
        *capacity = 0;
        return;
    }

    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = '\0';

        char *comma = strchr(line, ',');
        if (!comma) continue;

        *comma = '\0';
        char *name = line;
        char *phone = comma + 1;

        if (*count == *capacity) {
            int newCap = (*capacity) * 2;
            Contact *tmp = realloc(*contacts, sizeof(Contact) * newCap);
            if (!tmp) break; // stop safely if out of memory
            *contacts = tmp;
            *capacity = newCap;
        }

        snprintf((*contacts)[*count].name, NAME_LEN, "%s", name);
        snprintf((*contacts)[*count].phone_number, PHONE_LEN, "%s", phone);
        (*count)++;
    }

    fclose(fp);
}
