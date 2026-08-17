#ifndef MEMORY_H
#define MEMORY_H

typedef struct {
    long double total_memory;
    long double free_memory;
    long double used_memory;
} memory_info;

memory_info get_memory();
    ///_|> descry: Gets total, free, and used memory using sysinfo.
    ///_|> returning: This function returns a struct containing total, free, and used memory in GB, type: memory_info

#endif