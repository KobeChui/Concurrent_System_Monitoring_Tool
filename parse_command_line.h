#ifndef PARSE_H
#define PARSE_H

typedef struct Flags{
    int samples_value;
    int tdelay_value;
    int has_memory_flag;
    int has_cpu_flag;
    int has_cores_flag;
    int has_samples_flag;
    int has_tdelay_flag;
}flags;

void parse_arguments(flags* flag, int argc, char** argv);
    ///_|> descry: A parsing function that reads and stores arguments into flag from the command line.
    ///_|> flag: A struct containing acceptable flags for this program, type: flags*
    ///_|> argc: The number of arguments from the command line, type: int
    ///_|> argv: The list of arguments from the command line, type: char**
    ///_|> returning: This function returns nothing

#endif