#include <stdio.h>
#include "udt.h"

// Вспомогательная процедура (вариант 6)
void fix_inversion(udt *d) {
    if (udt_size(d) < 2) return;

    udt tmp, rest, big;
    udt_create(&tmp);
    udt_create(&rest);
    udt_create(&big);

    // первый элемент
    data_type prev = d->data[d->head];
    udt_pop_front(d);
    udt_push_back(&tmp, prev);

    int found = 0;
    while (!udt_is_empty(d) && !found) {
        data_type curr = d->data[d->head];
        udt_pop_front(d);
        if (curr.key < prev.key) {
            found = 1;
            // сохраняем оставшиеся
            while (!udt_is_empty(d)) {
                data_type x = d->data[d->head];
                udt_pop_front(d);
                udt_push_back(&rest, x);
            }
            // сдвигаем curr влево
            while (!udt_is_empty(&tmp)) {
                data_type last = tmp.data[(tmp.tail - 1 + MAX_SIZE) % MAX_SIZE];
                if (last.key > curr.key) {
                    udt_pop_back(&tmp);
                    udt_push_front(&big, last);
                } else break;
            }
            udt_push_back(&tmp, curr);
            // возвращаем большие обратно
            while (!udt_is_empty(&big)) {
                data_type x = big.data[(big.tail - 1 + MAX_SIZE) % MAX_SIZE];
                udt_pop_back(&big);
                udt_push_back(&tmp, x);
            }
            // собираем всё в d
            while (!udt_is_empty(&tmp)) {
                data_type x = tmp.data[tmp.head];
                udt_pop_front(&tmp);
                udt_push_back(d, x);
            }
            while (!udt_is_empty(&rest)) {
                data_type x = rest.data[rest.head];
                udt_pop_front(&rest);
                udt_push_back(d, x);
            }
            return;
        } else {
            udt_push_back(&tmp, curr);
            prev = curr;
        }
    }
    // инверсии нет – возвращаем всё
    while (!udt_is_empty(&tmp)) {
        data_type x = tmp.data[tmp.head];
        udt_pop_front(&tmp);
        udt_push_back(d, x);
    }
}

// проверка упорядоченности
int is_sorted(udt *d) {
    if (udt_size(d) < 2) return 1;
    udt tmp;
    udt_create(&tmp);
    data_type prev = d->data[d->head];
    udt_pop_front(d);
    udt_push_back(&tmp, prev);
    int sorted = 1;
    while (!udt_is_empty(d)) {
        data_type curr = d->data[d->head];
        udt_pop_front(d);
        if (curr.key < prev.key) sorted = 0;
        udt_push_back(&tmp, curr);
        prev = curr;
    }
    while (!udt_is_empty(&tmp)) {
        data_type x = tmp.data[tmp.head];
        udt_pop_front(&tmp);
        udt_push_back(d, x);
    }
    return sorted;
}

// сортировка многократным вызовом fix_inversion
void sort_udt(udt *d) {
    while (!is_sorted(d))
        fix_inversion(d);
}

int main() {
    udt d;
    udt_create(&d);

    // тестовые данные (ключи)
    data_type test[] = {
        {5, "five"},
        {2, "two"},
        {8, "eight"},
        {1, "one"},
        {9, "nine"},
        {3, "three"}
    };
    int n = sizeof(test)/sizeof(test[0]);
    for (int i = 0; i < n; i++)
        udt_push_back(&d, test[i]);

    printf("Исходный дек: ");
    udt_print(&d);
    sort_udt(&d);
    printf("Отсортированный дек: ");
    udt_print(&d);

    return 0;
}