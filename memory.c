#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include "memory.h"

memory_info get_memory(){
    struct sysinfo system_info;
    if(sysinfo(&system_info) == -1){
        fprintf(stderr, "Error encounter while retrieving memory information.");
        exit(1);
    }

    long double byte_to_gb_constant = 1073741824.0;
    memory_info mem_info;
    mem_info.total_memory = (long double)system_info.totalram/byte_to_gb_constant;
    mem_info.free_memory = (long double)system_info.freeram/byte_to_gb_constant;
    mem_info.used_memory = mem_info.total_memory - mem_info.free_memory;
    return mem_info;
}