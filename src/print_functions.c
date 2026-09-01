#include "../includes/malloc_internal.h"
#include <stdint.h>
#include <unistd.h>

void    print_str(char *str){
    int i = 0;
    while (str[i]){
        write(1, &str[i], 1);
        i++;
    }
}

void    print_nbr(size_t n){
    
    char    *base = "0123456789";
    if (n >= 10){
        print_nbr(n / 10);
    }

    write(1, &base[n % 10], 1);
}

static void print_hex_recursive(unsigned long addr){
    char *base = "0123456789ABCDEF";

    if (addr >= 16){
        print_hex_recursive(addr / 16);
    }

    write(1, &base[addr % 16], 1);

}

void    print_hex_address(void *ptr){
    if (ptr == NULL){
        return ;
    }

    unsigned long addr = (unsigned long)ptr;

    write(1, "0x", 2);
    if(addr == 0){
        write(1, "0", 1);
        return ;
    }

    print_hex_recursive(addr);

}