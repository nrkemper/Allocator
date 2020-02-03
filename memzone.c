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
        return false;
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
    
    __chunk_fixed_partition (64, NUM_64BYTE_BLOCKS, true, 0, false,
                             "64_BYTE_BLOCKS");
    
    __chunk_fixed_partition (128, NUM_128BYTE_BLOCKS, true, 0, false,
                             "128_BYTE_BLOCKS");
    
    __chunk_fixed_partition (192, NUM_192BYTE_BLOCKS, true, 0, false,
                             "192_BYTE_BLOCKS");
    
    __chunk_fixed_partition (256, NUM_256BYTE_BLOCKS, true, 0, false,
                             "256_BYTE_BLOCKS");
    
    __chunk_fixed_partition (320, NUM_320BYTE_BLOCKS, true, 0,false,
                             "320_BYTE_BLOCKS");
    
    __chunk_fixed_partition (384, NUM_384BYTE_BLOCKS, true, 0, false,
                             "384_BYTE_BLOCKS");
    
    __chunk_fixed_partition (512, NUM_512BYTE_BLOCKS, true, 0, false,
                             "512_BYTE_BLOCKS");
    
    __chunk_fixed_partition (640, NUM_640BYTE_BLOCKS, true, 0, false,
                             "640_BYTE_BLOCKS");
    
    __chunk_fixed_partition(0, 0, false, HEAP_SIZE, false, "HEAP");
    
    Sys_Printf("TOTAL MEMORY ALLOCATED: %lu\n", totmemalloc);
    return true;
}

unsigned long __memzone_calculate_size (void)
{
    unsigned long   size;
    
    size = sizeof (struct memzone) + calculate_padding(sizeof (struct memzone));
    size += __chunk_calculate_size(64, NUM_64BYTE_BLOCKS, true, 0);
    size += __chunk_calculate_size(128, NUM_128BYTE_BLOCKS, true, 0);
    size += __chunk_calculate_size(192, NUM_192BYTE_BLOCKS, true, 0);
    size += __chunk_calculate_size(256, NUM_256BYTE_BLOCKS, true, 0);
    size += __chunk_calculate_size(320, NUM_320BYTE_BLOCKS, true, 0);
    size += __chunk_calculate_size(384, NUM_384BYTE_BLOCKS, true, 0);
    size += __chunk_calculate_size(512, NUM_512BYTE_BLOCKS, true, 0);
    size += __chunk_calculate_size(640, NUM_640BYTE_BLOCKS, true, 0);
    size += __chunk_calculate_size(0, 0, false, HEAP_SIZE);
    
    return size;
}

void __memzone_dump (FILE* stream)
{
    struct chunk*   c = memory->clast;
    
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
    unsigned long       mzsize = memory->totalspace;
    unsigned long       bnodefreed=0, cnodefreed, totfreed;
    struct chunk*       c=memory->data;
    bool                ret=true;
    
    while (c) {
        bnodefreed  += __chunk_free (c);
        c           = c->next;
    }
    
    bnodefreed += __blockdll_destroy (&sparenodes);
    cnodefreed = __chunkdll_destroy(&memory->chunks);
    free (memory);
    
    totfreed    = bnodefreed + cnodefreed + mzsize;
    
    Sys_Printf ("TOTAL MEMORY ALLOCATED:    %lu bytes\n", totmemalloc);
    Sys_Printf ("DESTROYED BLOCK NODES:     %lu bytes\n", bnodefreed);
    Sys_Printf ("DESTROYED CHUNK NODES:     %lu bytes\n", cnodefreed);
    Sys_Printf ("DESTROYED MEMZONE:         %lu bytes\n", mzsize);
    Sys_Printf ("TOTAL MEMORY FREED:        %lu bytes\n", totfreed);
    Sys_Printf ("STATUS OF MEMORY:          ");
    
    //FIXME: clean this up
    if (totfreed < totmemalloc) {
        Sys_Printf ("MEMORY LEAK\n");
        ret = false;
    } else if (totfreed > totmemalloc) {
        Sys_Printf ("SYSTEM ERROR\n");
        ret = false;
    } else if (totfreed == totmemalloc)
        Sys_Printf ("SUCCESS\n");
    
    meminit = false;
    totmemalloc -= totfreed;
    
    return ret;
}
