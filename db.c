#include <stdlib.h>
#include "dict.h"
#include "db.h"

Db * db_init()
{
    Db *db = malloc(sizeof(Db));
    db->dict = dict_init();

    return db;
}

int db_set(Db *db, const char *key, const char *value)
{
    dict_add(&db->dict, key, value);
    return DB_OK;
}

const char *db_get(Db *db, const char *key)
{
    return dict_get(db->dict, key);
}