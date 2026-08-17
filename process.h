#ifndef PROCESS_H
#define PROCESS_H

#include <sys/types.h>

typedef struct Process_Info{
    pid_t memory_pid;
    pid_t cpu_pid;
    pid_t core_count_pid;
    pid_t core_max_freq_pid;
    int memory_pipe[2];
    int cpu_pipe[2];
    int core_count_pipe[2];
    int core_max_freq_pipe[2];
} process_info;

void terminate_all_children(process_info *process);
    ///_|> descry: Sends SIGTERM to all child processes to terminate them.
    ///_|> process: The struct that holds all PIDs of child processes, type: process_info*
    ///_|> returning: This function does not return anything

void pause_all_children(process_info* process);
    ///_|> descry: Sends SIGSTOP to all child processes to pause them.
    ///_|> process: The struct that holds all PIDs of child processes, type: process_info*
    ///_|> returning: This function does not return anything

void resume_all_children(process_info* process);
    ///_|> descry: Sends SIGCONT to all child processes to resume running.
    ///_|> process: The struct that holds all PIDs of child processes, type: process_info*
    ///_|> returning: This function does not return anything

void setup_pipes_and_pid(process_info *process);
    ///_|> descry: Initializes pipe file descriptors and sets all child PIDs to -1.
    ///_|> process: The pointer to struct containing all pipe FDs and child processes PIDs, type: process_info*
    ///_|> returning: This function returns nothing

pid_t fork_memory_process(int fd[2], int samples, int tdelay);
    ///_|> descry: Forks a child process to get memory usage over time.
    ///_|> fd: The pipe to write to parent, type: int[2]
    ///_|> samples: The umber of CPU samples to be sent, type: int
    ///_|> tdelay: The delay between samples in microsecond, type: int
    ///_|> returning: This function returns the PID of the child process, type: pid_t

pid_t fork_cpu_process(int fd[2], int samples, int tdelay);
    ///_|> descry: Forks a child process to compute CPU utilization over time.
    ///_|> fd: The pipe to write to parent, type: int[2]
    ///_|> samples: The umber of CPU samples to be sent, type: int
    ///_|> tdelay: The delay between samples in microsecond, type: int
    ///_|> returning: This function returns the PID of the child process, type: pid_t

pid_t fork_core_count_process(int fd[2]);
    ///_|> descry: Forks a child process to retrieve the number of cores in the machine.
    ///_|> fd: The pipe to write to parent, type: int[2]
    ///_|> returning: This function returns the PID of the child process, type: pid_t

pid_t fork_core_max_freq_process(int fd[2]);
    ///_|> descry: Forks a child process to retrieve the maximum frequency of cores in the machine.
    ///_|> fd: The pipe to write to parent, type: int[2]
    ///_|> returning: This function returns the PID of the child process, type: pid_t

int read_from_child(int fd, void* read_to, size_t size, char* name, int failure_flag);
    ///_|> descry: Reads data from a pipe.
    ///_|> fd: The side of file descriptor to be read from, type: int
    ///_|> read_to: The pointer to where the read data is stored, type: void*
    ///_|> size: The number of bytes to read, type: size_t
    ///_|> name: The name of the child process, type: char*
    ///_|> failure_flag: The flag that indicates whether the child failed. If true, suppress error output, type: int
    ///_|> returning: The function returns 1 on success, 0 otherwise, type: int

int check_process_status(pid_t pid, char* name, int* failure, int option);
    ///_|> descry: Checks the status of a child process using waitpid.
    ///_|> pid: The PID of the child process to be checked, type: pid_t
    ///_|> name: The name of the process, type: char*
    ///_|> failure: The flag that indicates whether the child failed, set to 1 if failed, 0 if normal, type: int*
    ///_|> option: The option for waitpid, type: int
    ///_|> returning: This function returns 1 iff process exited with errors, 0 otherwise, type: int

int check_cores_processes(process_info* process, int* core_count_failed, int* core_max_freq_failed, int option);
    ///_|> descry: Checks both core-count and max-freq child process statuses, terminates if failed.
    ///_|> process: The pointer to struct containing all pipe FDs and child processes PIDs, type: process_info*
    ///_|> core_count_failed: The flag for core count child indicating whether it has failed or not, type: int*
    ///_|> core_max_freq_failed: The flag for core max freq child indicating whether it has failed or not, type: int*
    ///_|> option: The option for waitpid, type: int
    ///_|> returning: This function always return 0, exits on error, type: int

void check_all_child_processes(process_info* process, int* memory_failed, int* cpu_failed, int* core_count_failed, int* core_max_freq_failed);
    ///_|> descry: Checks all child process statuses and terminates if failed.
    ///_|> process: The pointer to struct containing all pipe FDs and child processes PIDs, type: process_info*
    ///_|> memory_failed: The flag for memory child indicating whether it has failed or not, type: int*
    ///_|> cpu_failed: The flag for CPU child indicating whether it has failed or not, type: int*
    ///_|> core_count_failed: The flag for core count child indicating whether it has failed or not, type: int*
    ///_|> core_max_freq_failed: The flag for core max freq child indicating whether it has failed or not, type: int*
    ///_|> returning: This function does not return anything

#endif