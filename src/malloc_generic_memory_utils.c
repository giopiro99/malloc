#include "../includes/malloc_internal.h"

void    remove_block_from_free_list(t_block **first_free_block, t_block *block){
    t_block *prev_free_block = block->payload.free_pointers.back;
    t_block *next_free_block = block->payload.free_pointers.next;

    if (prev_free_block){
        prev_free_block->payload.free_pointers.next = next_free_block;
    }
    else{
        (*first_free_block) = next_free_block;
    }

    if(next_free_block){
        next_free_block->payload.free_pointers.back = prev_free_block;
    }
}

t_zone  *get_zone_from_block(t_zone **head_zone, t_block *block){
    t_zone  *current_zone = (*head_zone);

    while (current_zone != NULL)
    {
        //siccome quando chiediamo al kernel della memoria, non ci assicura di darci zone adiacenti,
        //non possiamo semplicemente usare current_zone->next come limite.
        //dobbiamo calcolare la fine della zona e chiedere se il nostro indirizzo cade all interno di questi confini
        void    *end_zone = ((char *)current_zone + current_zone->zone_size);
        if ((void *)block > (void *)current_zone && (void *)block < end_zone){
            return (current_zone);
        }

        current_zone = current_zone->next_zone;
    }
    return (NULL);
}

void    *handle_perfect_fit(t_block **first_free_block, t_block *block){
    void    *final_ptr = NULL;

    remove_block_from_free_list(first_free_block, block);

    final_ptr = block->payload.data;
    set_allocated(block->size);
    return (final_ptr);
}

static inline void  try_to_update_prev_size(t_block *block, t_zone *assigned_zone){
    void    *end_zone = (void *)((char *)assigned_zone + assigned_zone->zone_size);
    size_t  block_size = get_size(block->size);

    t_block *next_block = (t_block *)((char *)block + block_size);
    if ((void *)next_block >= end_zone){
        return ;
    }

    next_block->prev_size = block_size;
}

void    split_block(t_block **first_free_block, t_block *free_block, t_zone *assigned_zone, size_t block_size){

    remove_block_from_free_list(first_free_block, free_block);

    size_t  free_block_size = get_size(free_block->size) - block_size;
    t_block *new_free_block = (t_block *)((char *)free_block + block_size);
    ZONE_AREA   area = get_zone(free_block->size);

    initialize_free_block(new_free_block, block_size, free_block_size, area);
    add_new_free_block(first_free_block, new_free_block);

    //devo aggiornare il prev size del blocco successivo a quello che e' diventato libero
    try_to_update_prev_size(new_free_block, assigned_zone);
}