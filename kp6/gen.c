#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "computer.h"

void random_choice(char *dest, size_t size, const char *choices[], int count) {
    int idx = rand() % count;
    strncpy(dest, choices[idx], size - 1);
    dest[size - 1] = '\0';
}

int main() {
    srand(time(NULL));

    const char *surnames[] = {"Иванов", "Петров", "Сидоров", "Кузнецов"};
    const char *cpu_types[] = {"Intel Core i3", "Intel Core i5", "Intel Core i7"};
    const char *video_types[] = {"встроенный", "внешний", "видео-шина"};
    const char *hd_types[] = {"SAS", "SATA"};
    const char *os_list[] = {"Windows 10", "Windows 11", "Ubuntu"};

    FILE *f = fopen("computers.bin", "wb");
    if (!f) {
        perror("fopen");
        return 1;
    }

    int n = 25;
    struct computer comp;

    for (int i = 0; i < n; ++i) {
        random_choice(comp.surname, STR_LEN, surnames, sizeof(surnames)/sizeof(surnames[0]));
        comp.cpu_count = (rand() % 2) + 1;
        random_choice(comp.cpu_type, STR_LEN, cpu_types, sizeof(cpu_types)/sizeof(cpu_types[0]));
        int ram_choices[] = {4, 8, 16, 32};
        comp.ram_size = ram_choices[rand() % 4];
        random_choice(comp.video_controller, STR_LEN, video_types, sizeof(video_types)/sizeof(video_types[0]));
        int vram_choices[] = {0, 1, 2, 4, 8};
        comp.video_memory = vram_choices[rand() % 5];
        random_choice(comp.hd_type, STR_LEN, hd_types, sizeof(hd_types)/sizeof(hd_types[0]));
        comp.hd_count = rand() % 3 + 1;
        int cap_choices[] = {256, 512, 1024, 2048};
        comp.hd_capacity = cap_choices[rand() % 4] * comp.hd_count;
        comp.integrated_controllers = rand() % 5;
        comp.external_devices = rand() % 10;
        random_choice(comp.os, STR_LEN, os_list, sizeof(os_list)/sizeof(os_list[0]));

        fwrite(&comp, sizeof(comp), 1, f);
    }

    fclose(f);
    printf("Сгенерировы записи в файл\n");

    printf("\nСодержимое файла computers.bin:\n");
    f = fopen("computers.bin", "rb");
    if (!f) {
        perror("fopen");
        return 1;
    }

    printf("%-20s %-3s %-15s %-5s %-15s %-5s %-5s %-3s %-7s %-5s %-5s %-15s\n",
           "Фамилия", "CPU", "Тип CPU", "RAM", "Видео", "VRAM", "HDD", "HDDcnt", "HDDcap", "IntCtrl", "ExtDev", "OS");
    while (fread(&comp, sizeof(comp), 1, f) == 1) {
        printf("%-20s %-3d %-15s %-5d %-15s %-5d %-5s %-3d %-7d %-5d %-5d %-15s\n",
               comp.surname, comp.cpu_count, comp.cpu_type, comp.ram_size,
               comp.video_controller, comp.video_memory, comp.hd_type,
               comp.hd_count, comp.hd_capacity, comp.integrated_controllers,
               comp.external_devices, comp.os);
    }
    fclose(f);

    return 0;
}