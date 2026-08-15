#include "includes/malloc.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>

int main(){
    char *ptr = ft_malloc(4900);

    for (int i = 0; i < 10; i++){
        ptr[i] = 'a';
    }

    strcat(ptr, "ww");
    printf("%s", ptr);
    return (0);
}