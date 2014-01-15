#include <stdlib.h>
#include <stdio.h>

typedef struct DictEntry {
    const char *key;
    const char *value;
    struct DictEntry *next;
} DictEntry;

typedef struct Dict {
    DictEntry *first;
    DictEntry *cursor;
} Dict;

typedef Dict *p_Dict;
typedef DictEntry *p_DictEntry;

Dict *dict_init()
{
    Dict *d = malloc(sizeof(Dict));
    d->first = d->cursor = NULL;
    return d;
}

void dict_add(p_Dict *dict, const char *key, const char *value)
{
    DictEntry *de = malloc(sizeof(DictEntry));
    de->key = key;
    de->value = value;

    de->next = (*dict)->first; // add new dict entry to head of linked list (faster)
    (*dict)->first = de;
    (*dict)->cursor = de;
}

p_DictEntry dict_iter(p_Dict *dict)
{
    p_DictEntry tmp = (*dict)->cursor;
    
    if (tmp == NULL ) {
        return NULL;
    }
    
    (*dict)->cursor = tmp->next;

    return tmp;
}

int main()
{
    Dict *d = dict_init();
    dict_add(&d, "key1", "value1");
    dict_add(&d, "key2", "value2");

    DictEntry *cursor = dict_iter(&d);

    while (cursor != NULL) {
        printf("%s\n", cursor->key);
        cursor = dict_iter(&d);
    }
}