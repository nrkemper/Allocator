//
//  block.c
//  Allocator Attempt 2
//
//  Created by Nicholas Kemp on 2020-01-18.
//  Copyright © 2020 Nicholas Kemp. All rights reserved.
//

#include "block.h"
#include "memzone.h"
#include "sys.h"

bool __block_fixed_partition (struct chunk* c, unsigned int size)
{
    struct block*   b;
    
    if (!c->fixed)
        return FALSE;
    
    if (size > c->freespace)
        return FALSE;
    return 1;
}

bool __block_partition (struct chunk* c, unsigned int size)
{
    
}

void __block_dump (struct block* b, FILE* stream)
{
    Sys_FPrintf (stream, " --------------------------\n");
    Sys_FPrintf (stream, "|          MEMORY          |\n");
    Sys_FPrintf (stream, " --------------------------\n");
    Sys_FPrintf (stream, "TYPE:         BLOCK\n");
    Sys_FPrintf (stream, "ID:           %u\n", b->id);
    Sys_FPrintf (stream, "ADDRESS:      %p\n", b);
    Sys_FPrintf (stream, "START:        %p\n", b->start);
    Sys_FPrintf (stream, "END:          %p\n", b->end);
    Sys_FPrintf (stream, "SIZE:         %u\n", b->size);
}
