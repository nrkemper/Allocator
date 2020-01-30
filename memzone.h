//
//  memzone.h
//  Allocator Attempt 2
//
//  Created by Nicholas Kemp on 2020-01-19.
//  Copyright © 2020 Nicholas Kemp. All rights reserved.
//

#ifndef __MEMZONE_H__
#define __MEMZONE_H__

#define NUM_CHUNKS              9
#define NUM_64BYTE_BLOCKS       25
#define NUM_128BYTE_BLOCKS      35
#define NUM_192BYTE_BLOCKS      25
#define NUM_256BYTE_BLOCKS      35
#define NUM_320BYTE_BLOCKS      25
#define NUM_384BYTE_BLOCKS      10
#define NUM_512BYTE_BLOCKS      10
#define NUM_640BYTE_BLOCKS      10
#define HEAP_SIZE               4096

#define TRUE    1
#define FALSE   0

#include "chunkdll.h"

typedef char    bool;

struct memzone {
    void*           data;
    unsigned long   usedspace;
    unsigned long   freespace;
    unsigned long   totalspace;
    unsigned int    nchunks;
    struct chunkdll chunks;
    struct chunk*   clast;      //last chunk to be allocated
};

extern struct memzone*      memory;
extern unsigned long        totmemalloc;    //total memory to be cleaned up
extern struct blockdll      sparenodes;
extern unsigned int         memid;
extern bool                 meminit;        //memzone initialized

/*
 *  MISCELLANEOUS MEMORY FUNCTIONS
 */
void* align_address (void* addr);
short calculate_padding (unsigned long n);
char word_size (void);

/*
 *  MEMZONE FUNCTIONS
 */
bool __memzone_init (void);
unsigned long __memzone_calculate_size (void);
void __memzone_dump (FILE* stream);
bool __memzone_destroy (void);


#endif /* __MEMZONE_H__ */
