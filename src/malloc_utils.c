#include "../includes/malloc_internal.h"

void    initialize_block(t_block *new_block, size_t prev_size, size_t block_size, ZONE_AREA area){
    new_block->size = block_size;
    new_block->prev_size = prev_size;
    set_allocated(new_block->size);
    set_zone(new_block->size, area);
}

void    initialize_free_block(t_block *new_free_block, size_t prev_size, size_t free_block_size, ZONE_AREA area){
    new_free_block->prev_size = prev_size;
    new_free_block->size = free_block_size;
    set_free(new_free_block->size);
    set_zone(new_free_block->size, area);
}

void    add_new_free_block(t_block **first_free_block, t_block *new_free_block){    
    if ((*first_free_block) == NULL){
        (*first_free_block) = new_free_block;
        (*first_free_block)->payload.free_pointers.next = NULL;
        (*first_free_block)->payload.free_pointers.back = NULL;
    }
    else{
        t_block *old_first = (*first_free_block);
        old_first->payload.free_pointers.back = new_free_block;
        new_free_block->payload.free_pointers.next = old_first;
        new_free_block->payload.free_pointers.back = NULL;
        (*first_free_block) = new_free_block;
    }
}

static inline void    add_new_zone(t_zone **head_zone, t_zone *new_zone){
    if (*head_zone == NULL)
    {
        (*head_zone) = new_zone;
    }else {
        t_zone *old_first = (*head_zone);
        new_zone->next_zone = old_first;
        (*head_zone) = new_zone;
    }
}

void    initialize_zone(t_zone **head_zone, t_zone *new_zone, ZONE_AREA area, size_t zone_size){
    add_new_zone(head_zone, new_zone);
    new_zone->zone_area = area;
    new_zone->zone_size = zone_size;
}
