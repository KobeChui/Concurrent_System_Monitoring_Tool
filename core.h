#ifndef CORE_H
#define CORE_H

int get_number_of_cores();
    ///_|> descry: Retrieves the number of cores in the current machine, counted by number of processors in /proc/cpuinfo.
    ///_|> returning: This function returns the number of CPU cores, type: int

float get_cpu_max_frequency();
    ///_|> descry: Retrieves the maximum CPU frequency fromm the machine.
    ///_|> returning: This function returns the maximum CPU frequency in GHz, type: float

void print_core_info();
    ///_|> descry: Prints the number of CPU cores and maximum frequency.
    ///_|> returning: This function returns nothing

#endif