#include "../includes/malloc.h"

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