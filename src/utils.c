#include <unistd.h>
#include "../includes/malloc.h"

ZONE_AREA   get_zone_area(size_t size){
    if (size <= LIMIT_TINY_ZONE_AREA)
        return (TINY);
    else if (size <= LIMIT_SMALL_ZONE_AREA)
        return (SMALL);
    return (LARGE);
}

bool    is_tiny_area(size_t size){
    return (size <= LIMIT_TINY_ZONE_AREA ? true : false);
}

bool    is_small_area(size_t size){
    return (size <= LIMIT_SMALL_ZONE_AREA ? true : false);
}

bool    is_small_area(size_t size){
bool    is_large_area(size_t size){
    return (size > LIMIT_SMALL_ZONE_AREA ? true : false);
}