#include "includes/malloc.h"
#include "includes/malloc_internal.h"
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>

void    test_large_malloc(){
    //test per richiesta large
    char *large_ptr = ft_malloc(4900);
    char *large_ptr2 = ft_malloc(4900);
    char *large_ptr3 = ft_malloc(4900);
    char *large_ptr4 = ft_malloc(4900);
    char *large_ptr5 = ft_malloc(4900);

    for (int i = 0; i < 4899; i++){
        large_ptr[i] = 'a';
        large_ptr2[i] = 'b';
        large_ptr3[i] = 'c';
        large_ptr4[i] = 'd';
        large_ptr5[i] = 'e';
    }
    large_ptr[4899] = '\0';
    large_ptr2[4899] = '\0';
    large_ptr3[4899] = '\0';
    large_ptr4[4899] = '\0';
    large_ptr5[4899] = '\0';

    //printf("%s\n", large_ptr);
    //printf("%s\n", large_ptr2);
    //printf("%s\n", large_ptr3);
    //printf("%s\n", large_ptr4);
    //printf("%s\n", large_ptr5);
}

void    test_small_malloc(){
    char *small_ptr = ft_malloc(300);
    char *small_ptr1 = ft_malloc(300);
    char *small_ptr2 = ft_malloc(300);
    char *small_ptr3 = ft_malloc(300);
    char *small_ptr4 = ft_malloc(300);
    char *small_ptr5 = ft_malloc(300);
    
    for (int i = 0; i < 299; i++){
        small_ptr[i] = 'a';
        small_ptr1[i] = 'b';
        small_ptr2[i] = 'c';
        small_ptr3[i] = 'd';
        small_ptr4[i] = 'e';
        small_ptr5[i] = 'f';
    }

    small_ptr[299] = '\0';
    small_ptr1[299] = '\0';
    small_ptr2[299] = '\0';
    small_ptr3[299] = '\0';
    small_ptr4[299] = '\0';
    small_ptr5[299] = '\0';
    //printf("%s\n", small_ptr);
    //printf("%s\n", small_ptr1);
    //printf("%s\n", small_ptr2);
    //printf("%s\n", small_ptr3);
    //printf("%s\n", small_ptr4);
    //printf("%s\n", small_ptr5);
}

void    test_tiny_malloc(){
    char *tiny_ptr = ft_malloc(50);
    char *tiny_ptr1 = ft_malloc(50);
    char *tiny_ptr2 = ft_malloc(50);
    char *tiny_ptr3 = ft_malloc(50);
    char *tiny_ptr4 = ft_malloc(50);
    char *tiny_ptr5 = ft_malloc(50);
    
    for (int i = 0; i < 49; i++){
        tiny_ptr[i] = 'a';
        tiny_ptr1[i] = 'b';
        tiny_ptr2[i] = 'c';
        tiny_ptr3[i] = 'd';
        tiny_ptr4[i] = 'e';
        tiny_ptr5[i] = 'f';
    }
    tiny_ptr[49] = '\0';
    tiny_ptr1[49] = '\0';
    tiny_ptr2[49] = '\0';
    tiny_ptr3[49] = '\0';
    tiny_ptr4[49] = '\0';
    tiny_ptr5[49] = '\0';

    /*print_str(tiny_ptr);
    print_str("\n");

    print_str(tiny_ptr1);
    print_str("\n");

    print_str(tiny_ptr2);
    print_str("\n");

    print_str(tiny_ptr3);
    print_str("\n");

    print_str(tiny_ptr4);
    print_str("\n");

    print_str(tiny_ptr5);
    print_str("\n"); */
}

int main(){

    test_large_malloc();

    test_small_malloc();

    test_tiny_malloc();

    show_alloc_mem();

    return (0);
}