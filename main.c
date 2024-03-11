#include <stdio.h>
#include "database.h"

//Some test functions
int main() {
    Database *d=newDatabase();
    struct Persona *pietro=newPersona("pietro", "bartolocci", "gabelletta", 21);
    struct Persona *gatto=newPersona("gatto", "puma", "casa mia", 6);
    struct Persona *mario=newPersona("mario", "bianchi", "roma", 78);
    struct Persona *carlo=newPersona("carlo", "rossi", "marmore", 31);
    struct Persona *giulia=newPersona("giulia", "verdi", "terni", 18);
    insert(d, pietro);
    insert(d, mario);
    insert(d, gatto);
    insert(d, carlo);
    insert(d, giulia);
    print(d->name);
    printAge(d->age);
    struct Persona *trovato=findByAddress(d, "marmore");
    if (trovato != NULL)
        printf("Found: %s\n", trovato->surname);
    else
        printf("NOT found\n");
    return 0;
}