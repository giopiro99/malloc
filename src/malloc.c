#include <sys/mman.h>
#include <stddef.h>
#include <unistd.h>
#include "../includes/malloc.h"

g_malloc_zones  malloc_zones;

__attribute__((constructor)) void init_fundamental_stuffs(){
    malloc_zones.tiny_zones = NULL;
    malloc_zones.small_zones = NULL;
    malloc_zones.large_zones= NULL;
    malloc_zones.pages_size = sysconf(_SC_PAGESIZE);
}

void    *searc_for_prealloc_zone(){
    return (NULL);
}

void    *alloc_large_area(size_t real_size){
    return (NULL);
}

void    *request_tiny_memory(size_t real_size){
    return (NULL);
}

void    *request_small_memory(size_t real_size){
    return (NULL);
}

void    *ft_malloc(size_t size){
    
    if (size == 0)
        return (NULL);

    void    *final_ptr = NULL;
    size_t  real_size = ALIGN_16(size + sizeof(t_block));
    if (is_large_area(real_size)){
        final_ptr = alloc_large_area(real_size);
    }

    if (final_ptr){
        return (final_ptr);
    }

    if (is_tiny_area(real_size)){
        final_ptr = request_tiny_memory(real_size);
    }
    else if (is_small_area(real_size)){
        final_ptr = request_small_memory(real_size);
    }

    return (final_ptr);
}