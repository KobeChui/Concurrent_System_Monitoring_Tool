#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include <string.h>
#include <math.h>

#include "parse_command_line.h"
#include "graph.h"
#include "utilities.h"
#include "core.h"
#include "memory.h"
#include "cpu.h"
#include "process.h"
#include "signal_handler.h"

volatile sig_atomic_t pause_requested = 0;

int pause_program(process_info* process, int print_row){
    ///_|> descry: A function that pauses the program and prompts the user to input whether they want to exit or resume the program.
    ///_|> process: The struct containing pids and pipes of all forked processes, type: process_info*
    ///_|> print_row: The line in terminal where the prompts will be printed on, type: int
    ///_|> returning: This function returns 0 iff user inputs 'Y' or 'y' and 1 iff user inputs 'N' or 'n'. 
    int quit_requested;
    
    pause_all_children(process);

    //Sanity check: only accepts valid inputs; Keeps prompting until valid input is received.
    do{
        printf("\n\033[%d;0H\x1b[2K", print_row);
        printf("Quit the program? (Y/N): ");
        
        char input[3];
        fgets(input, sizeof(input), stdin);

        printf("\033[%d;0H\x1b[2K", print_row + 1);

        if (input[0] == 'y' || input[0] == 'Y') {
            quit_requested = 1;
            printf("\x1b[2K");
            break;
        } 

        else if(input[0] == 'n' || input[0] == 'N'){
            quit_requested = 0;
            printf("\033[%d;0H\x1b[2K", print_row);
            break;
        }
        
        else {
            printf("Invalid input.\n");
        }
    } while(1);

    //Reset
    pause_requested = 0;

    resume_all_children(process);

    return quit_requested;
}

void exit_program(process_info* process, int print_row){
    ///_|> descry: A function that exits the program with exit message prompted.
    ///_|> process: The struct containing pids and pipes of all forked processes, type: process_info*
    ///_|> print_row: The line in terminal where the prompts will be printed on, type: int
    ///_|> returning: This function does not return anything. 

    printf("\n\033[%d;0H", print_row);
    printf("Program terminated. Bye Bye\n");

    terminate_all_children(process);
    exit(0);
}

