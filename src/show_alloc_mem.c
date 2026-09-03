#include "../includes/malloc_internal.h"

static inline void  print_generic_zones(t_zone *head_zone, char *label){
    
    while (head_zone != NULL)
    {
        print_str(label);
        print_hex_address((void*)head_zone);
        print_str("\n");
        void    *end_zone = (void *)((char *)head_zone + head_zone->zone_size);
        t_block *current_block = (t_block *)((char *)head_zone + sizeof(t_zone));
        while ((void *)current_block > (void *)head_zone && (void *)current_block < end_zone){
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
        head_zone = head_zone->next_zone;
    }
}

static inline void  print_large_zones(){
    t_zone  *large_zone = malloc_zones.large_zones;
    while (large_zone != NULL){
        print_str("LARGE :  ");
        print_hex_address((void *)large_zone);
        print_str("\n");
        t_block *current_block = (t_block *)((char *)large_zone + sizeof(t_zone));
        if (!is_free(current_block->size)){
            size_t  payload_size = get_size(current_block->size);
            void    *end_block = ((char *)current_block + payload_size);
            print_hex_address((void *)current_block);
            print_str(" - ");
            print_hex_address((void *)end_block);
            print_str(" : ");
            print_nbr(payload_size);
            print_str("\n");
        }
        large_zone = large_zone->next_zone;
    }
}

void    show_alloc_mem(){
    if (malloc_zones.tiny_zones == NULL && malloc_zones.small_zones == NULL && malloc_zones.large_zones == NULL){
        print_str("==================NESSUNA ZONA ALLOCATA==================\n\n");
        return ;
    }
    print_generic_zones(malloc_zones.tiny_zones, "TINY : ");
    print_generic_zones(malloc_zones.small_zones, "SMALL : ");
    print_large_zones();
    print_str("==================FINE SHOW ALLOC MEM==================\n\n");
}