#include <math.h>
#include "utilities.h"

int num_of_digit(int num){
    if(num == 0) return 1;
    return (int)log10(num) + 1;
}

int get_closest_factor(int num){
    int factor = sqrt(num);
    while(num % factor != 0){
        factor--;
    }
    return factor;
}