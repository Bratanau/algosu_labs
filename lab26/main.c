#include <stdio.h>
#include "udt.h"

void sort_udt(udt *d) {
    int n = (int)udt_size(d);
    if (n < 2) return;

    data_type a[MAX_SIZE];

    for (int i = 0; i < n; i++) {
        int idx = (d->head + i) % MAX_SIZE;
        a[i] = d->data[idx];
    }

    for (int i = 1; i < n; i++) {
        data_type key_elem = a[i];
        int j = i - 1;
        while (j >= 0 && a[j].key > key_elem.key) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = key_elem;
    }

    udt_create(d);
    for (int i = 0; i < n; i++) {
        udt_push_back(d, a[i]);
    }
}

int main(void) {
    udt d;
    udt_create(&d);

    data_type test[] = {
        {5, "five"},
        {2, "two"},
        {8, "eight"},
        {1, "one"},
        {9, "nine"},
        {3, "three"}
    };

    int n = (int)(sizeof(test) / sizeof(test[0]));
    for (int i = 0; i < n; i++)
        udt_push_back(&d, test[i]);

    printf("Исходный дек: ");
    udt_print(&d);
    sort_udt(&d);
    printf("Отсортированный дек: ");
    udt_print(&d);

    return 0;
}