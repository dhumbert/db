#ifndef DICT_H
#define DICT_H

typedef struct DictEntry {
    const char *key;
    const char *value;
    struct DictEntry *next;
} DictEntry;

typedef struct Dict {
    DictEntry *first;
    DictEntry *cursor;
} Dict;

Dict * dict_init(void);
void        dict_add(Dict **dict, const char *key, const char *value);
DictEntry * dict_iter(Dict **dict);
void        dict_reset(Dict **dict);
const char * dict_get(Dict *dict, const char *key);


#endif