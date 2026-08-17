#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>

#include "signal_handler.h"

extern volatile sig_atomic_t pause_requested;

void handle_pause(int signal){
    pause_requested = 1;
    return;
}

void setup_signal_handlers(){

    struct sigaction new_action;
    new_action.sa_handler = SIG_IGN;
    new_action.sa_flags = SA_RESTART;
    sigemptyset(&new_action.sa_mask);

    if(sigaction(SIGTSTP, &new_action, NULL) == -1){
        perror("Sigaction with SIGTSTP (CTRL + Z)");
        exit(1);
    }
    
    new_action.sa_handler = handle_pause;
    if(sigaction(SIGINT, &new_action, NULL) == -1){
        perror("Sigaction with SIGINT (CTRL + C)");
        exit(1);
    }

    return;
}
