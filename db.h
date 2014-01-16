#ifndef DB_H
#define DB_H

#include "dict.h"

#define DB_OK 1
#define DB_ERR 2

typedef struct Db {
    Dict *dict;
} Db;

Db *       db_init();
int         db_set(Db *db, const char *key, const char *value);
const char *db_get(Db *db, const char *key);

#endif