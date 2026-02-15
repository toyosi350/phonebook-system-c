#ifndef PHONEBOOK_H
#define PHONEBOOK_H

#include <stdio.h>

#define NAME_LEN 50
#define PHONE_LEN 20

typedef struct {
    char name[NAME_LEN];
    char phone_number[PHONE_LEN];
}Contact;



void addContact(Contact **contacts, int *count, int *capacity);
void listContacts(Contact *contacts, int count);
void deleteContact(Contact *contacts, int *count);
void editContact(Contact *contacts, int count);


int findContactIndex(Contact *contacts, int count, char *name);
void searchContacts(Contact *contacts, int count);

void saveContacts(Contact *contacts, int count);
void loadContacts(Contact **contacts, int *count, int *capacity);

#endif
