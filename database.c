#include "database.h"   //I made some changhes in the .h file
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
Database *newDatabase() {
    Database *d=malloc(sizeof(Database));
    IndexNodeString *n=malloc(sizeof(IndexNodeString));
    IndexNodeString *s=malloc(sizeof(IndexNodeString));
    IndexNodeString *a=malloc(sizeof(IndexNodeString));
    IndexNodeInt *y=malloc(sizeof(IndexNodeInt));
    d->name=n;
    d->surname=s;
    d->address=a;
    d->age=y;
    return d;
}

struct Persona *newPersona(char* n, char* s, char* a, int age){
    struct Persona *p=malloc(sizeof(struct Persona)); //non so se i nomi vanno messi con malloc
    p->name=malloc(sizeof(n));
    strcpy(p->name,n);
    p->surname=malloc(sizeof(s));
    strcpy(p->surname,s);
    p->address=malloc(sizeof(a));
    strcpy(p->address,a);
    p->age=age;
}

void insertByName(IndexNodeString *database, struct Persona *persona) {
    
    if (database->value == NULL) {
        database->value=persona->name;
        database->user=persona;
        return;
    }
    if (strcmp(persona->name, database->value)<0) { //persona's name is lower than value
        if (database->left == NULL) {
            IndexNodeString *t=malloc(sizeof(IndexNodeString));
            t->value=persona->name;
            t->left=NULL;
            t->right=NULL;
            t->user=persona;
            database->left=t;
            return;
        }
        else {
            insertByName(database->left, persona);
            return;
        }
    }
    if (database->right == NULL){
        IndexNodeString *t=malloc(sizeof(IndexNodeString));
        t->value=persona->name;
        t->left=NULL;
        t->right=NULL;
        t->user=persona;
        database->right=t;
        return;
    }
    insertByName(database->right, persona);
}

void insertBySurname(IndexNodeString *database, struct Persona *persona) {
    if (database == NULL) {
        printf("PROBLEMI PROBLEMI PROBLEMI\n");
        return;
    }
    if (database->value == NULL) {
        database->value=persona->surname;
        database->user=persona;
        return;
    }
    if (strcmp(persona->surname, database->value)<0) { //persona's surname is lower than value
        if (database->left == NULL) { 
            IndexNodeString *t=malloc(sizeof(IndexNodeString));
            t->value=persona->surname;
            t->left=NULL;
            t->right=NULL;
            t->user=persona;
            database->left=t;
            return;
        }
        else {
            insertBySurname(database->left, persona);
            return;
        }
    }
    if (database->right == NULL){
        IndexNodeString *t=malloc(sizeof(IndexNodeString));
        t->value=persona->surname;
        t->left=NULL;
        t->right=NULL;
        t->user=persona;
        database->right=t;
        return;
    }
    insertBySurname(database->right, persona);
}

void insertByAddress(IndexNodeString *database, struct Persona *persona) {
    if (database == NULL) {
        printf("PROBLEMI PROBLEMI PROBLEMI\n");
        return;
    }
    if (database->value == NULL) {
        database->value=persona->address;
        database->user=persona;
        return;
    }
    if (strcmp(persona->address, database->value)<0) { //persona's address is lower than value
        if (database->left == NULL) { 
            IndexNodeString *t=malloc(sizeof(IndexNodeString));
            t->value=persona->address;
            t->left=NULL;
            t->right=NULL;
            t->user=persona;
            database->left=t;
            return;
        }
        else {
            insertByAddress(database->left, persona);
            return;
        }
    }
    if (database->right == NULL){
        IndexNodeString *t=malloc(sizeof(IndexNodeString));
        t->value=persona->address;
        t->left=NULL;
        t->right=NULL;
        t->user=persona;
        database->right=t;
        return;
    }
    insertByAddress(database->right, persona);
}

void insertByAge(IndexNodeInt* database, struct Persona* persona) {
    if (database == NULL) {
        printf("PROBLEMI PROBLEMI PROBLEMI\n");
        return;
    }
    if (database->value == 0) { //not sure if value is initialized =0
        database->value=persona->age;
        database->user=persona;
        return;
    }
    if (persona->age < database->value) { //persona's age is lower than value
        if (database->left == NULL) { 
            IndexNodeInt *t=malloc(sizeof(IndexNodeInt));
            t->value=persona->age;
            t->left=NULL;
            t->right=NULL;
            t->user=persona;
            database->left=t;
            return;
        }
        else {
            insertByAge(database->left, persona);
            return;
        }
    }
    if (database->right == NULL){
        IndexNodeInt *t=malloc(sizeof(IndexNodeInt));
        t->value=persona->age;
        t->left=NULL;
        t->right=NULL;
        t->user=persona;
        database->right=t;
        return;
    }
    insertByAge(database->right, persona);
}

void insert(Database *database, struct Persona *persona) {
    if (persona == NULL)
        return;
    insertByName(database->name, persona);
    insertBySurname(database->surname, persona);
    insertByAddress(database->address, persona);
    insertByAge(database->age, persona);
}

void print(IndexNodeString* database) {
    if (database == NULL)
        return;
    print(database->left);
    printf("%s\n", database->value);
    print(database->right);
}

void printAge(IndexNodeInt* database) {
    if (database == NULL)
        return;
    printAge(database->left);
    printf("%d\n", database->value);
    printAge(database->right);
}

struct Persona * search(IndexNodeString* database, char *key) {
    if (database == NULL)
        return NULL;
    if (strcmp(key, database->value)==0){
        return database->user;
    }
    struct Persona *l=search(database->left, key);
    struct Persona *r=search(database->right, key);
    if (l != NULL)
        return l;
    if (r != NULL)
        return r;
    return NULL;
}

struct Persona* findByName(Database * database, char * name) {
    if (name == NULL)
        return NULL;
    return search(database->name, name);
}

struct Persona* findBySurname(Database * database, char * surname) {
    if (surname == NULL)
        return NULL;
    return search(database->surname, surname);
}

struct Persona* findByAddress(Database * database, char * address) {
    if (address == NULL)
        return NULL;
    return search(database->address, address);
}

struct Persona * searchAge(IndexNodeInt* database, int key) {
    if (database == NULL)
        return NULL;
    if (key == database->value){
        return database->user;
    }
    struct Persona *l=searchAge(database->left, key);
    struct Persona *r=searchAge(database->right, key);
    if (l != NULL)
        return l;
    if (r != NULL)
        return r;
    return NULL;
}

struct Persona* findByAge(Database * database, int age) {
    return searchAge(database->age, age);
}

int main() {
    Database *d=newDatabase();
    struct Persona *pietro=newPersona("pietro", "bartolocci", "gabelletta", 21);
    struct Persona *gatto=newPersona("zatto", "puma", "casa mia", 6);
    struct Persona *rrr=newPersona("rrr", "franco", "cuccia", 78);
    struct Persona *giulia=newPersona("giulia", "rossi", "marmore", 18);
    struct Persona *mamma=newPersona("aaa", "mamma", "terni", 200);
    insert(d, pietro);
    insert(d, rrr);
    insert(d, gatto);
    insert(d, giulia);
    insert(d, mamma);
    print(d->name);
    printAge(d->age);
    struct Persona *trovato=findByAge(d, 200);
    if (trovato != NULL)
        printf("%s\n", trovato->surname);
    else
        printf("NON trovato\n");
    return 0;
}