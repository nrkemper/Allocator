//
//  chunk.c
//  Allocator Attempt 2
//
//  Created by Nicholas Kemp on 2020-01-18.
//  Copyright © 2020 Nicholas Kemp. All rights reserved.
//

#include <string.h>
#include "chunk.h"
#include "sys.h"

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
    Sys_FPrintf (stream, "---------------------------\n\n");
}
