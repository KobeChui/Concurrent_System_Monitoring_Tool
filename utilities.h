#ifndef UTIL_H
#define UTIL_H

#include <math.h>

int num_of_digit(int num);
    ///_|> descry: Calculates the number of digit in an integer.
    ///_|> num: The integer to be checked, type: int
    ///_|> returning: This function returns the number of digits in the integer num, type: int

int get_closest_factor(int num);
    ///_|> descry: Calculates the smaller number from the factor pair of a number that are closest to each other.
    ///_|> num: The integer to be checked, type: int
    ///_|> returning: This function returns the smaller number from the factor pair of num that are closest to each other, type: int

#endif