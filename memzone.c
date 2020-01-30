//
//  memzone.c
//  Allocator Attempt 2
//
//  Created by Nicholas Kemp on 2020-01-19.
//  Copyright © 2020 Nicholas Kemp. All rights reserved.
//

#include <stdlib.h>
#include <string.h>

#include "memzone.h"
#include "chunk.h"
#include "sys.h"

struct memzone*     memory;
unsigned long       totmemalloc;    //total memory to be cleaned up
struct blockdll     sparenodes;
unsigned int        memid=0;
bool                meminit=0;        //memzone initialized
/*
 *  MISCELLANEOUS MEMORY FUNCTIONS
 */
void* align_address (void* addr)
{
    return (char*)addr + calculate_padding((unsigned long)addr);
}

short calculate_padding (unsigned long n)
{
    long wordsize = word_size ();
    
    return ((wordsize - (n & (wordsize - 1))) & (wordsize - 1));
}

char word_size (void)
{
    return sizeof (void*);
}

/*
 *  MEMZONE FUNCTIONS
 */
bool __memzone_init (void)
{
    unsigned long       size = __memzone_calculate_size ();

    if (meminit)
        __memzone_destroy();

    memory = (struct memzone*)malloc (size);
    if (memory == (struct memzone*)0) {
        Sys_FPrintf (stderr, "ERROR: could not allocate memory for memzone\n");
        return FALSE;
    }
    
    totmemalloc         = size;
    memset (memory, 0, size);
    memory->totalspace  = size;
    memory->usedspace   = sizeof (struct memzone) +
                            calculate_padding(sizeof (struct memzone));
    memory->freespace   = memory->totalspace - memory->usedspace;
    memory->data        = (char*)memory +
                            sizeof (struct memzone) +
                            calculate_padding(sizeof (struct memzone));
    
    __chunk_fixed_partition (64, NUM_64BYTE_BLOCKS, TRUE, 0, FALSE,
                             "64_BYTE_BLOCKS");
    
    __chunk_fixed_partition (128, NUM_128BYTE_BLOCKS, TRUE, 0, FALSE,
                             "128_BYTE_BLOCKS");
    
    __chunk_fixed_partition (192, NUM_192BYTE_BLOCKS, TRUE, 0, FALSE,
                             "192_BYTE_BLOCKS");
    
    __chunk_fixed_partition (256, NUM_256BYTE_BLOCKS, TRUE, 0, FALSE,
                             "256_BYTE_BLOCKS");
    
    __chunk_fixed_partition (320, NUM_320BYTE_BLOCKS, TRUE, 0,FALSE,
                             "320_BYTE_BLOCKS");
    
    __chunk_fixed_partition (384, NUM_384BYTE_BLOCKS, TRUE, 0, FALSE,
                             "384_BYTE_BLOCKS");
    
    __chunk_fixed_partition (512, NUM_512BYTE_BLOCKS, TRUE, 0, FALSE,
                             "512_BYTE_BLOCKS");
    
    __chunk_fixed_partition (640, NUM_640BYTE_BLOCKS, TRUE, 0, FALSE,
                             "640_BYTE_BLOCKS");
    
    __chunk_fixed_partition(0, 0, FALSE, HEAP_SIZE, FALSE, "HEAP");
    
    Sys_Printf("TOTAL MEMORY ALLOCATED: %lu\n", totmemalloc);
    return TRUE;
}

unsigned long __memzone_calculate_size (void)
{
    unsigned long   size;
    
    size = sizeof (struct memzone) + calculate_padding(sizeof (struct memzone));
    size += __chunk_calculate_size(64, NUM_64BYTE_BLOCKS, TRUE, 0);
    size += __chunk_calculate_size(128, NUM_128BYTE_BLOCKS, TRUE, 0);
    size += __chunk_calculate_size(192, NUM_192BYTE_BLOCKS, TRUE, 0);
    size += __chunk_calculate_size(256, NUM_256BYTE_BLOCKS, TRUE, 0);
    size += __chunk_calculate_size(320, NUM_320BYTE_BLOCKS, TRUE, 0);
    size += __chunk_calculate_size(384, NUM_384BYTE_BLOCKS, TRUE, 0);
    size += __chunk_calculate_size(512, NUM_512BYTE_BLOCKS, TRUE, 0);
    size += __chunk_calculate_size(640, NUM_640BYTE_BLOCKS, TRUE, 0);
    size += __chunk_calculate_size(0, 0, FALSE, HEAP_SIZE);
    
    return size;
}

void __memzone_dump (FILE* stream)
{
    struct chunk*   c = memory->data;
    
    Sys_FPrintf (stream, " ----------------------\n");
    Sys_FPrintf (stream, "|        MEMORY        |\n");
    Sys_FPrintf (stream, " ----------------------\n");
    Sys_FPrintf (stream, "TYPE:             MEMZONE\n");
    Sys_FPrintf (stream, "ADDRESS:          %p\n", memory);
    Sys_FPrintf (stream, "DATA ADDRESS:     %p\n", memory->data);
    Sys_FPrintf (stream, "FREE SPACE:       %lu\n", memory->freespace);
    Sys_FPrintf (stream, "USED SPACE:       %lu\n", memory->usedspace);
    Sys_FPrintf (stream, "TOTAL SPACE:      %lu\n", memory->totalspace);
    Sys_FPrintf (stream, "NUMBER OF CHUNKS: %lu\n", memory->nchunks);
    Sys_FPrintf (stream, " ----------------------\n\n");
    
    while (c) {
        __chunk_dump (c, stream);
        c = c->next;
    }
}
bool __memzone_destroy (void)
{
    unsigned long       size = memory->totalspace;
    
    __chunkdll_destroy(&memory->chunks);
    free (memory);
    totmemalloc -= size;
    
    Sys_Printf("TOTAL MEMORY REMAINING: %lu\n", totmemalloc);
    return 1;
}
