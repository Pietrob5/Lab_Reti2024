
// This represent a record in the only schema of this database
struct Persona {
    char *name;
    char *surname;
    char *address;
    int age;
};

// This is a node of an index that hold a string
typedef struct IndexNodeString {
    char * value;
    struct IndexNodeString * left;
    struct IndexNodeString * right;
    struct Persona * user;
} IndexNodeString;

// This is a node of an index that hold an int
typedef struct IndexNodeInt {
    int value;
    struct IndexNodeInt * left;
    struct IndexNodeInt * right;
    struct Persona * user;
} IndexNodeInt;

// A database hold a set of records and a set of indexes
typedef struct {
    IndexNodeString * name;
    IndexNodeString * surname;
    IndexNodeString * address;
    IndexNodeInt * age;
} Database;

// TODO implement the following methods
// The method return a struct Persona or NULL 

void insert(Database * database, struct Persona * Persona);
struct Persona* findByName(Database * database, char * name);
struct Persona* findBySurname(Database * database, char * surname);
struct Persona* findByAddress(Database * database, char * address);
struct Persona* findByAge(Database * database, int age);
