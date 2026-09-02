#ifndef MALLOC_INTERNAL_H
# define MALLOC_INTERNAL_H

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

//se con l operatore & l espressione ritorna 1 vuol dire che l ultimo bit e' 1 quindi occupato senno' 0 libero
#define is_free(size) ((((size) & 1)) ? false : true)
//settiamo l ultimo bit a 0 invertendo 1 con ~
#define set_free(size) ((size) &= ~1)
//settiamo l ultimo bit a 1 usando |
#define set_allocated(size) ((size) |= 1)
// 6 in binario e' 0110 quindi invertendo 6 azzeriamo quei bit di size a 0 e poi con << 1 mettiamo la nuova zona che e' rappresentata da
// 00 TINY, 01 SMALL, 10 LARGE
#define set_zone(size, zone) ((size) = ((size) & ~6) | ((zone) << 1))
// confrontiamo i bit ZZ con 6 isolandoli. Poi shiftiamo a destra di 1 (>> 1) 
// per riportare il valore al suo stato originale (0, 1 o 2) facendolo 
// combaciare con i valori dell'enum ZONE_AREA.
#define get_zone(size) (((size) & 6) >> 1)
// 15 in binario e' 1111 invertendolo mettiamo i bit dei metadati a 0000 cosi' avremo il numero di size reale
#define get_size(size) ((size) & ~15)
// macro per allineamento in memoria: il kernel e' molto piu' veloce a leggere la memoria "allineata"(la memoria e' allineata quando e' un multiplo di 16)
// questa formula ci garantisce che qualsiasi allocazione venga richiesta, se non sara' un multiplo di 16 verra' arrotondata per eccesso rendendola allineata
// la formula funziona in questo modo:
// memoria per i miei metadati 16,  memoria richiesta 1
// size = 17 => 17+15=32 (gia' allineato) => in binario 32 e' 00100000
// & ~15 non ha effetto, perche' quei bit sono gia' a 0 e lascera' la size a 32
#define ALIGN_16(size) (((size) + 15) & ~15)

// dobbiamo allineare anche le pagine, per evitare sprechi di memoria.
// esempio: creiamo una pagina che richiede 13000byte, anche il kernel allinea la memoria a secondo del sistema operativo
// nel nostro caso sysconf(_SC_PAGESIZE) = 4096 quindi le pagine saranno di questa grandezza.
// se noi abbiamo bisogno di 13000byte il kernel non ci puo' dare 2 pagine e un terzo ma ce ne dara' 3.
// senza questa formula, noi useremo solo 13000byte, non utilizzando il resto della memoria
// con questa formula invece, verra' arrotondato sempre a un multiplo di 4096, in questo esempio 16384byte.
// quando creeremo le pagine, nella chiamata a mmap, avremo qualcosa del genere ALIGN_PAGES(size_richiesta, sysconf(_SC_PAGESIZE))
#define ALIGN_PAGES(size, aligment) (((size) + (aligment - 1)) & ~(aligment - 1))

// Spazio totale = sizeof(t_zone=>32) + (100 * [Dimensione massima del singolo blocco])
# define LIMIT_TINY_ZONE_AREA 128
# define LIMIT_SMALL_ZONE_AREA 512

typedef enum{
	TINY, //00
	SMALL, //01
	LARGE //10
} ZONE_AREA;

/*typedef struct s_metadata{
	unsigned int	is_free;
	ZONE_AREA	 	zone_area;
	size_t			size;
}	t_metadata;*/

//32 byte
typedef struct s_block{
	size_t	size;
	size_t	prev_size;
	/* L'union indica al programma di usare lo stesso blocco di memoria sia per free_pointers che per data.
    Questo mi servirà per fare in modo che, quando il blocco di memoria è libero, ci salvo dentro le informazioni sulla lista
    dei blocchi liberi; quando è occupato, ci saranno i dati dell'utente.
    Siccome i due puntatori pesano 8 byte ciascuno (16 byte in totale), per qualunque allocazione verrà richiesta, il payload
    sarà di almeno 16 byte e mai di meno, per evitare di sovrascrivere dati adiacenti e causare segfault. */
	union {
        struct {
            struct s_block *next;
            struct s_block *back;
        } free_pointers;
        char data[0];
    } payload;
}	t_block;

// il compilatore arrotonda 32 byte aggiungendo del padding, proprio per allineare e rendere il kernel piu' efficiente
typedef struct s_zone{
	size_t					zone_size;//8 byte
	size_t					allocated_blocks;//8 byte
	struct s_zone*			next_zone;//8 byte
	ZONE_AREA				zone_area;//4 byte
}	t_zone;


typedef struct s_malloc_zones{
    t_zone  *tiny_zones;
    t_zone  *small_zones;
    t_zone  *large_zones;

    t_block *first_tiny_block_free;
    t_block *first_small_block_free;

	size_t	system_pages_size;
    size_t  tiny_mmap_pages_size;
    size_t  small_mmap_pages_size;
}   g_malloc_zones;


extern g_malloc_zones  malloc_zones;

// l implementazione e' nel file src/utils
ZONE_AREA   get_zone_area(size_t size);

bool    	is_tiny_area(size_t size);
bool    	is_small_area(size_t size);
bool    	is_large_area(size_t size);

//free functions
void    try_free_large_ptr(t_zone *assigned_zone, void* ptr);
void    try_free_generic_memory(t_zone *assigned_area, void *ptr);

//free functions utils
void    delete_zone_from_list(t_zone **head_zone, t_zone *zone_to_delete);
t_zone  *find_zone_by_ptr(void *ptr);

//malloc functions
void    *alloc_large_area(size_t block_size);
void    *request_generic_memory(t_zone **head_zone, t_block **first_free_block, size_t block_size,  ZONE_AREA area);

//malloc functions utils
void    add_new_zone(t_zone **head_zone, t_zone *new_zone);
void    initialize_zone(t_zone **head_zone, t_zone *new_zone, ZONE_AREA area, size_t zone_size);
void    initialize_block(t_block *new_block, size_t block_size, ZONE_AREA area);
void    initialize_free_block(t_block *new_free_block, size_t prev_size, size_t free_block_size, ZONE_AREA area);
void    add_new_free_block(t_block **first_free_block, t_block *new_free_block);

//malloc generic memory utils
void    split_block(t_block **first_free_block, t_block *free_block, size_t block_size);
void    *handle_perfect_fit(t_block **first_free_block, t_block *block);
t_zone  *get_zone_from_block(t_zone **head_zone, t_block *block);

//print utils
void    print_str(char *str);
void    print_nbr(size_t n);
void    print_hex_address(void *ptr);

#endif