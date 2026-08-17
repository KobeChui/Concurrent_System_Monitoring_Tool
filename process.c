#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>  

#include "process.h"
#include "memory.h"
#include "cpu.h"
#include "core.h"
#include "signal_handler.h"

extern int usleep(useconds_t);

void send_signal_to_all_children(process_info* process, int signal){
    ///_|> descry: Sends a signal to all child processes contained in process_info.
    ///_|> process: The struct that holds all PIDs of child processes, type: process_info*
    ///_|> signal: The signal to be sent, type: int
    ///_|> returning: This function returns mothing
    if (process->memory_pid > 0){
        kill(process->memory_pid, signal);
    }
    if (process->cpu_pid > 0){
        kill(process->cpu_pid, signal);
    }
    if (process->core_count_pid > 0){
        kill(process->core_count_pid, signal);
    }
    if (process->core_max_freq_pid > 0){
        kill(process->core_max_freq_pid, signal);
    }
    return;
}

void pause_all_children(process_info* process){
    send_signal_to_all_children(process, SIGSTOP);
    return;
}

void resume_all_children(process_info* process){
    send_signal_to_all_children(process, SIGCONT);
    return;
}

void terminate_all_children(process_info* process){
    send_signal_to_all_children(process, SIGTERM);
    return;
}

void setup_pipes_and_pid(process_info *process){
    if (pipe(process->memory_pipe) == -1) {
        perror("Failed to create memory pipe");
        exit(1);
    }

    if (pipe(process->cpu_pipe) == -1) {
        perror("Failed to create CPU pipe");
        exit(1);
    }

    if (pipe(process->core_count_pipe) == -1) {
        perror("Failed to create Core pipe for number of cores");
        exit(1);
    }

    if (pipe(process->core_max_freq_pipe) == -1) {
        perror("Failed to create Core pipe for maximum frequency");
        exit(1);
    }

    process->memory_pid = -1;
    process->cpu_pid = -1;
    process->core_count_pid = -1;
    process->core_max_freq_pid = -1;

    return;
}

pid_t fork_memory_process(int fd[2], int samples, int tdelay){
    pid_t pid = fork();
    if (pid == 0) {
        close(fd[0]);
        for (int i = 0; i < samples; i++) {
            usleep(tdelay);
            memory_info mem = get_memory();
            if (write(fd[1], &mem, sizeof(memory_info)) < 0){
                close(fd[1]);
                exit(1);
            }
        }
        close(fd[1]);
        exit(0);
    }
    return pid;
}

pid_t fork_cpu_process(int fd[2], int samples, int tdelay){
    pid_t pid = fork();
    if (pid == 0) {
        close(fd[0]);
        cpu_util_stats prev = get_utilization();;
        for (int i = 0; i < samples; i++) {
            usleep(tdelay);
            double util = get_cpu(&prev);
            if (write(fd[1], &util, sizeof(double)) < 0){
                close(fd[1]);
                exit(1);
            }
        }
        close(fd[1]);
        exit(0);
    }
    return pid;
}

pid_t fork_core_count_process(int fd[2]){
    pid_t pid = fork();
    if (pid == 0) {
        close(fd[0]);

        int cores = get_number_of_cores();
        
        if (write(fd[1], &cores, sizeof(int)) < 0){
            close(fd[1]);
            exit(1);
        }
        close(fd[1]);
        exit(0);
    }
    return pid;
}

pid_t fork_core_max_freq_process(int fd[2]){
    pid_t pid = fork();
    if (pid == 0) {
        close(fd[0]);

        float freq = get_cpu_max_frequency();
        
        if (write(fd[1], &freq, sizeof(float)) < 0){
            close(fd[1]);
            exit(1);
        }
        close(fd[1]);
        exit(0);
    }
    return pid;
}

int read_from_child(int fd, void* read_to, size_t size, char* name, int failure_flag){
    if (read(fd, read_to, size) <= 0) {
        if(!failure_flag){
            fprintf(stderr, "%s child closed pipe early.\n", name);
        }
        return 0;
    }
    return 1;
}

int check_process_status(pid_t pid, char* name, int* failure, int option) {
    if(pid == -1){
        return 0;
    }
    int status;
    
    waitpid(pid, &status, option);
    
    *failure = 1;
    
    //Terminated normally with error; exit(1) called
    if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
        fprintf(stderr, "%s process exited with failure.\n", name);
        return 1;
    }

    //No failure
    *failure = 0;
    return 0;
}

int check_cores_processes(process_info* process, int* core_count_failed, int* core_max_freq_failed, int option){
    if (check_process_status(process->core_count_pid, "Core Count", core_count_failed, option) ||
        check_process_status(process->core_max_freq_pid, "Core Max Freq", core_max_freq_failed, option)) 
        {
            terminate_all_children(process);
            exit(1);
    }
    return 0;
}

void check_all_child_processes(process_info* process, int* memory_failed, int* cpu_failed, int* core_count_failed, int* core_max_freq_failed){
    if (check_process_status(process->memory_pid, "Memory", memory_failed, WNOHANG) ||
        check_process_status(process->cpu_pid, "CPU", cpu_failed, WNOHANG) || 
        check_cores_processes(process, core_count_failed, core_max_freq_failed, WNOHANG)) 
        {
            terminate_all_children(process);
            exit(1);
    }
    return; 
}
