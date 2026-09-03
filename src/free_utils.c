#include "../includes/malloc_internal.h"
#include <errno.h>
#include <sys/mman.h>

void    return_memory_to_kernel(t_zone *assigned_zone){
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

void    delete_zone_from_list(t_zone **head_zone, t_zone *zone_to_delete){
    t_zone  *current_zone = (*head_zone);
    t_zone  *prev_zone = NULL;

    while (current_zone != NULL && current_zone != zone_to_delete){
        prev_zone = current_zone;
        current_zone = current_zone->next_zone;
    }

    if (!current_zone){
        return ;
    }

    if (prev_zone){
        prev_zone->next_zone = current_zone->next_zone;
    }
    else{
        (*head_zone) = current_zone->next_zone;
    }
}

t_zone  *find_zone_by_ptr(void *ptr){

    t_zone  *large_zone = malloc_zones.large_zones;
    t_zone  *small_zone = malloc_zones.small_zones;
    t_zone  *tiny_zone = malloc_zones.tiny_zones;

    while (large_zone != NULL){
        void    *start_zone = (void *)large_zone;
        void    *end_zone   = (void *)((char *)large_zone + large_zone->zone_size);
        if (ptr > start_zone && ptr < end_zone){
            return (large_zone);
        }
        large_zone = large_zone->next_zone;
    }

    while (small_zone != NULL){
        void    *start_zone = (void *)small_zone;
        void    *end_zone   = (void *)((char *)small_zone + small_zone->zone_size);
        if (ptr > start_zone && ptr < end_zone){
            return (small_zone);
        }
        small_zone = small_zone->next_zone;
    }

    while (tiny_zone != NULL){
        void    *start_zone = (void *)tiny_zone;
        void    *end_zone   = (void *)((char *)tiny_zone + tiny_zone->zone_size);
        if (ptr > start_zone && ptr < end_zone){
            return (tiny_zone);
        }
        tiny_zone = tiny_zone->next_zone;
    }

    return (NULL);
}
