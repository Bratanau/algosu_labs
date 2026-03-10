#ifndef COMPUTER_H
#define COMPUTER_H

#define STR_LEN 30

struct computer {
    char surname[STR_LEN];
    int cpu_count;
    char cpu_type[STR_LEN];
    int ram_size;       
    char video_controller[STR_LEN];
    int video_memory;   
    char hd_type[STR_LEN];
    int hd_count;
    int hd_capacity;   
    int integrated_controllers;
    int external_devices;
    char os[STR_LEN];
};

#endif