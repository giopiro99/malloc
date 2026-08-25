#include "../includes/malloc.h"
#include <stdio.h>
#include <sys/mman.h>
#include <errno.h>

void    *request_tiny_memory(size_t size){

    size_t  block_size = ALIGN_16(size + sizeof(t_block));
    void    *final_ptr = NULL;
    t_block *new_tiny_block = NULL;

    if (malloc_zones.tiny_zones == NULL){
        size_t  total_required = ALIGN_PAGES((block_size + sizeof(t_zone)), malloc_zones.system_pages_size);
        //vuol dire che e' il primo blocco tiny in assoluto ad essere allocato devo prima allocare la pagina in memoria e poi salvare i dati, come in large
        final_ptr = mmap(NULL, total_required, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
        if (final_ptr == MAP_FAILED){
            #ifdef DEBUG
                printf("errore in mmap per tiny zone");
            #endif
            perror("mmap");
            errno = EINVAL;
            return (NULL);
        }
        t_zone  *new_zone = ((t_zone *)final_ptr);
        new_zone->zone_size = total_required;
        new_zone->next_zone = NULL;
        new_zone->zone_area = TINY;
        new_zone->allocated_blocks++;

        new_tiny_block = (t_block *)((char *)final_ptr + sizeof(t_zone));
        new_tiny_block->size = block_size;
        set_allocated(new_tiny_block->size);
        set_zone(new_tiny_block->size, TINY);

        final_ptr = new_tiny_block->payload.data;
        return (final_ptr);
    }

    t_zone  *tiny_zones = malloc_zones.tiny_zones;
    bool    is_first_of_zone = true;
    while(tiny_zones != NULL){
        t_block *current_block = NULL;
        if (is_first_of_zone){
            current_block = (t_block *)((char *)tiny_zones + sizeof(t_zone));
            
        }
        bool    zone_ended = false;
        while (!zone_ended){

        }

        size_t  current_size = get_size(current_block->size);
        tiny_zones = tiny_zones->next_zone;
    }


    // se arriviamo qua, vuol dire che ci sono gia' blocchi tiny allocati, io devo iterare su ogni nodo e controllare se 
    // c e' abbastanza spazio per il nuovo blocco;
    return (final_ptr);
}