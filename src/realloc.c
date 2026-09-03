#include <stddef.h>
#include <string.h>
#include "../includes/malloc_internal.h"
#include "../includes/malloc.h"

static inline bool  is_edge_case(void **ptr, size_t size){
    if (ptr != NULL && size == 0){
        #ifdef DEBUG
            print_str("realloc chiamata con ptr valido e size == 0, LIBERO ptr");
        #endif
        ft_free(*ptr);
        *ptr = NULL;
        return (true);
    }

    if (ptr == NULL){
        #ifdef DEBUG
            print_str("realloc chiamata con ptr NULL alloco SIZE");
        #endif
        *ptr = ft_malloc(size);
        return (true);
    }
    return (false);
}

static inline void *handle_base_case(void **old_ptr, size_t size, size_t size_to_copy){
    void *final_ptr = ft_malloc(size);
    if (!final_ptr){
        return (NULL);
    }

    memcpy(final_ptr, *old_ptr, size_to_copy);
    ft_free(*old_ptr);
    return (final_ptr);
}

static inline void  merge_next_block(t_block **first_free_block, t_block *assigned_block, t_block *free_block, 
    void *end_zone, size_t new_size){

    t_block     *next_block = (t_block *)((char *)free_block + get_size(free_block->size));
    if ((void *)next_block < end_zone){
        next_block->prev_size = new_size;
    }

    remove_block_from_free_list(first_free_block, free_block);
    assigned_block->size = new_size;
}

static inline bool  try_expand(t_zone *assigned_zone, t_block *assigned_block, size_t size){
    if (assigned_zone->zone_area == LARGE) {
        return (false);
    }

    size_t  required_size = ALIGN_16(size + sizeof(t_block));
    size_t  assigned_block_size = get_size(assigned_block->size);
    t_block *next_block = (t_block *)((char *)assigned_block + assigned_block_size);
    void    *end_zone = (void *)((char *)assigned_zone + assigned_zone->zone_size);
    ZONE_AREA   area = assigned_zone->zone_area == TINY ? TINY : SMALL;

    if ((void *)next_block < end_zone && is_free(next_block->size)){
        size_t  next_block_size = get_size(next_block->size);
        size_t  sum_size = assigned_block_size + next_block_size;
        t_block **first_free_block = assigned_zone->zone_area == TINY ? &malloc_zones.first_tiny_block_free : &malloc_zones.first_small_block_free;
        if (sum_size >= required_size){
            if (sum_size - required_size > sizeof(t_block)){
                return (true);
            }
            else{
                merge_next_block(first_free_block, assigned_block, next_block, end_zone, sum_size);
                set_allocated(assigned_block->size);
                set_zone(assigned_block->size, area);
                return (true);
            }
        }
    }
    return (false);
}

void    *ft_realloc(void *ptr, size_t size){
    
    if (is_edge_case(&ptr, size)){
        return (ptr);
    }

    t_zone  *assigned_zone = find_zone_by_ptr(ptr);
    if (assigned_zone == NULL){
        return (NULL);
    }

    t_block *assigned_block = find_block_by_ptr(assigned_zone, ptr);
    if (assigned_block == NULL){
        return (NULL);
    }

    size_t  old_user_size = get_size(assigned_block->size) - sizeof(t_block);
    if (size <= old_user_size){
        return ptr;
    }

    void    *final_ptr = NULL;

    if (try_expand(assigned_zone, assigned_block, size)){
        final_ptr = ptr;
    }
    else{
        final_ptr = handle_base_case(&ptr, size, old_user_size);
    }

    return (final_ptr);
}