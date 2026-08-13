#include "includes/malloc.h"
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>

int main(int ac, char **av){
    //char *ptr = ft_malloc(sizeof(char) * (5 + 1));
    int pages_size = sysconf(_SC_PAGESIZE);
    int size = atoi(av[1]);
    void *ptr = mmap(NULL, ALIGN_PAGES(size, pages_size), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);


    printf("%p", ptr);
    return (0);
}