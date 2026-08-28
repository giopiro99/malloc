#include "includes/malloc.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>

int main(){

    //test per richiesta large
    char *large_ptr = ft_malloc(4900);
    char *large_ptr2 = ft_malloc(4900);
    char *large_ptr3 = ft_malloc(4900);
    char *large_ptr4 = ft_malloc(4900);
    char *large_ptr5 = ft_malloc(4900);


    for (int i = 0; i < 4900; i++){
        large_ptr[i] = 'a';
        large_ptr2[i] = 'b';
        large_ptr3[i] = 'c';
        large_ptr4[i] = 'd';
        large_ptr5[i] = 'e';
    }
    large_ptr[4900] = '\0';
    large_ptr2[4900] = '\0';
    large_ptr3[4900] = '\0';
    large_ptr4[4900] = '\0';
    large_ptr5[4900] = '\0';

    strcat(large_ptr, "ww");
    strcat(large_ptr2, "ww");
    strcat(large_ptr3, "ww");
    strcat(large_ptr4, "ww");
    strcat(large_ptr5, "ww");

    printf("%s\n", large_ptr);
    printf("%s\n", large_ptr2);
    printf("%s\n", large_ptr3);
    printf("%s\n", large_ptr4);
    printf("%s\n", large_ptr5);

    char *small_ptr = ft_malloc(300);
    char *small_ptr1 = ft_malloc(300);
    char *small_ptr2 = ft_malloc(300);
    char *small_ptr3 = ft_malloc(300);
    char *small_ptr4 = ft_malloc(300);
    char *small_ptr5 = ft_malloc(300);
    
    for (int i = 0; i < 300; i++){
        small_ptr[i] = 'a';
        small_ptr1[i] = 'b';
        small_ptr2[i] = 'c';
        small_ptr3[i] = 'd';
        small_ptr4[i] = 'e';
        small_ptr5[i] = 'f';
    }

    small_ptr[300] = '\0';
    small_ptr1[300] = '\0';
    small_ptr2[300] = '\0';
    small_ptr3[300] = '\0';
    small_ptr4[300] = '\0';
    small_ptr5[300] = '\0';
    printf("%s\n", small_ptr);
    printf("%s\n", small_ptr1);
    printf("%s\n", small_ptr2);
    printf("%s\n", small_ptr3);
    printf("%s\n", small_ptr4);
    printf("%s\n", small_ptr5);

    char *tiny_ptr = ft_malloc(50);
    char *tiny_ptr1 = ft_malloc(50);
    char *tiny_ptr2 = ft_malloc(50);
    char *tiny_ptr3 = ft_malloc(50);
    char *tiny_ptr4 = ft_malloc(50);
    char *tiny_ptr5 = ft_malloc(50);
    
    for (int i = 0; i < 50; i++){
        tiny_ptr[i] = 'a';
        tiny_ptr1[i] = 'b';
        tiny_ptr2[i] = 'c';
        tiny_ptr3[i] = 'd';
        tiny_ptr4[i] = 'e';
        tiny_ptr5[i] = 'f';
    }
    tiny_ptr[50] = '\0';
    tiny_ptr1[50] = '\0';
    tiny_ptr2[50] = '\0';
    tiny_ptr3[50] = '\0';
    tiny_ptr4[50] = '\0';
    tiny_ptr5[50] = '\0';

    printf("%s\n", tiny_ptr);
    printf("%s\n", tiny_ptr1);
    printf("%s\n", tiny_ptr2);
    printf("%s\n", tiny_ptr3);
    printf("%s\n", tiny_ptr4);
    printf("%s\n", tiny_ptr5);
    return (0);
}