#include <errno.h>
#include <stdio.h>
#include <sys/mman.h>
#include <stddef.h>
#include <unistd.h>
#include "../includes/malloc.h"

g_malloc_zones  malloc_zones;

__attribute__((constructor)) void init_fundamental_stuffs(){
    malloc_zones.tiny_zones = NULL;
    malloc_zones.small_zones = NULL;
    malloc_zones.large_zones= NULL;
    malloc_zones.system_pages_size = sysconf(_SC_PAGESIZE);
}

void    *alloc_large_area(size_t real_size){

    size_t total_required = real_size + sizeof(t_zone);
    size_t aligned_page_size = ALIGN_PAGES(total_required, malloc_zones.system_pages_size);
    #ifdef DEBUG
            printf("pagina large allocata, dimensione=%lu\n", aligned_page_size);
            printf("blocco richiesto=%lu\n", real_size);
    #endif
    void *ptr = mmap(NULL, aligned_page_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (ptr == MAP_FAILED){
        #ifdef DEBUG
            perror("DEBUG: mmap fallita dentro alloc_large_area\n");
        #endif
        errno = ENOMEM;
        return (NULL);
    }

    if (malloc_zones.large_zones == NULL){
        #ifdef DEBUG
            printf("prima large zone allocata, indirizzo %p\n", ptr);
        #endif
        malloc_zones.large_zones = ptr;
        malloc_zones.large_zones->next_zone = NULL;
        malloc_zones.large_zones->zone_area = LARGE;
        malloc_zones.large_zones->zone_size = aligned_page_size;
    }else{
        #ifdef DEBUG
            printf("large zones gia' esistenti, ne aggiungo una nuova, indirizzo %p\n", ptr);
        #endif
        t_zone  *new_zone = ptr;
        new_zone->next_zone = malloc_zones.large_zones;
        malloc_zones.large_zones = new_zone;
        new_zone->zone_area = LARGE;
        new_zone->zone_size = aligned_page_size;
    }

    // castando a char, facciamo in modo che si sposti esattamente di sizeof(byte che vogliamo saltare) perche' char = 1 byte
    //senza questo farebbe una moltiplicazione * t_zone
    t_block *new_large_block = (t_block *)((char *)ptr + sizeof(t_zone));
    new_large_block->size = real_size;

    //possiamo ritornare data, perche' grazie all union, data sara' il primo blocco disponibile dopo i metadati
    return (new_large_block->payload.data);
}

void    *request_tiny_memory(size_t real_size){
    (void)(real_size);
    return (NULL);
}

void    *request_small_memory(size_t real_size){
    (void)(real_size);
    return (NULL);
}

void    *ft_malloc(size_t size){
    
    if (size == 0)
        return (NULL);

    void    *final_ptr = NULL;
    size_t  real_size = ALIGN_16(size + sizeof(t_block));
    if (is_large_area(real_size)){
        final_ptr = alloc_large_area(real_size);
    }

    if (final_ptr){
        return (final_ptr);
    }

    if (is_tiny_area(real_size)){
        final_ptr = request_tiny_memory(real_size);
    }
    else if (is_small_area(real_size)){
        final_ptr = request_small_memory(real_size);
    }

    return (final_ptr);
}