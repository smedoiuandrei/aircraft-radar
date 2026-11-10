#include "util.h"

float util_clampf(float value, float min, float max){
    
    if(min > max){
        float temp = min;
        min = max;
        max = temp;
    }
    
    if(value < min){
        return min;
    }
    else if(value > max){
        return max;
    }
    else{
        return value;
    }   
}