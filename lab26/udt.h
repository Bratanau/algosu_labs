#ifndef UDT_H
#define UDT_H

#include <stdbool.h>
#include <stddef.h>

#define MAX_SIZE 100
#define STR_LEN 256

typedef int key_type;
typedef char value_type[STR_LEN];

typedef struct {
    key_type key;
    value_type value;
} data_type;

typedef struct {
    data_type data[MAX_SIZE];
    int head;
    int tail;
    int count;
} udt;

void udt_create(udt *);
bool udt_is_empty(const udt *);
void udt_push_front(udt *, data_type);
void udt_push_back(udt *, data_type);
void udt_pop_front(udt *);
void udt_pop_back(udt *);
void udt_print(const udt *);
size_t udt_size(const udt *);
void udt_insert(udt *, data_type);

#endif