#include "../includes/malloc_internal.h"

bool    try_free_small_ptr(void* ptr){
    (void)ptr;
    return (false);
}