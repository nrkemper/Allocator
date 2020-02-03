//
//  chunk.h
//  Allocator Attempt 2
//
//  Created by Nicholas Kemp on 2020-01-18.
//  Copyright © 2020 Nicholas Kemp. All rights reserved.
//

#ifndef __CHUNK_H__
#define __CHUNK_H__

#include <stdio.h>
#include "blockdll.h"

typedef char        bool;

struct chunk {
    unsigned int    id;
    void*           start;
    void*           end;
    char            name[16];
    unsigned long   free_space;         //space not partitioned into a block or used for a
    unsigned long   used_space;         //header
    unsigned long   total_space;
    unsigned short  num_free_blocks;    //used only if fixed
    unsigned short  num_used_blocks;
    unsigned int    size_blocks;        //0 if not fixed. size of blocks if fixed
    struct chunk*   next;
    struct chunk*   prev;
    struct block*   last_block;     //last block to be partitioned. Only if fixed
    bool            fixed;
    bool            read_only;
    struct blockdll blocks_by_addr;
    struct blockdll blocks_by_size;
    struct blockdll free_blocks;
};

unsigned long __chunk_calculate_size (unsigned int block_size,
                                      unsigned int num_blocks, bool fixed,
                                      unsigned int heap_size);
bool __chunk_fixed_partition (unsigned int block_size, unsigned int num_blocks,
                              bool fixed, unsigned int heap_size, bool read_only,
                              char name[17]);
void __chunk_dump (struct chunk* c, FILE* stream);
unsigned long __chunk_free (struct chunk* c);

#endif /* __CHUNK_H__ */
