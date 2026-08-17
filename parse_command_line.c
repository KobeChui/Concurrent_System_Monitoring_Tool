#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "parse_command_line.h"

int positional_argument_checker(int argc, char** argv){
    ///_|> descry: Checks whether there is a positional argument appearing after one or more flags are indicated.
    ///_|> argc: The number of arguments from the command line, type: int
    ///_|> argv: The list of arguments from the command line, type: char**
    ///_|> returning: This function returns 0 iff no positional arguments appear after flags are indicated,
    ///_|>            otherwise, returns the index of the first invalid positional argument in argv, type: int

    int flag_found = 0;
    for(int i=1; i<argc; i++){
        if(strncmp(argv[i], "--", 2) == 0){ //flag found
            flag_found++;
            continue;
        }
        //Positional argument found
        if(flag_found){ //Checks if there is a flag indicated before this positonal argument
            return i;
        }
    }
    return 0;
}

void parse_arguments(flags* flag, int argc, char** argv){

    int option;
    int option_index = 0;
    opterr = 0; //Prevents getopt_long() from printing error messasges in the shell
    char* endptr = NULL;

    flag->samples_value = 20;
    flag->tdelay_value = 500000;
    flag->has_memory_flag = 0;
    flag->has_cpu_flag = 0;
    flag->has_cores_flag = 0;
    flag->has_samples_flag = 0;
    flag->has_tdelay_flag = 0;

    static struct option long_options[] = {
        {"memory", no_argument, NULL, 'm'},
        {"cpu", no_argument, NULL, 'c'},
        {"cores", no_argument, NULL, 'o'},
        {"samples", optional_argument, NULL, 's'},
        {"tdelay", optional_argument, NULL, 't'},
        {0,0,0,0}
    };

    if((option = positional_argument_checker(argc, argv))){
        fprintf(stderr, "Invalid position argument: %s\n", argv[option]);
        exit(1);
    }

    //To check flags and corresponding arguments
    while((option = getopt_long(argc, argv, "", long_options, &option_index)) != -1){       
        
        //Making a copy of a flag for format checking
        char option_name[strlen(argv[optind - 1] + 2) + 1];
        strncpy(option_name, argv[optind - 1] + 2, strlen(argv[optind - 1] + 2) + 1);
        option_name[strlen(argv[optind - 1] + 2)] = '\0';

        //optarg != null when has_arg is optional_argument or required_argument (ex. samples and tdelay)
    
        if(optarg != NULL && strchr(option_name, '=')){
            *strchr(option_name, '=') = '\0';
        }

        if (strcmp(option_name, long_options[option_index].name) != 0) {
            fprintf(stderr, "Invalid option: %s\n", argv[optind - 1]);
            exit(1);
        }

        switch(option){
            case 'm':
                flag->has_memory_flag = 1;
                break;
            case 'c':
                flag->has_cpu_flag = 1;
                break;
            case 'o':
                flag->has_cores_flag = 1;
                break;
            case 's':
                flag->has_samples_flag = 1;

                //Check if argument is provided
                if(optarg == NULL || optarg[0] == '\0'){
                    break;
                }

                flag->samples_value = (int)strtol(optarg, &endptr, 10);

                //Check possibly non-digit character, and only accept ints
                //If strtol reads a int then endptr must be \0
                if(*endptr != '\0' || flag->samples_value <= 0){
                    fprintf(stderr, "Invalid argument for --samples.\n");
                    exit(1);
                }
                break;
            case 't':
                flag->has_tdelay_flag = 1;

                //Check if argument is provided
                if(optarg == NULL || optarg[0] == '\0'){
                    break;
                }

                flag->tdelay_value = (int)strtol(optarg, &endptr, 10);

                //Check possibly non-digit character, and only accept ints
                //If strtol reads a int then endptr must be \0
                if(*endptr != '\0' || flag->tdelay_value <= 0){
                    fprintf(stderr, "Invalid argument for --tdelay.\n");
                    exit(1);
                }
                break;
            default:
                break;
        } 
    }
    
    //Check number of positional arguments
    //Prioritize flags over positional arguments
    switch(argc - optind){
        case 2:
            if(!flag->has_tdelay_flag){
                flag->tdelay_value = strtol(argv[optind + 1], NULL, 10);
            }
        case 1:
            if(!flag->has_samples_flag){
                flag->samples_value = strtol(argv[optind], NULL, 10);
            }
            break;
        case 0:
            break;
        default:
            fprintf(stderr, "More than 2 positional arguments: ");
            for(int i = optind + 2; i<argc; i++){
                fprintf(stderr, "%s ", argv[i]);
            }
            fprintf(stderr, "\n");
            exit(1);
            break;
    }

    return;
}