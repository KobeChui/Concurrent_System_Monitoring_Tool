#include <string.h>
#include <math.h>
#include <stdio.h>

#include "graph.h"
#include "utilities.h"

void print_y_axis(int start, int y_axis_length, int offset){

    for(int i=0; i<y_axis_length; i++){
        printf("\033[%d;%dH", start++, offset);
        printf("|\n");
    }
}

void print_x_axis(int y_start, int x_axis_length, int offset){

    for(int i=0; i<=x_axis_length; i++){
        printf("\033[%d;%dH", y_start, offset + i);
        printf("-\n");
    }
}

void print_memory_graph(long int total_memory, int row, int y_axis_length, int samples_value){

    char *min_label = "0 GB ";
    int buffer = graphing_offset(total_memory, "GB", 0);
    printf("\033[%d;1H", row + 1);
    printf("%ld GB", total_memory);
    print_y_axis(row + 1, y_axis_length, buffer);
    printf("\033[%d;%dH", row + 1 + y_axis_length, buffer - (int)strlen(min_label));
    printf("%s\n", min_label);
    print_x_axis(row + 1 + y_axis_length, samples_value, buffer);
    
    return;
}

void print_cpu_graph(int row, int y_axis_length, int samples_value){

    int max = 100;
    int buffer = graphing_offset(max, "%%", 0);
    printf("\033[%d;1H", row + 1);
    printf("%d %%", max);
    print_y_axis(row + 1, y_axis_length, buffer);

    printf("\033[%d;%dH", row + 1 + y_axis_length, buffer - (int)strlen("0 %%"));
    printf("0 %%\n");
    
    print_x_axis(row + 1 + y_axis_length, samples_value, buffer);
    return;
}

void print_core_diagram(int row, int col){

    for(int r = 0; r < row; r++){
        for (int c = 0; c < col; c++){
            printf("+--+ ");
        }
        printf("\n");
        for (int c = 0; c < col; c++){
            printf("|  | ");
        }
        printf("\n");
        for (int c = 0; c < col; c++){
            printf("+--+ ");
        }
        printf("\n");
    }
    return;
}

void plot_on_graph(long double total, long double value, int row, char* unit, int y_axis_length, int sample_num, char* caption){
    
    printf("\033[%d;1H \033[2K\r", row);
    printf("%s ", caption);
    if(strcmp(unit, "%%") == 0){
        printf("%.2Lf %%\n", value);
    }
    else{
        printf("%.2Lf %s\n", value, unit);
    }
    int ratio = (int)ceil(value/(total/y_axis_length));
    int buffer = graphing_offset(total, unit, sample_num);
    printf("\033[%d;%dH", row + y_axis_length + 1 - ratio, buffer);
    printf("#\n");

    return;
}

int graphing_offset(int max_label, char* unit_label, int sample_num){

    //At least 3 including one character for the vertical axis, and two characters of space.
    int number_offset = num_of_digit(max_label);
    int unit_offset;
    if(strstr(unit_label, "%%") != NULL){
        unit_offset = 1;
    }
    else{
        unit_offset = strlen(unit_label);
    }
    int buffer = number_offset + unit_offset + sample_num;
    return buffer + 3;
}