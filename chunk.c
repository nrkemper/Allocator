//
//  chunk.c
//  Allocator Attempt 2
//
//  Created by Nicholas Kemp on 2020-01-18.
//  Copyright © 2020 Nicholas Kemp. All rights reserved.
//

#include <stdlib.h>
#include <string.h>
#include "chunk.h"
#include "block.h"
#include "sys.h"
#include "memzone.h"

unsigned long __chunk_calculate_size (unsigned int block_size,
                                      unsigned int num_blocks, bool fixed,
                                      unsigned int heap_size)
{
    unsigned long       total_size;
    
    total_size = sizeof (struct chunk) + calculate_padding (sizeof (struct chunk));
    
    if (fixed) {
        block_size =    sizeof (struct block) +
                        calculate_padding(sizeof (struct block)) +
                        block_size +
                        calculate_padding (block_size);
        
        total_size  += (block_size * num_blocks);
    } else {
        total_size  += heap_size + calculate_padding (heap_size);
    }
    
    return total_size;
}

bool __chunk_fixed_partition (unsigned int block_size, unsigned int num_blocks,
                              bool fixed, unsigned int heap_size, bool read_only,
                              char name[17])
{
    struct chunk*       c;
    struct chunknode*   node;
    uint64_t            total_size = __chunk_calculate_size(block_size, num_blocks,
                                                            fixed, heap_size);
    
    if (total_size > memory->freespace)
        return false;
    
    block_size += calculate_padding (block_size);
    
    if (memory->clast) {
        c                   = (struct chunk*)((char*)memory->clast +
                                              memory->clast->total_space);
        memory->clast->next = c;
        c->prev             = memory->clast;
    } else {
        c       = memory->data;
        c->prev = (struct chunk*)0;
    }
    
    memory->clast       = c;
    
    memset (c, 0, sizeof (struct chunk));
    c->id               = memid++;
    memcpy (c->name, name, 16);
    c->total_space      = total_size;
    c->used_space       = sizeof (struct chunk);
    c->free_space       = c->total_space - c->used_space;
    c->size_blocks      = fixed ? block_size : 0;
    c->num_free_blocks  = num_blocks;
    c->num_used_blocks  = 0;
    c->fixed            = fixed;
    c->read_only        = read_only;
    c->next             = (struct chunk*)0;
    c->last_block       = (struct block*)0;
    c->start            = (char*)c + sizeof (struct chunk);
    c->end              = (char*)c + c->total_space - 1;
    
    if (fixed) {
        while (num_blocks--) {
            __block_fixed_partition (c, block_size);
        }
    }
    
    node = (struct chunknode*)malloc (sizeof (struct chunknode));
    if (node == (struct chunknode*)0)
        return false;
    
    node->ref   = c;
    __chunkdll_push (&memory->chunks, node);
    totmemalloc         += sizeof (struct chunknode);
    
    memory->usedspace   += total_size;
    memory->freespace   -= total_size;
    memory->nchunks++;
    
    return true;
}

void __chunk_dump (struct chunk* c, FILE* stream)
{
    char                name[17];
    struct blocknode*   block_ptr;
    
    memset (name, 0, 17);
    memcpy (name, c->name, 16);
    
    Sys_FPrintf (stream, "--------------------------------\n");
    Sys_FPrintf (stream, "|            MEMORY            |\n");
    Sys_FPrintf (stream, "--------------------------------\n");
    Sys_FPrintf (stream, "TYPE:         CHUNK\n");
    Sys_FPrintf (stream, "ID:           %u\n", c->id);
    Sys_FPrintf (stream, "NAME:         %s\n", c->name);
    Sys_FPrintf (stream, "ADDRESS:      %p\n", c);
    Sys_FPrintf (stream, "START:        %p\n", c->start);
    Sys_FPrintf (stream, "END:          %p\n", c->end);
    Sys_FPrintf (stream, "FREE SPACE:   %lu\n", c->free_space);
    Sys_FPrintf (stream, "USED SPACE:   %lu\n", c->used_space);
    Sys_FPrintf (stream, "TOTAL SPACE:  %lu\n", c->total_space);
    Sys_FPrintf (stream, "NEXT CHUNK:   %p\n", c->next);
    Sys_FPrintf (stream, "PREV CHUNK:   %p\n", c->prev);
    Sys_FPrintf (stream, "FREE BLOCKS:  %hu\n", c->num_free_blocks);
    Sys_FPrintf (stream, "USED BLOCKS:  %hu\n", c->num_used_blocks);
    Sys_FPrintf (stream, "SIZE BLOCKS:  %hu\n", c->size_blocks);
    Sys_FPrintf (stream, "FIXED:        %s\n", c->fixed ? "YES" : "NO");
    Sys_FPrintf (stream, "READONLY:     %s\n", c->read_only ? "YES" : "NO");
    
    for (block_ptr=c->blocks_by_addr.head; block_ptr; block_ptr=block_ptr->next)
        __block_dump(block_ptr->ref, stream);
    
    Sys_FPrintf (stream, "--------------------------------\n\n");
}

unsigned long __chunk_free (struct chunk* c)
{
    unsigned long   freed;
    
    freed = __blockdll_destroy(&c->blocks_by_size);
    freed += __blockdll_destroy(&c->blocks_by_addr);
    
    return freed;
}
