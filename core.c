#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core.h"
#include "utilities.h"
#include "graph.h"

int get_number_of_cores(){
    FILE *f = fopen("/proc/cpuinfo", "r");
    
    if(f == NULL){
        fprintf(stderr, "Failed to open \"/proc/cpuinfo\": ");
        perror("");
        exit(1);
    }

    char line[81];
    int num_cores = 0;
    while(fgets(line, sizeof(line), f) != NULL){
        if (strncmp(line, "processor", (int)strlen("processor")) == 0) {
            num_cores++;
        }
    }

    if(fclose(f) != 0){
        fprintf(stderr, "Failed to close \"/proc/cpuinfo\"");
        perror("");
        exit(1);
    }

    return num_cores;
}

float get_cpu_max_frequency(){
    FILE *f = fopen("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq", "r");\

    if(f == NULL){
        fprintf(stderr, "Failed to open \"/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq\": ");
        perror("");
        exit(1);
    }

    float freq = 0;
    int success = fscanf(f, "%f", &freq);
    
    if(fclose(f) != 0){
        fprintf(stderr, "Failed to close \"/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq\": ");
        exit(1);
    }
    if (success != 1) {
        fprintf(stderr, "Error reading CPU frequency: ");
        perror("");
        exit(1);
    }
    return freq/1000000;
}

void print_core_info(){
    int num_of_cores = get_number_of_cores();
    float max_freq = get_cpu_max_frequency();
    printf("Number of Cores: %d @ %.2f GHz\n", num_of_cores, max_freq);
    return;
}