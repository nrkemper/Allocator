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

unsigned long __chunk_calculate_size (unsigned int sblocks,
                                      unsigned int nblocks, bool fixed,
                                      unsigned int sheap)
{
    unsigned long       size;
    
    size = sizeof (struct chunk) + calculate_padding (sizeof (struct chunk));
    
    if (fixed) {
        sblocks =   sizeof (struct block) +
                    calculate_padding(sizeof (struct block)) +
                    sblocks +
                    calculate_padding (sblocks);
        
        size    += (sblocks * nblocks);
    } else {
        size += sheap + calculate_padding (sheap);
    }
    
    return size;
}

bool __chunk_fixed_partition (unsigned int sblocks, unsigned int nblocks,
                              bool fixed, unsigned int sheap, bool readonly,
                              char name[17])
{
    struct chunk*       c;
    struct chunknode*   node;
    
    sblocks = sblocks + calculate_padding (sblocks);
    
    if (memory->clast) {
        c                   = (struct chunk*)((char*)memory->clast +
                                              memory->clast->totalspace);
        memory->clast->next = c;
        c->prev             = memory->clast;
    } else {
        c       = memory->data;
        c->prev = (struct chunk*)0;
    }
    
    memory->clast       = c;
    
    memset (c, 0, sizeof (struct chunk));
    c->id           = memid++;
    memcpy (c->name, name, 16);
    c->totalspace   = __chunk_calculate_size(sblocks, nblocks, fixed, sheap);
    c->usedspace    = sizeof (struct chunk);
    c->freespace    = c->totalspace - c->usedspace;
    c->sblocks      = fixed ? sblocks : 0;
    c->nfreeblocks  = nblocks;
    c->nusedblocks  = 0;
    c->fixed        = fixed;
    c->readonly     = readonly;
    c->next         = (struct chunk*)0;
    c->blast        = (struct block*)0;
    c->start        = (char*)c + sizeof (struct chunk);
    c->end          = (char*)c + c->totalspace - 1;
    
    if (fixed) {
        while (nblocks--) {
            __block_fixed_partition (c, sblocks);
        }
    }
    
    node = (struct chunknode*)malloc (sizeof (struct chunknode));
    if (node == (struct chunknode*)0)
        return 0;
    
    node->ref   = c;
    __chunkdll_push (&memory->chunks, node);
    totmemalloc         += sizeof (struct chunknode);
    
    memory->usedspace   += sizeof (struct chunk);
    memory->freespace   -= sizeof (struct chunk);
    memory->nchunks++;
    
    return 1;
}

void __chunk_dump (struct chunk* c, FILE* stream)
{
    char    name[17];
    
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
    Sys_FPrintf (stream, "FREE SPACE:   %lu\n", c->freespace);
    Sys_FPrintf (stream, "USED SPACE:   %lu\n", c->usedspace);
    Sys_FPrintf (stream, "TOTAL SPACE:  %lu\n", c->totalspace);
    Sys_FPrintf (stream, "NEXT CHUNK:   %p\n", c->next);
    Sys_FPrintf (stream, "PREV CHUNK:   %p\n", c->prev);
    Sys_FPrintf (stream, "FREE BLOCKS:  %hu\n", c->nfreeblocks);
    Sys_FPrintf (stream, "USED BLOCKS:  %hu\n", c->nusedblocks);
    Sys_FPrintf (stream, "SIZE BLOCKS:  %hu\n", c->sblocks);
    Sys_FPrintf (stream, "FIXED:        %s\n", c->fixed ? "YES" : "NO");
    Sys_FPrintf (stream, "READONLY:     %s\n", c->readonly ? "YES" : "NO");
    Sys_FPrintf (stream, "--------------------------------\n\n");
}

unsigned long __chunk_free (struct chunk* c)
{
    unsigned long   freed;
    freed = __blockdll_destroy(&c->blocks);
    //FIXME: blksbysize & blksbyaddr
    
    return freed;
}
