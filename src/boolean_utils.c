#include <unistd.h>
#include "../includes/malloc_internal.h"

bool    is_tiny_area(size_t size){
    return (size <= LIMIT_TINY_ZONE_AREA ? true : false);
}

bool    is_small_area(size_t size){
    return (size <= LIMIT_SMALL_ZONE_AREA ? true : false);
}

bool    is_large_area(size_t size){
    return (size > LIMIT_SMALL_ZONE_AREA ? true : false);
}