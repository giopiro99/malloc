#include "includes/malloc.h"
#include "includes/malloc_internal.h"
#include <stdint.h>
#include <unistd.h>
#include <sys/mman.h>

static inline void  initialize_ptr(char *ptr, char *ptr1, char *ptr2, char *ptr3,
                                    char *ptr4, char *ptr5, int size){
    for (int i = 0; i < size; i++){
        ptr[i] = 'a';
        ptr1[i] = 'b';
        ptr2[i] = 'c';
        ptr3[i] = 'd';
        ptr4[i] = 'e';
        ptr5[i] = 'f';
    }

    ptr[size] = '\0';
    ptr1[size] = '\0';
    ptr2[size] = '\0';
    ptr3[size] = '\0';
    ptr4[size] = '\0';
    ptr5[size] = '\0';
}

static inline void  free_ptr(char *ptr, char *ptr1, char *ptr2, char *ptr3,
                                char *ptr4, char *ptr5){
    ft_free(ptr);
    ft_free(ptr1);
    ft_free(ptr2);
    ft_free(ptr3);
    ft_free(ptr4);
    ft_free(ptr5);
}

void    test_large_malloc(){
    int large_size = 590867;
    //test per richiesta large
    char *large_ptr = ft_malloc(large_size);
    char *large_ptr1 = ft_malloc(large_size);
    char *large_ptr2 = ft_malloc(large_size);
    char *large_ptr3 = ft_malloc(large_size);
    char *large_ptr4 = ft_malloc(large_size);
    char *large_ptr5 = ft_malloc(large_size);

    initialize_ptr(large_ptr, large_ptr1, large_ptr2, large_ptr3, large_ptr4, large_ptr5, large_size);
    show_alloc_mem();

    free_ptr(large_ptr, large_ptr1, large_ptr2, large_ptr3, large_ptr4, large_ptr5);

    show_alloc_mem();
}

void    test_small_malloc(){
    int small_size = 287;
    char *small_ptr = ft_malloc(small_size);
    char *small_ptr1 = ft_malloc(small_size);
    char *small_ptr2 = ft_malloc(small_size);
    char *small_ptr3 = ft_malloc(small_size);
    char *small_ptr4 = ft_malloc(small_size);
    char *small_ptr5 = ft_malloc(small_size);
    
    initialize_ptr(small_ptr, small_ptr1, small_ptr2, small_ptr3, small_ptr4, small_ptr5, small_size);

    show_alloc_mem();

    free_ptr(small_ptr, small_ptr1, small_ptr2, small_ptr3, small_ptr4, small_ptr5);

    show_alloc_mem();
    show_free_mem();
}

void    test_tiny_malloc(){
    int tiny_size = 1;
    char *tiny_ptr = ft_malloc(tiny_size);
    char *tiny_ptr1 = ft_malloc(tiny_size);
    char *tiny_ptr2 = ft_malloc(tiny_size);
    char *tiny_ptr3 = ft_malloc(tiny_size);
    char *tiny_ptr4 = ft_malloc(tiny_size);
    char *tiny_ptr5 = ft_malloc(tiny_size);
    
    initialize_ptr(tiny_ptr, tiny_ptr1, tiny_ptr2, tiny_ptr3, tiny_ptr4, tiny_ptr5, tiny_size);

    tiny_ptr = ft_realloc(tiny_ptr, 5);
    show_alloc_mem();

    free_ptr(tiny_ptr, tiny_ptr1, tiny_ptr2, tiny_ptr3, tiny_ptr4, tiny_ptr5);

    show_alloc_mem();
    show_free_mem();

}

int main(){

    test_large_malloc();

    test_small_malloc();

    test_tiny_malloc();


    return (0);
}