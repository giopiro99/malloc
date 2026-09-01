#include "../includes/malloc_internal.h"
#include <sys/mman.h>
#include <errno.h>

void    *alloc_large_area(size_t block_size){

    size_t total_required = block_size + sizeof(t_zone);
    size_t aligned_page_size = ALIGN_PAGES(total_required, malloc_zones.system_pages_size);

    #ifdef DEBUG
            print_str("pagina large allocata, dimensione=");
            print_nbr(aligned_page_size);
            print_str("\n");
            print_str("blocco richiesto=");
            print_nbr(block_size);
            print_str("\n");
    #endif
    void *ptr = mmap(NULL, aligned_page_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (ptr == MAP_FAILED){
        #ifdef DEBUG
            print_str("DEBUG: mmap fallita dentro alloc_large_area\n");
            perror("mmap");
        #endif
        errno = ENOMEM;
        return (NULL);
    }

    if (malloc_zones.large_zones == NULL){
        #ifdef DEBUG
            print_str("prima large zone allocata, indirizzo=");
            print_hex_address(ptr);
            print_str("\n");
        #endif
        malloc_zones.large_zones = ptr;
        malloc_zones.large_zones->next_zone = NULL;
        malloc_zones.large_zones->zone_area = LARGE;
        malloc_zones.large_zones->zone_size = aligned_page_size;
        print_str("size=");
        print_nbr(aligned_page_size);
        print_str("\n");
    }else{
        #ifdef DEBUG
            print_str("large zones gia' esistenti, ne aggiungo una nuova, indirizzo=");
            print_hex_address(ptr);
            print_str("\n");
        #endif
        t_zone  *new_zone = ptr;
        t_zone  *old_first_zone = malloc_zones.large_zones;
        new_zone->next_zone = old_first_zone;
        malloc_zones.large_zones = new_zone;
        new_zone->zone_area = LARGE;
        new_zone->zone_size = aligned_page_size;
        print_str("size=");
        print_nbr(aligned_page_size);
        print_str("\n");
    }

    // castando a char, facciamo in modo che si sposti esattamente di sizeof(byte che vogliamo saltare) perche' char = 1 byte
    //senza questo farebbe una moltiplicazione * t_zone
    t_block *new_large_block = (t_block *)((char *)ptr + sizeof(t_zone));
    new_large_block->size = block_size;
    print_str("size del blocco large=");
    print_nbr(get_size(new_large_block->size));
    print_str("\n");
    set_allocated(new_large_block->size);
    set_zone(new_large_block->size, LARGE);

    ptr = new_large_block->payload.data;
    //possiamo ritornare data, perche' grazie all union, data sara' il primo blocco disponibile dopo i metadati
    return (ptr);
}