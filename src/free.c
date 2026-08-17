#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include "../includes/malloc.h"

void    dealloc_tiny_ptr(void *ptr){
    (void)ptr;
}

void    dealloc_small_ptr(void *ptr){
    (void)ptr;
}

void    dealloc_large_ptr(void *ptr){
    (void)ptr;
}

bool   try_free_tiny_ptr(void* ptr){
    (void)ptr;
    return (false);
}

bool    try_free_small_ptr(void* ptr){
    (void)ptr;
    return (false);
}

bool    try_free_large_ptr(void* ptr){
    t_zone  *current_zone = malloc_zones.large_zones;
    t_zone  *prev_zone = NULL;
    t_block *current_block = NULL;
    while (current_zone != NULL){
        current_block = (t_block*)((char *)(current_zone) + sizeof(t_zone));
        if (current_block && current_block->payload.data == ptr){

            #ifdef DEBUG
                printf("%s", "blocco large da eliminare trovato!\n");
            #endif

            if (current_zone->next_zone == NULL){
                int status_returned = munmap(current_zone, current_zone->zone_size);
                if (status_returned == -1){
                    #ifdef DEBUG
                        printf("%s", "errore in munmmap\n");
                        perror("munmapp errno=");
                    #endif
                    errno = EINVAL;
                    return (false);
                }
                printf("%s%d\n", "eliminata pagina large! Status=", status_returned);
                return (true);           
            }else{
                if (prev_zone){
                    prev_zone->next_zone = current_zone->next_zone;
                }else{
                    malloc_zones.large_zones = current_zone->next_zone;
                }
                int status_returned = munmap(current_zone, current_zone->zone_size);
                if (status_returned == -1){
                    #ifdef DEBUG
                        printf("%s", "errore in munmmap\n");
                        perror("munmapp errno=");
                    #endif
                    errno = EINVAL;
                    return (false);
                }
                printf("%s%d\n", "eliminata pagina large! Status=", status_returned);
                return (true);
            }
        }
        prev_zone = current_zone;
        current_zone = current_zone->next_zone;
    }
    return (false);
}

void    ft_free(void *ptr){
    if (ptr == NULL) return;

    if (try_free_tiny_ptr(ptr)){
        return ;
    }
    if (try_free_small_ptr(ptr)){
        return ;
    }
    if (try_free_large_ptr(ptr)){
        return ;
    }
    return ;
}