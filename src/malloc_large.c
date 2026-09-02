#include "../includes/malloc_internal.h"
#include <sys/mman.h>
#include <errno.h>

void    *alloc_large_area(size_t block_size){

    size_t total_required = block_size + sizeof(t_zone);
    size_t aligned_page_size = ALIGN_PAGES(total_required, malloc_zones.system_pages_size);

    void *ptr = mmap(NULL, aligned_page_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (ptr == MAP_FAILED){
        #ifdef DEBUG
            print_str("DEBUG: mmap fallita dentro alloc_large_area\n");
        #endif
        perror("mmap");
        errno = ENOMEM;
        return (NULL);
    }

    t_zone  *new_large_zone = (t_zone *)ptr;
    initialize_zone(&malloc_zones.large_zones, new_large_zone, LARGE, aligned_page_size);

    // castando a char, facciamo in modo che si sposti esattamente di sizeof(byte che vogliamo saltare) perche' char = 1 byte
    //senza questo farebbe una moltiplicazione * t_zone
    t_block *new_large_block = (t_block *)((char *)ptr + sizeof(t_zone));
    initialize_block(new_large_block, 0, block_size, LARGE);
    
    new_large_zone->allocated_blocks++;
    ptr = new_large_block->payload.data;
    //possiamo ritornare data, perche' grazie all union, data sara' il primo blocco disponibile dopo i metadati
    return (ptr);
}