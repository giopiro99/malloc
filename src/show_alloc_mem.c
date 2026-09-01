#include "../includes/malloc_internal.h"

static inline void  print_tiny_zones(){
    t_zone  *TINY_ZONE = malloc_zones.tiny_zones;
    
    while (TINY_ZONE != NULL)
    {
        print_str("TINY : ");
        print_hex_address((void*)TINY_ZONE);
        print_str("\n");
        void    *end_zone = (void *)((char *)TINY_ZONE + TINY_ZONE->zone_size);
        t_block *current_block = (t_block *)((char *)TINY_ZONE + sizeof(t_zone));
        while ((void *)current_block > (void *)TINY_ZONE && (void *)current_block < end_zone){
            size_t current_block_size = get_size(current_block->size);
            if (!(is_free(current_block->size))){
                size_t  payload_size = current_block_size - sizeof(t_block);
                void    *end_block = ((char *)current_block + payload_size);
                print_hex_address((void *)current_block);
                print_str(" - ");
                print_hex_address((void *)end_block);
                print_str(" : ");
                print_nbr(payload_size);
                print_str("\n");
            }

            current_block = (t_block *)((char *)current_block + current_block_size);
        }
        TINY_ZONE = TINY_ZONE->next_zone;
    }
}

static inline void  print_small_zones(){
    t_zone  *SMALL_ZONE = malloc_zones.small_zones;
    
    while (SMALL_ZONE != NULL)
    {
        print_str("SMALL : ");
        print_hex_address((void*)SMALL_ZONE);
        print_str("\n");
        void    *end_zone = (void *)((char *)SMALL_ZONE + SMALL_ZONE->zone_size);
        t_block *current_block = (t_block *)((char *)SMALL_ZONE + sizeof(t_zone));
        while ((void *)current_block > (void *)SMALL_ZONE && (void *)current_block < end_zone){
            size_t current_block_size = get_size(current_block->size);
            if (!(is_free(current_block->size))){
                size_t  payload_size = current_block_size - sizeof(t_block);
                void    *end_block = ((char *)current_block + payload_size);
                print_hex_address((void *)current_block);
                print_str(" - ");
                print_hex_address((void *)end_block);
                print_str(" : ");
                print_nbr(payload_size);
                print_str("\n");
            }
            current_block = (t_block *)((char *)current_block + current_block_size);
        }
        SMALL_ZONE = SMALL_ZONE->next_zone;
    }
}

static inline void  print_large_zones(){
    t_zone  *LARGE_ZONE = malloc_zones.large_zones;
    
    while (LARGE_ZONE != NULL)
    {
        print_str("LARGE : ");
        print_hex_address((void*)LARGE_ZONE);
        print_str("\n");
        void    *end_zone = (void *)((char *)LARGE_ZONE + LARGE_ZONE->zone_size);
        t_block *current_block = (t_block *)((char *)LARGE_ZONE + sizeof(t_zone));
        while ((void *)current_block > (void *)LARGE_ZONE && (void *)current_block < end_zone){
            size_t current_block_size = get_size(current_block->size);
            if (!(is_free(current_block->size))){
                size_t  payload_size = current_block_size - sizeof(t_block);
                void    *end_block = ((char *)current_block + payload_size);
                print_hex_address((void *)current_block);
                print_str(" - ");
                print_hex_address((void *)end_block);
                print_str(" : ");
                print_nbr(payload_size);
                print_str("\n");
            }
            //print_hex_address(current_block);
            //print_str("\nsize del blocco inn show alloc mem=");
            //print_nbr(current_block_size);
            //print_str("\n");
            current_block = (t_block *)((char *)current_block + current_block_size);
        }
        LARGE_ZONE = LARGE_ZONE->next_zone;
    }
}

void    show_alloc_mem(){
    print_tiny_zones();   
    print_small_zones();
    print_large_zones(); 
}