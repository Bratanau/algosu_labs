// query.c – обработка запросов
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "computer.h"

void print_record(struct computer *c) {
    printf("%-20s %-3d %-15s %-5d %-15s %-5d %-5s %-3d %-7d %-5d %-5d %-15s\n",
           c->surname, c->cpu_count, c->cpu_type, c->ram_size,
           c->video_controller, c->video_memory, c->hd_type,
           c->hd_count, c->hd_capacity, c->integrated_controllers,
           c->external_devices, c->os);
}

void print_all(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        perror("fopen");
        exit(1);
    }
    struct computer c;
    printf("%-20s %-3s %-15s %-5s %-15s %-5s %-5s %-3s %-7s %-5s %-5s %-15s\n",
           "Фамилия", "CPU", "Тип CPU", "RAM", "Видео", "VRAM", "HDD", "HDDcnt", "HDDcap", "IntCtrl", "ExtDev", "OS");
    while (fread(&c, sizeof(c), 1, f) == 1) {
        print_record(&c);
    }
    fclose(f);
}

void print_incomplete(const char *filename, int p) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        perror("fopen");
        exit(1);
    }
    struct computer c;
    int found = 0;
    printf("Неукомплектованные компьютеры (внешних устройств < %d):\n", p);
    printf("%-20s %-3s %-15s %-5s %-15s %-5s %-5s %-3s %-7s %-5s %-5s %-15s\n",
           "Фамилия", "CPU", "Тип CPU", "RAM", "Видео", "VRAM", "HDD", "HDDcnt", "HDDcap", "IntCtrl", "ExtDev", "OS");
    while (fread(&c, sizeof(c), 1, f) == 1) {
        if (c.external_devices < p) {
            print_record(&c);
            found++;
        }
    }
    if (found == 0) {
        printf("Нет таких компьютеров.\n");
    }
    fclose(f);
}

void usage(const char *progname) {
    fprintf(stderr, "Использование:\n");
    fprintf(stderr, "  %s -f <файл>            - распечатать содержимое файла\n", progname);
    fprintf(stderr, "  %s -p <число> <файл>    - вывести список неукомплектованных компьютеров (внешних устройств < число)\n", progname);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        usage(argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "-f") == 0) {
        if (argc != 3) {
            usage(argv[0]);
            return 1;
        }
        print_all(argv[2]);
    }
    else if (strcmp(argv[1], "-p") == 0) {
        if (argc != 4) {
            usage(argv[0]);
            return 1;
        }
        int p = atoi(argv[2]);
        if (p < 0) {
            fprintf(stderr, "Параметр p должен быть неотрицательным.\n");
            return 1;
        }
        print_incomplete(argv[3], p);
    }
    else {
        usage(argv[0]);
        return 1;
    }

    return 0;
}