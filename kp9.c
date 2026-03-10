#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define KEY_LEN 6
#define DATA_LEN 256
#define MIN_SIZE 14

typedef struct {
    char key[KEY_LEN + 1];   // +1 для завершающего нуля
    char data[DATA_LEN];
} Record;

// Печать таблицы
void print_table(Record *t, int n, const char *title) {
    printf("\n%s\n", title);
    printf("%-10s %s\n", "Ключ", "Данные");
    for (int i = 0; i < n; i++) {
        printf("%-10s %s\n", t[i].key, t[i].data);
    }
}

// Сравнение ключей (для qsort и двоичного поиска)
int cmp_key(const void *a, const void *b) {
    return strcmp(((Record*)a)->key, ((Record*)b)->key);
}

// Сортировка Шелла
void shell_sort(Record *t, int n) {
    int step, i, j;
    Record tmp;
    for (step = n / 2; step > 0; step /= 2) {
        for (i = step; i < n; i++) {
            tmp = t[i];
            for (j = i; j >= step && strcmp(t[j - step].key, tmp.key) > 0; j -= step) {
                t[j] = t[j - step];
            }
            t[j] = tmp;
        }
    }
}

// Двоичный поиск
int binary_search(Record *t, int n, const char *key) {
    int left = 0, right = n - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        int cmp = strcmp(t[mid].key, key);
        if (cmp == 0)
            return mid;
        if (cmp < 0)
            left = mid + 1;
        else
            right = mid - 1;
    }
    return -1;
}

// Генерация случайной строки-ключа (из заглавных букв)
void random_key(char *buf, int len) {
    static const char chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 0; i < len; i++) {
        buf[i] = chars[rand() % (sizeof(chars)-1)];
    }
    buf[len] = '\0';
}

// Создание упорядоченного набора (ключи "KEY001" ... "KEY014")
void create_ordered(Record *t, int n) {
    for (int i = 0; i < n; i++) {
        sprintf(t[i].key, "KEY%03d", i+1);  // KEY001, KEY002, ...
        sprintf(t[i].data, "Данные для ключа %s", t[i].key);
    }
}

// Создание обратного порядка (просто копируем упорядоченный в обратном порядке)
void create_reversed(Record *t, int n) {
    Record tmp[MIN_SIZE];
    create_ordered(tmp, n);
    for (int i = 0; i < n; i++) {
        t[i] = tmp[n - 1 - i];
    }
}

// Создание случайного набора (случайные ключи)
void create_random(Record *t, int n) {
    for (int i = 0; i < n; i++) {
        random_key(t[i].key, KEY_LEN);
        sprintf(t[i].data, "Случайные данные #%d", i+1);
    }
}

// Тестирование для одного случая
void test_case(Record *t, int n, const char *case_name) {
    printf("\n========== %s ==========", case_name);
    print_table(t, n, "Исходная таблица:");

    // Сортируем
    shell_sort(t, n);
    print_table(t, n, "После сортировки Шелла:");

    // Поиск нескольких ключей
    printf("\nПоиск ключей:\n");
    // Возьмём несколько ключей из таблицы и один отсутствующий
    const char *keys_to_find[] = {
        t[0].key,               // первый
        t[n/2].key,             // средний
        t[n-1].key,             // последний
        "ZZZZZZ"                // заведомо отсутствующий
    };
    int num_keys = sizeof(keys_to_find) / sizeof(keys_to_find[0]);

    for (int i = 0; i < num_keys; i++) {
        int idx = binary_search(t, n, keys_to_find[i]);
        if (idx != -1)
            printf("Ключ %s найден: %s\n", keys_to_find[i], t[idx].data);
        else
            printf("Ключ %s не найден\n", keys_to_find[i]);
    }
}

int main() {
    srand(time(NULL));
    const int n = MIN_SIZE;  // 14 элементов

    Record table1[n], table2[n], table3[n];

    // 1. Упорядоченный случай
    create_ordered(table1, n);
    test_case(table1, n, "СЛУЧАЙ 1: Упорядоченные ключи");

    // 2. Обратный порядок
    create_reversed(table2, n);
    test_case(table2, n, "СЛУЧАЙ 2: Обратный порядок ключей");

    // 3. Случайный порядок
    create_random(table3, n);
    test_case(table3, n, "СЛУЧАЙ 3: Случайные ключи");

    return 0;
}