#include <stdio.h>
#include <sys/mman.h>
#include <stddef.h>
#include <unistd.h>
#include "../includes/malloc.h"

g_malloc_zones  malloc_zones;

__attribute__((constructor)) void init_fundamental_stuffs(){
    malloc_zones.tiny_zones = NULL;
    malloc_zones.small_zones = NULL;
    malloc_zones.large_zones= NULL;
    malloc_zones.system_pages_size = sysconf(_SC_PAGESIZE);
}

void    *ft_malloc(size_t size){
    
    if (size == 0)
        return (NULL);

    void    *final_ptr = NULL;

    if (is_large_area(size)){
        final_ptr = alloc_large_area(size);
    }

    if (final_ptr){
        return (final_ptr);
    }

    if (is_tiny_area(size)){
        final_ptr = request_tiny_memory(size);
    }
    else if (is_small_area(size)){
        final_ptr = request_small_memory(size);
    }

    return (final_ptr);
}