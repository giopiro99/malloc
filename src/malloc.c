#include "../includes/malloc_internal.h"
#include <stdio.h>
#include <sys/mman.h>
#include <stddef.h>
#include <unistd.h>


g_malloc_zones  malloc_zones;

__attribute__((constructor)) void init_fundamental_stuffs(){
    malloc_zones.tiny_zones = NULL;
    malloc_zones.small_zones = NULL;
    malloc_zones.large_zones = NULL;
    malloc_zones.first_tiny_block_free = NULL;
    malloc_zones.first_small_block_free = NULL;
    malloc_zones.system_pages_size = sysconf(_SC_PAGESIZE);

    size_t  RAW_TINY_SIZE = sizeof(t_zone) + (100 * (LIMIT_TINY_ZONE_AREA + sizeof(t_block)));
    size_t  RAW_SMALL_SIZE = sizeof(t_zone) + (100 * (LIMIT_SMALL_ZONE_AREA + sizeof(t_block)));

    malloc_zones.tiny_mmap_pages_size = ALIGN_PAGES(RAW_TINY_SIZE, malloc_zones.system_pages_size);
    malloc_zones.small_mmap_pages_size = ALIGN_PAGES(RAW_SMALL_SIZE, malloc_zones.system_pages_size);

}

void    *ft_malloc(size_t size){
    
    if (size == 0)
        return (NULL);

    void    *final_ptr = NULL;

    size_t  block_size = ALIGN_16(size + sizeof(t_block));
    if (is_large_area(size)){
        final_ptr = alloc_large_area(block_size);
    }

    if (final_ptr){
        return (final_ptr);
    }

    if (is_tiny_area(size)){
        final_ptr = request_generic_memory(&malloc_zones.tiny_zones,  &malloc_zones.first_tiny_block_free, block_size, TINY);
    }
    else if (is_small_area(size)){
        final_ptr = request_generic_memory( &malloc_zones.small_zones, &malloc_zones.first_small_block_free, block_size, SMALL);
    }

    return (final_ptr);
}