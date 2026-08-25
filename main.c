#include "includes/malloc.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>

int main(){
/*
    test per richiesta large
    char *ptr = ft_malloc(4900);
    char *ptr2 = ft_malloc(4900);
    char *ptr3 = ft_malloc(4900);
    char *ptr4 = ft_malloc(4900);
    char *ptr5 = ft_malloc(4900);


    for (int i = 0; i < 10; i++){
        ptr[i] = 'a';
        ptr2[i] = 'b';
        ptr3[i] = 'c';
        ptr4[i] = 'd';
        ptr5[i] = 'e';
    }

    strcat(ptr, "ww");
    strcat(ptr2, "ww");
    strcat(ptr3, "ww");
    strcat(ptr4, "ww");
    strcat(ptr5, "ww");

    printf("%s\n", ptr);
    printf("%s\n", ptr2);
    printf("%s\n", ptr3);
    printf("%s\n", ptr4);
    printf("%s\n", ptr5);

    ft_free(ptr);
    ft_free(ptr2);
    ft_free(ptr3);
    ft_free(ptr4);
    ft_free(ptr5);
*/

    char *ptr = ft_malloc(10);
    for (int i = 0; i < 10; i++){
        ptr[i] = 'a';
    }

    printf("%s\n", ptr);
    ft_free(ptr);
    return (0);
}