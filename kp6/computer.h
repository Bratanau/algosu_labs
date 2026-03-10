// computer.h
#ifndef COMPUTER_H
#define COMPUTER_H

#define NAME_LEN 30
#define PROC_TYPE_LEN 20
#define VIDEO_TYPE_LEN 20
#define HD_TYPE_LEN 10
#define OS_LEN 20

struct computer {
    char surname[NAME_LEN];
    int cpu_count;
    char cpu_type[PROC_TYPE_LEN];
    int ram_size;               // в ГБ
    char video_controller[VIDEO_TYPE_LEN];
    int video_memory;            // в ГБ
    char hd_type[HD_TYPE_LEN];
    int hd_count;
    int hd_capacity;             // суммарная ёмкость в ГБ
    int integrated_controllers;
    int external_devices;        // ключевое поле для условия
    char os[OS_LEN];
};

#endif