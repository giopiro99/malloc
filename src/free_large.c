#include "../includes/malloc_internal.h"
#include <stdio.h>
#include <sys/mman.h>
#include <errno.h>

void    try_free_large_ptr(t_zone *assigned_zone, void* ptr){
    delete_zone_from_list(&malloc_zones.large_zones, assigned_zone);
    t_block *assigned_block = (t_block *)((char *)assigned_zone + sizeof(t_zone));
    if (assigned_block->payload.data == ptr){
        int status_returned = munmap(assigned_zone, assigned_zone->zone_size);
        if (status_returned == -1){
            perror("munmap");
            errno = EINVAL;
            #ifdef DEBUG
                print_str("errore in munmap per zona=");
                print_hex_address(assigned_zone);
            #endif
        }
    }
    else{
        #ifdef DEBUG
            print_str("puntatore a payload errato indirizzo ptr=");
            print_hex_address(ptr);
            print_str("indirizzo payload=");
            print_hex_address(assigned_block->payload.data);
        #endif
    }
}
