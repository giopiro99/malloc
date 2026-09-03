#include "../includes/malloc_internal.h"
#include <sys/mman.h>

t_block    *find_block_by_ptr(t_zone *assigned_zone, void *ptr){
    t_block *current_block = (t_block *)((char *)assigned_zone + sizeof(t_zone));
    void    *start_zone = (void *)assigned_zone;
    void    *end_zone = (void *)((char *)assigned_zone + assigned_zone->zone_size);
    while ((void *)current_block > start_zone && (void *)current_block < end_zone){
        if ((void *)current_block > ptr){
            break ;
        }

        if (current_block->payload.data == ptr){
            return (current_block);
        }
        current_block = (t_block *)((char *)current_block + get_size(current_block->size));
    }

    return (NULL);
}

static inline void  remove_all_free_blocks_and_delete_zone(t_block **first_free_blocks, t_zone *assigned_zone, void *end_zone){
    t_block *current_block = (t_block *)((char *)assigned_zone + sizeof(t_zone));
    t_zone  **head_zone =  assigned_zone->zone_area == TINY ? &malloc_zones.tiny_zones : &malloc_zones.small_zones;

    while ((void *)current_block < end_zone){
        size_t  current_block_size = get_size(current_block->size);
        if (is_free(current_block->size)){
            remove_block_from_free_list(first_free_blocks, current_block);
        }
        current_block = (t_block *)((char *)current_block + current_block_size);
    }
    delete_zone_from_list(head_zone, assigned_zone);
    return_memory_to_kernel(assigned_zone);
    return ;
}

static inline void  try_to_merge_right_block(t_block **first_free_blocks, t_block *assigned_block, void *end_zone){
    t_block *next_block = (t_block *)((char *)assigned_block + get_size(assigned_block->size));
    if ((void *)next_block >= end_zone){
        return ;
    }

    if (!is_free(next_block->size)){
        return ;
    }

    remove_block_from_free_list(first_free_blocks, next_block);
    assigned_block->size = get_size(assigned_block->size) + get_size(next_block->size);
}

static inline void  *try_to_merge_left_block(t_block **first_free_blocks, t_block *assigned_block, void *start_zone){
    t_block *prev_block = (t_block *)((char *)assigned_block - assigned_block->prev_size);

    if ((void *)assigned_block <= start_zone || (void *)prev_block <= start_zone){
        return (assigned_block);
    }

    if (!is_free(prev_block->size)){
        return (assigned_block);
    }

    remove_block_from_free_list(first_free_blocks, prev_block);
    size_t  new_size = get_size(assigned_block->size) + get_size(prev_block->size);
    assigned_block = prev_block;
    assigned_block->size = new_size;
    return assigned_block ;
}

void    try_free_generic_memory(t_zone *assigned_zone, void *ptr){

    t_block **first_free_blocks =  assigned_zone->zone_area == TINY ? &malloc_zones.first_tiny_block_free : &malloc_zones.first_small_block_free;
    t_block *assigned_block = find_block_by_ptr(assigned_zone, ptr);
    t_block *next_block = NULL;
    void    *end_zone = ((char *)assigned_zone + assigned_zone->zone_size);
    
    if (assigned_block == NULL){
        #ifdef DEBUG
            print_str("blocco da eliminare non trovato, indirizzo ptr=");
            print_hex_address(ptr);
            print_str("\n");
            print_str("indirizzo zona=");
            print_hex_address(assigned_zone);
        #endif
        return;
    }

    assigned_zone->allocated_blocks--;
    if (assigned_zone->allocated_blocks == 0){
        remove_all_free_blocks_and_delete_zone(first_free_blocks, assigned_zone, end_zone);
        return ;
    }

    try_to_merge_right_block(first_free_blocks, assigned_block, end_zone);

    assigned_block = try_to_merge_left_block(first_free_blocks, assigned_block, (void *)assigned_zone);

    next_block = (t_block *)((char *)assigned_block + get_size(assigned_block->size));
    if ((void *)next_block < end_zone){
        next_block->prev_size = get_size(assigned_block->size);
    }

    set_free(assigned_block->size);
    set_zone(assigned_block->size, assigned_zone->zone_area);
    add_new_free_block(first_free_blocks, assigned_block);
}