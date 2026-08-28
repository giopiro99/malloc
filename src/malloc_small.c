#include "../includes/malloc_internal.h"
#include <errno.h>
#include <stdio.h>
#include <sys/mman.h>

static inline void  initialize_new_zone(t_zone *new_zone){
    new_zone->zone_size = malloc_zones.small_mmap_pages_size;
    new_zone->next_zone = NULL;
    new_zone->zone_area = SMALL;
}   

static inline void  initialize_new_block(t_block *new_block, size_t block_size){
    new_block->size = block_size;
    new_block->prev_size = 0;
    set_allocated(new_block->size);
    set_zone(new_block->size, SMALL);
}

static inline void    initialize_new_free_block(t_block *new_free_block, size_t block_size){
    new_free_block->size = malloc_zones.small_mmap_pages_size - block_size - sizeof(t_zone);
    new_free_block->prev_size = block_size;
    set_free(new_free_block->size);
    set_zone(new_free_block->size, SMALL);
}

static inline void  add_new_free_block(t_block *new_free_block){
    if (malloc_zones.first_small_block_free == NULL){
        malloc_zones.first_small_block_free = new_free_block;
    }else{
        t_block *old_first = malloc_zones.first_small_block_free;
        new_free_block->payload.free_pointers.next = old_first;
        old_first->payload.free_pointers.back = new_free_block;
        malloc_zones.first_small_block_free = new_free_block;
    }
}

static inline void  add_new_small_zone(t_zone *zone){
    if (malloc_zones.small_zones == NULL){
        malloc_zones.small_zones = zone;
    }else{
        t_zone  *current_zone = malloc_zones.small_zones;
        while (current_zone->next_zone != NULL){
            current_zone = current_zone->next_zone;
        }

        current_zone->next_zone = zone;
    }
}

static inline void    *allocate_new_small_page(size_t block_size){

    void    *final_ptr = mmap(NULL, malloc_zones.small_mmap_pages_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (final_ptr == MAP_FAILED){
        #ifdef DEBUG
            printf("mmap fallito in malloc small");
        #endif
        perror("mmap");
        errno = EINVAL;
        return (NULL);
    }

    t_zone  *new_zone = ((t_zone *)final_ptr);
    initialize_new_zone(new_zone);

    t_block *new_block = (t_block *)((char *)new_zone + sizeof(t_zone));
    initialize_new_block(new_block, block_size);

    t_block *new_free_block = (t_block *)((char *)new_block + block_size);
    initialize_new_free_block(new_free_block, block_size);

    add_new_free_block(new_free_block);
    add_new_small_zone(new_zone);

    if (new_block && new_zone){
        new_zone->allocated_blocks++;
    }

    final_ptr = new_block->payload.data;
    return (final_ptr);
}

static inline void  update_free_list(t_block *free_block){
    t_block *prev_free = free_block->payload.free_pointers.back;
    t_block *next_free = free_block->payload.free_pointers.next;

    if(prev_free){
        prev_free->payload.free_pointers.next = next_free;
    }else{
        malloc_zones.first_small_block_free = next_free;
    }

    if (next_free){
        next_free->payload.free_pointers.back = prev_free;
    }
}

static inline void  *handle_perfect_fit(t_block *free_block){
    void    *final_ptr = NULL;
    update_free_list(free_block);
    set_allocated(free_block->size);
    final_ptr = free_block->payload.data;
    return (final_ptr);
}

static inline void  initialize_splitted_block(t_block *new_free_block, size_t new_size, size_t prev_size){
    new_free_block->size = new_size;
    new_free_block->prev_size = prev_size;
    new_free_block->payload.free_pointers.back = NULL;
    new_free_block->payload.free_pointers.next = NULL;
    set_free(new_free_block->size);
    set_zone(new_free_block->size, TINY);
}

static inline void  split_block(t_block *free_block, size_t block_size){
    update_free_list(free_block);
    size_t  new_size = get_size(free_block->size) - block_size;
    t_block *new_free_block = (t_block *)((char *)free_block + block_size);
    
    initialize_splitted_block(new_free_block, new_size, block_size);
    add_new_free_block(new_free_block);
}

static inline t_zone  *get_zone_from_block(t_block *free_block){

    t_zone  *current_zone = malloc_zones.small_zones;
    while (current_zone){
        void    *end_zone = ((char *)current_zone + current_zone->zone_size);
        if ((void *)free_block > (void *)current_zone && (void *)free_block < end_zone)
            return (current_zone);
        current_zone = current_zone->next_zone;
    }

    return (NULL);
}

void    *request_small_memory(size_t block_size){

    void    *final_ptr = NULL;
    t_block *free_block = malloc_zones.first_small_block_free;

    while (free_block != NULL && final_ptr == NULL){
        size_t  current_block_size = free_block->size;
        if (current_block_size > block_size){
            if (current_block_size - block_size <= sizeof(t_block)){
                final_ptr = handle_perfect_fit(free_block);
            }else{
                split_block(free_block, block_size);
                free_block->size = block_size;
                set_allocated(free_block->size);
                set_zone(free_block->size, SMALL);
                final_ptr = free_block->payload.data;
            }
        }else if (current_block_size == block_size){
            final_ptr = handle_perfect_fit(free_block);
        }
        else{
            free_block = free_block->payload.free_pointers.next;
        }
    }

    if (final_ptr == NULL){
        final_ptr = allocate_new_small_page(block_size);
    }

    if (free_block){
        t_zone  *assigned_zone = get_zone_from_block(free_block);
        if (assigned_zone){
            assigned_zone->allocated_blocks++;
        }
    }
    return (final_ptr);
}