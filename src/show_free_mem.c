#include "../includes/malloc_internal.h"

static inline void  print_block(t_block *block, char *label, char *size_label, char *prev_size_label){
    if(block == NULL){
        return ;
    }

    print_str(label);
    print_hex_address(block);
    print_str("\n");
    print_str(size_label);
    print_nbr(get_size(block->size));
    print_str("\n");
    print_str(prev_size_label);
    print_nbr(block->prev_size);
    print_str("\n\n");
}

static inline void  print_generic_free_blocks(t_block *first_free_block, char *label){
    if (first_free_block == NULL){
        return ;
    }

    t_block *current_free_block = first_free_block;
    
    print_str(label);
    while (current_free_block != NULL){
        print_block(current_free_block, "current ptr = ", "current ptr size = ", 
            "current ptr prev-size = ");
        
        print_block(current_free_block->payload.free_pointers.back, "back ptr = ", "back ptr size = ",
            "back ptr prev-size = ");

        print_block(current_free_block->payload.free_pointers.next, "next ptr = ", "next ptr size = ",
            "next ptr prev-size = ");

        current_free_block = current_free_block->payload.free_pointers.next;
    }
}

void    show_free_mem(){
    t_block *free_tiny_blocks = malloc_zones.first_tiny_block_free;
    t_block *free_small_blocks = malloc_zones.first_small_block_free;
    print_generic_free_blocks(free_tiny_blocks, "FREE TINY : \n");
    print_generic_free_blocks(free_small_blocks, "FREE SMALL : \n");
    if (free_tiny_blocks == NULL && free_small_blocks == NULL){
        return ;
    }
    print_str("==================FINE SHOW FREE MEM==================\n\n");
}