#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define DICT_OK    0
#define DICT_EXIST 1
#define DICT_NOT_EXIST 2

typedef struct dictEntry {
    int key;
    int value;
    struct dictEntry *next;
} dictEntry;

typedef struct dict {
    dictEntry **table;
    int cap;
    int cnt;
} dict;

dict *dictCreate(int cap) {
    dict *d = (dict*)malloc(sizeof(dict));
    if (NULL == d) return NULL;

    d->table = (dictEntry**)malloc(sizeof(dictEntry*) * cap);
    if (NULL == d->table) {
        free(d);
        return NULL;
    }
    memset(d->table, 0, sizeof(dictEntry*) * cap);
    d->cap = cap;
    d->cnt = 0;

    return d;
}

// true for new, false for update value
int dictAdd(dict *d, int key, int value) {
    int index = key % d->cap;
    dictEntry *iter = d->table[index];
    while (iter) {
        if (iter->key == key) {
            iter->value = value;
            return DICT_EXIST;
        }
        iter = iter->next;
    }

    dictEntry *entry = malloc(sizeof(dictEntry));
    entry->key = key;
    entry->value = value;
    entry->next = NULL;

    entry->next = d->table[index];
    d->table[index] = entry;
    d->cnt++;

    return DICT_OK;
}

int dictDel(dict *d, int key) {
    int index = key % d->cap;
    dictEntry *entry = d->table[index];
    if (NULL == entry) return DICT_NOT_EXIST;
    if (entry->key == key) {
        d->table[index] = entry->next;
        free(entry);
        d->cnt--;
        return DICT_OK;
    }

    dictEntry *next = entry->next;
    while (next) {
        if (next->key != key) {
            entry = next;
            next = next->next;
            continue;
        } else {
            break;
        }
    }
    if (NULL != next) {
        entry->next = next->next;
        free(next);
        d->cnt--;
        return DICT_OK;
    }

    return DICT_NOT_EXIST;
}

int dictGet(dict *d, int key, int *value) {
    int index = key % d->cap;
    dictEntry *entry = d->table[index];
    while (entry) {
        if (entry->key == key) {
            *value = entry->value;
            return DICT_OK;
        }
        entry = entry->next;
    }
    return DICT_NOT_EXIST;
}

int main() {
    dict *d = dictCreate(7);
    int b = 0;

    b = dictAdd(d, 3, 3);
    printf("add 3, %d\n", b);
    b = dictAdd(d, 5, 5);
    printf("add 5, %d\n", b);
    b = dictAdd(d, 12, 12);
    printf("add 12, %d\n", b);

    b = dictDel(d, 5);
    printf("del 5, %d\n", b);

    b = dictAdd(d, 12, 120);
    printf("add 12, %d\n", b);

    int v = 0;
    b = dictGet(d, 3, &v);
    printf("get 3=%d, %d\n", v, b);
    b = dictGet(d, 5, &v);
    printf("get 5=%d, %d\n", v, b);
    b = dictGet(d, 12, &v);
    printf("get 12=%d, %d\n", v, b);

    return 0;
}
