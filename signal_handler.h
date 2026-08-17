#ifndef SIGNAL_HANDLER_H
#define SIGNAL_HANDLER_H

#include <unistd.h>
#include <signal.h>  

extern volatile sig_atomic_t pause_requested;

void handle_pause(int signal);
    ///_|> descry: Signal handler for SIGINT, sets pause_requested flag.
    ///_|> signal: The signal value received, type: int
    ///_|> returning: This function returns nothing

void setup_signal_handlers();
    ///_|> descry: Installs signal handlers, ignores Ctrl+Z, and handles Ctrl+C.
    ///_|> returning: This function returns nothing

#endif