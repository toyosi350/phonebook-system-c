#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "phonebook.h"



static int equalsIgnoreCase(const char *a, const char *b)
{
    while (*a && *b) {
        char ca = (char)tolower((unsigned char)*a);
        char cb = (char)tolower((unsigned char)*b);
        if (ca != cb) return 0;
        a++; b++;
    }
    return *a == '\0' && *b == '\0';
}

static int containsIgnoreCase(const char *text, const char *pattern)
{
    if (*pattern == '\0') return 1; /* empty pattern matches */

    for (; *text; text++) {
        const char *t = text;
        const char *p = pattern;

        while (*t && *p &&
               tolower((unsigned char)*t) == tolower((unsigned char)*p)) {
            t++;
            p++;
        }

        if (*p == '\0') return 1; /* full pattern matched */
    }
    return 0;
}


int findContactIndex(Contact *contacts, int count, char *name)
{
    for (int i = 0; i < count; i++) {
        if (equalsIgnoreCase(contacts[i].name, name)) {
            return i;
        }
    }
    return -1;
}

void listContacts(Contact *contacts, int count)
{
    if (count == 0) {
        printf("There are no contacts.\n");
        return;
    }

    printf("\n--- Contacts (%d) ---\n", count);
    for (int i = 0; i < count; i++) {
        printf("%d. %s - %s\n", i + 1, contacts[i].name, contacts[i].phone_number);
    }
}




void addContact(Contact **contacts, int *count, int *capacity)
{
    char name[NAME_LEN];
    char phone[PHONE_LEN];

    printf("Enter name: ");
    if (fgets(name, sizeof(name), stdin) == NULL) {
        printf("Input error.\n");
        return;
    }
    name[strcspn(name, "\n")] = '\0';

    if (name[0] == '\0') {
        printf("Name cannot be empty.\n");
        return;
    }

    /* prevent commas (CSV) */
    if (strchr(name, ',') != NULL) {
        printf("Name cannot contain commas.\n");
        return;
    }

    /* ✅ Duplicate prevention (case-insensitive) */
    if (findContactIndex(*contacts, *count, name) != -1) {
        printf("A contact with that name already exists.\n");
        return;
    }

    printf("Enter phone number: ");
    if (fgets(phone, sizeof(phone), stdin) == NULL) {
        printf("Input error.\n");
        return;
    }
    phone[strcspn(phone, "\n")] = '\0';

    if (phone[0] == '\0') {
        printf("Phone number cannot be empty.\n");
        return;
    }

    if (strchr(phone, ',') != NULL) {
        printf("Phone number cannot contain commas.\n");
        return;
    }

    /* allocate if first time */
    if (*capacity == 0) {
        *capacity = 2;
        *contacts = (Contact *)malloc(sizeof(Contact) * (*capacity));
        if (*contacts == NULL) {
            printf("Memory allocation failed.\n");
            *capacity = 0;
            return;
        }
    }

    /* expand if full */
    if (*count == *capacity) {
        *capacity *= 2;
        Contact *temp = (Contact *)realloc(*contacts, sizeof(Contact) * (*capacity));
        if (temp == NULL) {
            printf("Memory expansion failed.\n");
            return;
        }
        *contacts = temp;
    }

    /* safe copy */
    snprintf((*contacts)[*count].name, NAME_LEN, "%s", name);
    snprintf((*contacts)[*count].phone_number, PHONE_LEN, "%s", phone);

    (*count)++;
    printf("Contact added successfully.\n");
}




