#include "../includes/malloc_internal.h"

t_block    *find_block_by_ptr(t_zone *assigned_area, void *ptr){
    t_block *current_block = (t_block *)((char *)assigned_area + sizeof(t_zone));
    void    *start_zone = (void *)assigned_area;
    void    *end_zone = (void *)((char *)assigned_area + assigned_area->zone_size);
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

void    try_free_generic_memory(t_zone *assigned_area, void *ptr){

    t_block *assigned_block = find_block_by_ptr(assigned_area, ptr);
    if (assigned_block == NULL){
        #ifdef DEBUG
            print_str("blocco da eliminare non trovato, indirizzo ptr=");
            print_hex_address(ptr);
            print_str("\n");
            print_str("indirizzo zona=");
            print_hex_address(assigned_area);
        #endif
        return;
    }

}