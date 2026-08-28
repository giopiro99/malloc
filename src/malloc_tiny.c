#include "../includes/malloc_internal.h"
#include <stdio.h>
#include <sys/mman.h>
#include <errno.h>

static inline void  add_new_free_block(t_block  *new_free_block){
    if (malloc_zones.first_tiny_block_free == NULL){
        malloc_zones.first_tiny_block_free = new_free_block;
    }else{
        t_block *old_first = malloc_zones.first_tiny_block_free;
        
        new_free_block->payload.free_pointers.next = old_first;
        old_first->payload.free_pointers.back = new_free_block;
        malloc_zones.first_tiny_block_free = new_free_block;
    }
}

static inline void  add_new_tiny_zone(void *final_ptr){

    if (malloc_zones.tiny_zones == NULL){
        malloc_zones.tiny_zones = final_ptr;
    }else{
        t_zone  *current_zone = malloc_zones.tiny_zones;
        while(current_zone->next_zone != NULL){
            current_zone = current_zone->next_zone;
        }

        current_zone->next_zone = final_ptr;
    }
}

static inline void  initialize_new_free_block(t_block *new_free_block, size_t block_size){
    new_free_block->prev_size = block_size;
    new_free_block->size = malloc_zones.tiny_mmap_pages_size - block_size - sizeof(t_zone);
    new_free_block->payload.free_pointers.back = NULL;
    new_free_block->payload.free_pointers.next = NULL;
    set_free(new_free_block->size);
    set_zone(new_free_block->size, TINY);
}

static inline void  initialize_new_zone(t_zone *new_zone){
    new_zone->zone_size = malloc_zones.tiny_mmap_pages_size;
    new_zone->next_zone = NULL;
    new_zone->zone_area = TINY;
}

static inline void  initialize_new_tiny_block(t_block *new_tiny_block, size_t block_size){
    new_tiny_block->size = block_size;
    set_allocated(new_tiny_block->size);
    set_zone(new_tiny_block->size, TINY);
}

static inline void  update_free_list(t_block *free_block){
    t_block *prev_block = free_block->payload.free_pointers.back;
    t_block *next_block = free_block->payload.free_pointers.next;

    // se ho un prev, vuol dire che non sono il primo, quindi aggiorno il next del prev(puo' essere anche null se ero l ultimo)
    if (prev_block){
        prev_block->payload.free_pointers.next = next_block;
    }else{
        //se non ho un prev sono il primo della lista quindi aggiorno la variabile globale(puo' essere null se non ci sono blocchi liberi)
        malloc_zones.first_tiny_block_free = next_block;
    }

    if (next_block){
        //se ho un next, vuol dire che devo aggiornare il back del next(che ero io)
        next_block->payload.free_pointers.back = prev_block;
    }
    //se non ho un next, ero l ultimo non devo fare niente
}

static inline void   *allocate_new_tiny_page(size_t block_size){
    void* final_ptr = NULL;

    //vuol dire che e' il primo blocco tiny in assoluto ad essere allocato devo prima allocare la pagina in memoria e poi salvare i dati, come in large
    final_ptr = mmap(NULL, malloc_zones.tiny_mmap_pages_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (final_ptr == MAP_FAILED){
        #ifdef DEBUG
            printf("errore in mmap per tiny zone");
        #endif
        perror("mmap");
        errno = EINVAL;
        return (NULL);
    }

    t_zone  *new_zone = ((t_zone *)final_ptr);
    initialize_new_zone(new_zone);

    t_block *new_tiny_block = (t_block *)((char *)final_ptr + sizeof(t_zone));
    initialize_new_tiny_block(new_tiny_block, block_size);
    
    t_block *new_free_block = (t_block *)(((char *)new_tiny_block) + block_size);
    initialize_new_free_block(new_free_block, block_size);

    add_new_tiny_zone(final_ptr);
    add_new_free_block(new_free_block);

    // incremento il numero di blocchi allocati
    new_zone->allocated_blocks++;
    final_ptr = new_tiny_block->payload.data;
    return (final_ptr);
}

static inline void  *handle_perfect_fit(t_block *free_block){
    void    *final_ptr = NULL;
    set_allocated(free_block->size);
    set_zone(free_block->size, TINY);
    update_free_list(free_block);
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
    // devo splittare i due blocchi e salvare il blocco libero restante
    update_free_list(free_block);

    t_block *new_free_block = (t_block *)((char *)free_block + block_size);
    size_t  new_size = (get_size(free_block->size) - block_size);
    initialize_splitted_block(new_free_block, new_size, block_size);
    add_new_free_block(new_free_block);
}

static t_zone   *get_zone_from_block(t_block *block){
    t_zone  *current_zone = malloc_zones.tiny_zones;

    while (current_zone){
        //siccome quando chiediamo al kernel della memoria, non ci assicura di darci zone adiacenti,
        //non possiamo semplicemente usare current_zone->next come limite.
        //dobbiamo calcolare la fine della zona e chiedere se il nostro indirizzo cade all interno di questi confini
        void    *zone_end = ((char *)current_zone + current_zone->zone_size);
        if ((void *)block  > (void *)current_zone && (void *)block < zone_end)
            return (current_zone);
        current_zone = current_zone->next_zone;
    }
    return (NULL);
}

void    *request_tiny_memory(size_t block_size){

    void    *final_ptr = NULL;
    t_block *free_block = malloc_zones.first_tiny_block_free;
    t_zone  *assigned_zone = NULL;

    while(free_block != NULL && final_ptr == NULL){
        size_t  current_block_size = get_size(free_block->size);
        if(current_block_size > block_size){
            if (current_block_size - block_size <= sizeof(t_block)){
                //il blocco e' troppo piccolo per splittarlo, lo "regalo" all utente
                final_ptr = handle_perfect_fit(free_block);
            }else{
                //splitto il blocco in 2 parti, quello che serve all utente e quello che rimarra' libero per il futuro
                split_block(free_block, block_size);
                free_block->size = block_size;
                set_allocated(free_block->size);
                set_zone(free_block->size, TINY);
                final_ptr = free_block->payload.data;
            }
        }else if(current_block_size == block_size){
            // fit perfetto, final ptr diventa il blocco libero
            final_ptr = handle_perfect_fit(free_block);
        }else{
            free_block = free_block->payload.free_pointers.next;
        }
    }

    if (final_ptr == NULL){
        final_ptr = allocate_new_tiny_page(block_size);
    }

    if (free_block){
        assigned_zone = get_zone_from_block(free_block);
        if (assigned_zone){
            assigned_zone->allocated_blocks++;
        }
    }

    return (final_ptr);
}