void deleteContact(Contact *contacts, int *count)
{
    if (*count == 0) {
        printf("Phonebook is empty.\n");
        return;
    }

    char name[NAME_LEN];

    printf("Enter name to delete: ");
    if (fgets(name, sizeof(name), stdin) == NULL) {
        printf("Input error.\n");
        return;
    }
    name[strcspn(name, "\n")] = '\0';

    if (name[0] == '\0') {
        printf("Name cannot be empty.\n");
        return;
    }

    /* ✅ Case-insensitive delete through ONE path */
    int index = findContactIndex(contacts, *count, name);
    if (index == -1) {
        printf("Contact not found.\n");
        return;
    }

    for (int i = index; i < *count - 1; i++) {
        contacts[i] = contacts[i + 1];
    }

    (*count)--;
    printf("Contact deleted successfully.\n");
}



void editContact(Contact *contacts, int count)
{
    if (count == 0) {
        printf("Phonebook is empty.\n");
        return;
    }

    char searchName[NAME_LEN];

    printf("Enter name to edit: ");
    if (fgets(searchName, sizeof(searchName), stdin) == NULL) {
        printf("Input error.\n");
        return;
    }
    searchName[strcspn(searchName, "\n")] = '\0';

    if (searchName[0] == '\0') {
        printf("Name cannot be empty.\n");
        return;
    }

    int index = findContactIndex(contacts, count, searchName);
    if (index == -1) {
        printf("Contact not found.\n");
        return;
    }

    printf("\nEditing: %s - %s\n", contacts[index].name, contacts[index].phone_number);
    printf("1. Name\n");
    printf("2. Phone\n");
    printf("3. Both\n");
    printf("Choose: ");

    char choiceBuf[16];
    if (fgets(choiceBuf, sizeof(choiceBuf), stdin) == NULL) {
        printf("Input error.\n");
        return;
    }
    int choice = atoi(choiceBuf);

    if (choice == 1 || choice == 3) {
        char newName[NAME_LEN];

        printf("Enter new name: ");
        if (fgets(newName, sizeof(newName), stdin) == NULL) {
            printf("Input error.\n");
            return;
        }
        newName[strcspn(newName, "\n")] = '\0';

        if (newName[0] == '\0') {
            printf("New name cannot be empty.\n");
            return;
        }
        if (strchr(newName, ',') != NULL) {
            printf("Name cannot contain commas.\n");
            return;
        }

        /* prevent duplicates (case-insensitive), allow same record */
        int existing = findContactIndex(contacts, count, newName);
        if (existing != -1 && existing != index) {
            printf("Another contact with that name already exists.\n");
            return;
        }

        snprintf(contacts[index].name, NAME_LEN, "%s", newName);
    }

    if (choice == 2 || choice == 3) {
        char newPhone[PHONE_LEN];

        printf("Enter new phone number: ");
        if (fgets(newPhone, sizeof(newPhone), stdin) == NULL) {
            printf("Input error.\n");
            return;
        }
        newPhone[strcspn(newPhone, "\n")] = '\0';

        if (newPhone[0] == '\0') {
            printf("New phone number cannot be empty.\n");
            return;
        }
        if (strchr(newPhone, ',') != NULL) {
            printf("Phone number cannot contain commas.\n");
            return;
        }

        snprintf(contacts[index].phone_number, PHONE_LEN, "%s", newPhone);
    }

    printf("Contact updated successfully.\n");
}

void searchContacts(Contact *contacts, int count)
{
    if (count == 0) {
        printf("Phonebook is empty.\n");
        return;
    }

    char query[NAME_LEN];

    printf("Enter search text: ");
    if (fgets(query, sizeof(query), stdin) == NULL) {
        printf("Input error.\n");
        return;
    }
    query[strcspn(query, "\n")] = '\0';

    if (query[0] == '\0') {
        printf("Search query cannot be empty.\n");
        return;
    }

    int found = 0;
    printf("\n--- Search Results ---\n");

    for (int i = 0; i < count; i++) {
        if (containsIgnoreCase(contacts[i].name, query)) {
            printf("%d. %s - %s\n", i + 1, contacts[i].name, contacts[i].phone_number);
            found = 1;
        }
    }

    if (!found) {
        printf("No matching contacts found.\n");
    }
}
