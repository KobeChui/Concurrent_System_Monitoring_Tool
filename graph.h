#ifndef GRAPH_H
#define GRAPH_H

void print_y_axis(int start, int y_axis_length, int offset);
    ///_|> descry: Prints the y-axis of the graph at a given horizontal offset.
    ///_|> start: The row number where the y-axis should start to be printed, type: int
    ///_|> y_axis_length: The length of y-axis, type: int
    ///_|> offset: The horizontal required from the edge of the terminal, type: int
    ///_|> returning: This function does not return anything

void print_x_axis(int y_start, int x_axis_length, int offset);
    ///_|> descry: Prints the x-axis of the graph at a given horizontal offset.
    ///_|> y_start: The row number where the x-axis should start to be printed, type: int
    ///_|> x_axis_length: The length of x-axis, type: int
    ///_|> offset: The horizontal required from the edge of the terminal, type: int
    ///_|> returning: This function does not return anything

void print_memory_graph(long int total_memory, int row, int y_axis_length, int samples_value);
    ///_|> descry: Prints the memory graph including axes, maximum and minimum labels.
    ///_|> total_memory: The maximum memory to be graphed, type: int
    ///_|> row: The number of vertical offset from the top of the terminal for the graph to be printed, type: int
    ///_|> y_axis_length: The length of y-axis, type: int
    ///_|> samples_value: The sampling size, type: int
    ///_|> returning: This function does not return anything

void print_cpu_graph(int row, int y_axis_length, int samples_value);
    ///_|> descry: Prints the cpu graph including axes, maximum and minimum labels.
    ///_|> row: The number of vertical offset from the top of the terminal for the graph to be printed, type: int
    ///_|> y_axis_length: The length of y-axis, type: int
    ///_|> samples_value: The sampling size, type: int
    ///_|> returning: This function does not return anything

void print_core_diagram(int row, int col);
    ///_|> descry: Prints the core diagram in the most compact way.
    ///_|> row: The number of cores to be drawn vertically, type: int
    ///_|> col: The number of cores to be drawn horizontally, type: int
    ///_|> returning: This function does not return anything

void plot_on_graph(long double total, long double value, int row, char* unit, int y_axis_length, int , char* caption);
    ///_|> descry: Prints the core diagram in the most compact way.
    ///_|> total: The maximum number on the graph, type: long double
    ///_|> value: The number to be displayed on plotted relative to the graph, type: long double
    ///_|> row: The number of vertical offset from the top of the terminal for the graph to be printed, type: int
    ///_|> unit: The unit of value to be displayed on the graph, type: char*
    ///_|> y_axis_length: The length of y-axis, type: int
    ///_|> sample_num: The number of samples to be plotted, type: int
    ///_|> caption: The string that indicates what the value represent in the graph, type: char*
    ///_|> returning: This function does not return anything

int graphing_offset(int max_label, char* unit_label, int sample_num);
    ///_|> descry: Calculates horizontal offset for plotting.
    ///_|> max_label: The number of cores to be drawn vertically, type: int
    ///_|> unit_label: The number of cores to be drawn horizontally, type: char*
    ///_|> sample_num: The index of current sample, type: int
    ///_|> returning: This function returns the value of horizontal offset required for plotting on graphs.

#endif