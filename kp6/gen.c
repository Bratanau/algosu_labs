// gen.c – генерация бинарного файла с записями
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

    const char *surnames[] = {"Иванов", "Петров", "Сидоров", "Кузнецов", "Смирнов",
                              "Васильев", "Попов", "Новиков", "Федоров", "Морозов"};
    const char *cpu_types[] = {"Intel Core i3", "Intel Core i5", "Intel Core i7",
                               "AMD Ryzen 3", "AMD Ryzen 5", "AMD Ryzen 7"};
    const char *video_types[] = {"встроенный", "внешний", "видео-шина"};
    const char *hd_types[] = {"SAS", "SATA"};
    const char *os_list[] = {"Windows 10", "Windows 11", "Linux Ubuntu", "macOS", "FreeBSD"};

    FILE *f = fopen("computers.bin", "wb");
    if (!f) {
        perror("fopen");
        return 1;
    }

    int n = 25;
    struct computer comp;

    for (int i = 0; i < n; ++i) {
        // Фамилия
        random_choice(comp.surname, NAME_LEN, surnames, sizeof(surnames)/sizeof(surnames[0]));
        // Число процессоров (1 или 2)
        comp.cpu_count = (rand() % 2) + 1;
        // Тип процессора
        random_choice(comp.cpu_type, PROC_TYPE_LEN, cpu_types, sizeof(cpu_types)/sizeof(cpu_types[0]));
        // ОЗУ (4, 8, 16, 32 ГБ)
        int ram_choices[] = {4, 8, 16, 32};
        comp.ram_size = ram_choices[rand() % 4];
        // Видеоконтроллер
        random_choice(comp.video_controller, VIDEO_TYPE_LEN, video_types, sizeof(video_types)/sizeof(video_types[0]));
        // Видеопамять (0, 1, 2, 4, 8 ГБ)
        int vram_choices[] = {0, 1, 2, 4, 8};
        comp.video_memory = vram_choices[rand() % 5];
        // Тип винчестера
        random_choice(comp.hd_type, HD_TYPE_LEN, hd_types, sizeof(hd_types)/sizeof(hd_types[0]));
        // Количество винчестеров (1-3)
        comp.hd_count = rand() % 3 + 1;
        // Суммарная ёмкость (ГБ)
        int cap_choices[] = {256, 512, 1024, 2048};
        comp.hd_capacity = cap_choices[rand() % 4] * comp.hd_count;
        // Интегрированные контроллеры
        comp.integrated_controllers = rand() % 5;
        // Внешние устройства (0-9)
        comp.external_devices = rand() % 10;
        // ОС
        random_choice(comp.os, OS_LEN, os_list, sizeof(os_list)/sizeof(os_list[0]));

        fwrite(&comp, sizeof(comp), 1, f);
    }

    fclose(f);
    printf("Сгенерировано %d записей в файл computers.bin\n", n);

    // Вывод содержимого в виде таблицы
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