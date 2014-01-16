#include <stdlib.h>
#include <string.h>
#include "dict.h"

Dict * dict_init()
{
    Dict *d = malloc(sizeof(Dict));
    d->first = NULL;
    d->cursor = NULL;
    return d;
}

void dict_add(Dict **dict, const char *key, const char *value)
{
    DictEntry *de = malloc(sizeof(DictEntry));
    de->key = key;
    de->value = value;

    de->next = (*dict)->first; // add new entry to head of linked list (faster)
    (*dict)->first = de;
    (*dict)->cursor = de;
}

DictEntry * dict_iter(Dict **dict)
{
    DictEntry *tmp = (*dict)->cursor;
    
    if (tmp == NULL ) {
        return NULL;
    }
    
    (*dict)->cursor = tmp->next;

    return tmp;
}

void dict_reset(Dict **dict)
{
    (*dict)->cursor = (*dict)->first;
}

const char * dict_get(Dict *dict, const char *key)
{
    dict_reset(&dict);
    DictEntry *cursor = dict_iter(&dict);

    // for now this implementation iterates over the dict.
    // should implement hash table
    while (cursor != NULL) {
        if (strcmp(cursor->key, key) == 0) {
            return cursor->value;
        }

        cursor = dict_iter(&dict);
    }

    dict_reset(&dict);
    return NULL;
}