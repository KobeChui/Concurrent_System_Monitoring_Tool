#ifndef CPU_H
#define CPU_H

typedef struct {
    long double total_time;
    long double idle_time;
} cpu_util_stats;

cpu_util_stats get_utilization();
    ///_|> descry: Gets total and idle CPU time from /proc/stat.
    ///_|> returning: This function returns a struct containing CPU utilization stats, type: cpu_util_stats

double calculate_utilization(cpu_util_stats before, cpu_util_stats after);
    ///_|> descry: Calculates CPU usage in percentage between two time intervals before and after.
    ///_|> before: The CPU utilization from previous timestamp, type: cpu_util_stats
    ///_|> after: The CPU utilization from current timestamp, type: cpu_util_stats
    ///_|> returning: This function returns the difference of CPU usage percentage between the intervals, type: double

double get_cpu(cpu_util_stats *prev_cpu_stats);
    ///_|> descry: Gets current CPU utilization and compares it to the last reading.
    ///_|> before: Pointer to previous CPU utilization, type: cpu_util_stats*
    ///_|> returning: This function returns the CPU usage percentage since the last reading, type: double

#endif