#include "../includes/malloc.h"

void    *request_small_memory(size_t size){

    size_t  block_size = ALIGN_16(size + sizeof(t_block));
    (void)block_size;
    return (NULL);
}