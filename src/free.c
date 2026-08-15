#include <stdlib.h>

void    dealloc_tiny_ptr(void *ptr){
    (void)ptr;
}

void    dealloc_small_ptr(void *ptr){
    (void)ptr;
}

void    dealloc_large_ptr(void *ptr){
    (void)ptr;
}

void*    try_free_tiny_ptr(void* ptr){
    return (NULL);
}

void*    try_free_small_ptr(void* ptr){
    return (NULL);
}

void*    try_free_large_ptr(void* ptr){
    return (NULL);
}

void    free(void *ptr){
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