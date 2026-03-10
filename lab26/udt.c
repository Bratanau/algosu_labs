#include "udt.h"
#include <stdio.h>
#include <string.h>

void udt_create(udt *d) {
    d->count = 0;
}

bool udt_is_empty(const udt *d) {
    return d->count == 0;
}

void udt_push_front(udt *d, data_type val) { //добавить в начало
    if (d->count == MAX_SIZE) return;
    for (int i = d->count; i > 0; i--)
        d->data[i] = d->data[i - 1];
    d->data[0] = val;
    d->count++;
}

void udt_push_back(udt *d, data_type val) { //добавить в конец
    if (d->count == MAX_SIZE) return;
    d->data[d->count] = val;
    d->count++;
}

void udt_pop_front(udt *d) { //удалить из начала
    if (d->count == 0) return;
    for (int i = 0; i < d->count - 1; i++)
        d->data[i] = d->data[i + 1];
    d->count--;
}

void udt_pop_back(udt *d) { //удалить из конца
    if (d->count == 0) return;
    d->count--;
}

void udt_print(const udt *d) { //печать дека
    printf("[");
    for (int i = 0; i < d->count; i++) {
        printf("(%d,%s)", d->data[i].key, d->data[i].value);
        if (i < d->count - 1) printf(", ");
    }
    printf("]\n");
}

size_t udt_size(const udt *d) {
    return d->count;
}

void udt_insert(udt *d, data_type val) {
    udt_push_back(d, val);
}