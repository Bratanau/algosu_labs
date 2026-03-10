#include "udt.h"
#include <stdio.h>
#include <string.h>

void udt_create(udt *d) {
    d->head = 0;
    d->tail = 0;
    d->count = 0;
}

bool udt_is_empty(const udt *d) {
    return d->count == 0;
}

void udt_push_front(udt *d, data_type val) {
    if (d->count == MAX_SIZE) return;
    d->head = (d->head - 1 + MAX_SIZE) % MAX_SIZE;
    d->data[d->head] = val;
    d->count++;
}

void udt_push_back(udt *d, data_type val) {
    if (d->count == MAX_SIZE) return;
    d->data[d->tail] = val;
    d->tail = (d->tail + 1) % MAX_SIZE;
    d->count++;
}

void udt_pop_front(udt *d) {
    if (d->count == 0) return;
    d->head = (d->head + 1) % MAX_SIZE;
    d->count--;
}

void udt_pop_back(udt *d) {
    if (d->count == 0) return;
    d->tail = (d->tail - 1 + MAX_SIZE) % MAX_SIZE;
    d->count--;
}

void udt_print(const udt *d) {
    printf("[");
    for (int i = 0; i < d->count; i++) {
        int idx = (d->head + i) % MAX_SIZE;
        printf("(%d,%s)", d->data[idx].key, d->data[idx].value);
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