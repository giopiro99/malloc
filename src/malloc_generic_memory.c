#include "../includes/malloc_internal.h"
#include <errno.h>
#include <sys/mman.h>

void    *request_new_page(t_zone **head_zone, t_block **first_free_block, size_t block_size, ZONE_AREA area){
    
    #ifdef DEBUG
        print_str("richiesta di una nuova pagina di memoria in corso\n");
    #endif

    void    *final_ptr = NULL;
    size_t  zone_size = 0;

    if (area == TINY){
        zone_size = malloc_zones.tiny_mmap_pages_size;
        final_ptr = mmap(NULL, zone_size, PROT_READ | PROT_WRITE,  MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    }else{
        zone_size = malloc_zones.small_mmap_pages_size;
        final_ptr = mmap(NULL, zone_size, PROT_READ | PROT_WRITE,  MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    }

    if (final_ptr == MAP_FAILED){
        #ifdef DEBUG
            print_str("errore in mmap per zona tiny/small:\nblocco richiesto=");
            print_nbr(block_size);
        #endif
        errno = ENOMEM;
        perror("mmap");
        return (NULL);
    }

    t_zone  *new_zone = (t_zone *)final_ptr;
    initialize_zone(head_zone, new_zone, area, zone_size);

    t_block *new_block = (t_block *)((char *)new_zone + sizeof(t_zone));
    initialize_block(new_block, block_size, area);

    t_block *new_free_block = (t_block *)((char *)new_block + block_size);
    size_t  new_free_block_size = zone_size - block_size - sizeof(t_zone);
    initialize_free_block(new_free_block, block_size, new_free_block_size, area);
    add_new_free_block(first_free_block, new_free_block);

    new_zone->allocated_blocks++;
    final_ptr = new_block->payload.data;

    return final_ptr;
}

void    *request_generic_memory(t_zone **head_zone, t_block **first_free_block, size_t block_size,  ZONE_AREA area){
    
    void    *final_ptr = NULL;
    t_block *free_block = (*first_free_block);
    t_zone  *assigned_zone = NULL;
    size_t  current_size = 0;

    while (free_block != NULL && final_ptr == NULL){
        current_size = get_size(free_block->size);
        if (current_size > block_size){
            if (current_size - block_size <= sizeof(t_block)){
                final_ptr = handle_perfect_fit(first_free_block, free_block);
            }
            else{
                split_block(first_free_block, free_block, block_size);
                free_block->size = block_size;
                set_allocated(free_block->size);
                set_zone(free_block->size, area);
                final_ptr = free_block->payload.data;
            }
        }
        else if (current_size == block_size){
            final_ptr = handle_perfect_fit(first_free_block, free_block);
        }
        else{
            free_block = free_block->payload.free_pointers.next;
        }
    }

    if (final_ptr == NULL){
        final_ptr = request_new_page(head_zone, first_free_block, block_size, area);
    }

    if (free_block){
        assigned_zone = get_zone_from_block(head_zone, free_block);
        if(assigned_zone){
            assigned_zone->allocated_blocks++;
        }
    }

    return (final_ptr);
}