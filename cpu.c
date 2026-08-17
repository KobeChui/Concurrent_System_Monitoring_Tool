#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

cpu_util_stats get_utilization(){
    long double cpu_times[8];
    
    FILE *f = fopen("/proc/stat", "r");
    if(f == NULL){
        fprintf(stderr, "Failed to open \"/proc/stat\": ");
        perror("");
        exit(1);
    }
    fscanf(f, "%*s %Lf %Lf %Lf %Lf %Lf %Lf %Lf %Lf", &cpu_times[0], &cpu_times[1], &cpu_times[2], &cpu_times[3], &cpu_times[4], &cpu_times[5], &cpu_times[6], &cpu_times[7]);
    if(fclose(f) != 0){
        fprintf(stderr, "Failed to close \"/proc/stat\": ");
        perror("");
        exit(1);
    }

    cpu_util_stats stats;
    stats.total_time = 0;
    for(int i=0; i<8; i++){
        stats.total_time += cpu_times[i];
    }
    stats.idle_time = cpu_times[3];
    return stats;
}

double calculate_utilization(cpu_util_stats before, cpu_util_stats after){
    long double total_difference = after.total_time - before.total_time;
    if (total_difference == 0){
        return 0;
    }
    return (1 - (after.idle_time - before.idle_time)/total_difference) * 100;
}

double get_cpu(cpu_util_stats *prev_cpu_stats){
    cpu_util_stats cur_cpu_stats = get_utilization();
    double utilization = calculate_utilization(*prev_cpu_stats, cur_cpu_stats);
    prev_cpu_stats->idle_time = cur_cpu_stats.idle_time;
    prev_cpu_stats->total_time = cur_cpu_stats.total_time;
    return utilization;
}