int main(int argc, char **argv) {
    ///_|> descry: Main driver of myMonitoringTool. Handles signal, parses flags, fork child processes, and draws graphs.
    ///_|> argc: The number of arguments from the command line, type: int
    ///_|> argv: The list of arguments from the command line, type: char**
    ///_|> returning: This function returns 0 on normal exit and 1 on failure, type: int

    setup_signal_handlers();

    flags flag;
    parse_arguments(&flag, argc, argv);

    process_info process;
    setup_pipes_and_pid(&process);

    int show_mem_graph = flag.has_memory_flag || !(flag.has_memory_flag || flag.has_cpu_flag || flag.has_cores_flag);
    int show_cpu_graph = flag.has_cpu_flag || !(flag.has_memory_flag || flag.has_cpu_flag || flag.has_cores_flag);
    int show_core_diagram = flag.has_cores_flag || !(flag.has_memory_flag || flag.has_cpu_flag || flag.has_cores_flag);

    //Only fork for child processes if flagged by user.
    if (show_mem_graph) {
        process.memory_pid = fork_memory_process(process.memory_pipe, flag.samples_value, flag.tdelay_value);
    }

    if (show_cpu_graph) {
        process.cpu_pid = fork_cpu_process(process.cpu_pipe, flag.samples_value, flag.tdelay_value);
    }

    if (show_core_diagram) {
        process.core_count_pid = fork_core_count_process(process.core_count_pipe);
        process.core_max_freq_pid = fork_core_max_freq_process(process.core_max_freq_pipe);
    }

    // Parent Process closes one end of pipe.
    close(process.memory_pipe[1]);
    close(process.cpu_pipe[1]);
    close(process.core_count_pipe[1]);
    close(process.core_max_freq_pipe[1]);

    //Graph initial values
    int graph_row = 3;
    int memory_graph_y = 12;
    int cpu_graph_y = 10;

    //Error flags if processes are terminated.
    int memory_failed = 0;
    int cpu_failed = 0;
    int core_count_failed = 0;
    int core_max_freq_failed = 0;

    //CTRL C prompts variables
    int quit_requested = 0;
    int quit_msg_row = graph_row + show_mem_graph * (memory_graph_y + 3) + show_cpu_graph * (cpu_graph_y + 3);

    printf("\033[2J\033[H");

    //Skips loop if only core is requested.
    if (flag.has_cores_flag && !(flag.has_memory_flag || flag.has_cpu_flag)){
        flag.samples_value = 0;
    }
    for (int i = 0; i < flag.samples_value; i++) {   
        printf("\n");     
        if (pause_requested) {
            quit_requested = pause_program(&process, quit_msg_row);
        }
        
        if (quit_requested) {
            exit_program(&process, quit_msg_row + 1);
        }

        //Flags on whether graphs are allowed to be printed
        int read_memory = 0;
        int read_cpu = 0;
        
        //Retrive memory information if needed
        memory_info mem_info;
        if(show_mem_graph){
            read_memory = read_from_child(process.memory_pipe[0], &mem_info, sizeof(memory_info), "Memory", !pause_requested || memory_failed);
        }

        //Retrive CPU information if needed
        double cpu_usage;
        if(show_cpu_graph){
            read_cpu = read_from_child(process.cpu_pipe[0], &cpu_usage, sizeof(double), "CPU", !pause_requested || cpu_failed);
        }

        //Checks on processes; Exit if process is exited with error
        check_all_child_processes(&process, &memory_failed, &cpu_failed, &core_count_failed, &core_max_freq_failed);
        
        printf("\n\033[H");
        printf("Number of Samples: %d -- every %d microsecs (%.3f secs)\n", flag.samples_value, flag.tdelay_value, (float)flag.tdelay_value/1000000);
        
        graph_row = 3;

        //Prints memory info if it is successfully read.
        if (read_memory) {
            print_memory_graph((int)ceil(mem_info.total_memory), graph_row, memory_graph_y, flag.samples_value);
            plot_on_graph(mem_info.total_memory, mem_info.used_memory, graph_row, "GB", memory_graph_y, i + 1, "Used Memory:");
            graph_row += memory_graph_y + 3;
        }

        //Prints CPU info if it is successfully read.
        if (read_cpu) {
            print_cpu_graph(graph_row, cpu_graph_y, flag.samples_value);
            plot_on_graph(100.0, cpu_usage, graph_row, "%%", cpu_graph_y, i + 1, "CPU Utilization:");
            graph_row += cpu_graph_y + 3;
        }
    }

    printf("\033[%d;0H", graph_row);
    
    //Retrive core information if needed
    if (show_core_diagram) {
        int cores;
        float freq;
        
        //Core processes do not require delay, so wait for processes to exit to ensure information is retrieved.
        check_cores_processes(&process, &core_count_failed, &core_max_freq_failed, 0);
        read_from_child(process.core_count_pipe[0], &cores, sizeof(int), "Core", core_count_failed);
        read_from_child(process.core_max_freq_pipe[0], &freq, sizeof(int), "Core", core_max_freq_failed);

        //Prints core info onto terminal.
        printf("Number of Cores: %d @ %.2f GHz\n", cores, freq);
        int col = get_closest_factor(cores);
        int row = cores / col;
        print_core_diagram(row, col);
    }

    waitpid(process.memory_pid, NULL, 0);
    waitpid(process.cpu_pid, NULL, 0);
    waitpid(process.core_count_pid, NULL, 0);
    waitpid(process.core_max_freq_pid, NULL, 0);

    close(process.memory_pipe[0]);
    close(process.cpu_pipe[0]);
    close(process.core_count_pipe[0]);
    close(process.core_max_freq_pipe[0]);
    
    return 0;
}
