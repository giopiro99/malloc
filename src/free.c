#include "../includes/malloc_internal.h"

void    ft_free(void *ptr){
    if (ptr == NULL) return;

    t_zone  *assigned_area = find_zone_by_ptr(ptr);
    if (assigned_area == NULL){
        return ;
    }

    if (assigned_area->zone_area == LARGE){
        try_free_large_ptr(assigned_area, ptr);
    }
    else{
        try_free_generic_memory(assigned_area, ptr);
    }
    return ;
}