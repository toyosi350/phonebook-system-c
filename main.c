#include <stdio.h>
#include <stdlib.h>
#include "phonebook.h"

int main(void)
{
    Contact *contacts = NULL;
    int count = 0;
    int capacity = 0;
    int choice;

    loadContacts(&contacts, &count, &capacity);

    printf("WELCOME TO TOYOSI'S PHONEBOOK LIBRARY\n");

    while (1) {
        printf("\nMenu:\n");
        printf("1. Add contact\n");
        printf("2. List contacts\n");
        printf("3. Delete contact\n");
        printf("4. Edit contact\n");
        printf("5. Search contacts\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            getchar();
            continue;
        }
        getchar(); /* consume newline */

        switch (choice) {
            case 1:
                addContact(&contacts, &count, &capacity);
                saveContacts(contacts, count);
                break;

            case 2:
                listContacts(contacts, count);
                break;

            case 3:
                deleteContact(contacts, &count);
                saveContacts(contacts, count);
                break;

            case 4:
                editContact(contacts, count);
                saveContacts(contacts, count);
                break;

            case 5:
                searchContacts(contacts, count);
                break;

            case 6:
                saveContacts(contacts, count);
                free(contacts);
                printf("Goodbye!\n");
                return 0;

            default:
                printf("Wrong choice.\n");
        }
    }
}
