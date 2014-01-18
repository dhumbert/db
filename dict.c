#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "dict.h"

// locks: http://www.cognitus.net/html/howto/pthreadSemiFAQ_10.html

Dict * dict_init()
{
    Dict *d = malloc(sizeof(Dict));
    d->first = NULL;
    d->cursor = NULL;
    pthread_rwlock_init(&d->lock, NULL);
    return d;
}

void dict_add(Dict **dict, const char *key, const char *value)
{
    int locked = pthread_rwlock_wrlock(&(*dict)->lock);
    if (locked != 0) {
        perror("ERROR: Unable to obtain write lock");
        exit(1);
    }

    DictEntry *de = malloc(sizeof(DictEntry));
    de->key = key;
    de->value = value;

    de->next = (*dict)->first; // add to head of linked list (faster)
    (*dict)->first = de;
    (*dict)->cursor = de;

    int unlocked = pthread_rwlock_unlock(&(*dict)->lock);
    if (unlocked != 0) {
        perror("ERROR: Unable to release write lock");
        exit(1);
    }
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
    int locked = pthread_rwlock_rdlock(&dict->lock);
    if (locked != 0) {
        perror("ERROR: Unable to obtain read lock");
        exit(1);
    }

    dict_reset(&dict);
    DictEntry *cursor = dict_iter(&dict);

    const char *result = NULL;

    // for now this implementation iterates over the dict.
    // TODO: implement hash table
    while (cursor != NULL) {
        if (strcmp(cursor->key, key) == 0) {
            result = cursor->value;
            break;
        }

        cursor = dict_iter(&dict);
    }

    dict_reset(&dict);

    int unlocked = pthread_rwlock_unlock(&dict->lock);
    if (unlocked != 0) {
        perror("ERROR: Unable to release read lock");
        exit(1);
    }

    return result;
}