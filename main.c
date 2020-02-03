//
//  main.c
//  Allocator Attempt 2
//
//  Created by Nicholas Kemp on 2020-01-17.
//  Copyright © 2020 Nicholas Kemp. All rights reserved.
//

#include <stdlib.h>
#include <string.h>
#include "memzone.h"
#include "sys.h"
#include "block.h"


int main(int argc, const char * argv[])
{
    bool        ret;
    struct chunk*   c;
    uint64_t    mz_size = sizeof (struct memzone) +
                            calculate_padding(sizeof (struct memzone)) +
                            __chunk_calculate_size(0, 0, false, HEAP_SIZE);
    
    memory = malloc (mz_size);
    if (memory == (struct memzone*)0)
    {
        Sys_Printf("ERROR: unable to allocate memory for memzone\n");
        exit (-1);
    }
    totmemalloc= mz_size;
    memset (memory, 0, mz_size);
    memory->totalspace  = mz_size;
    memory->usedspace   = sizeof (struct memzone)
                            + calculate_padding(sizeof (struct memzone));
    memory->freespace   = memory->totalspace - memory->usedspace;
    memory->data        = (char*)memory +
                            sizeof (struct memzone) +
                            calculate_padding(sizeof (struct memzone));
    memory->nchunks     = 0;
    memory->clast       = (struct chunk*)0;
    
    ret = __chunk_fixed_partition(0, 0, false, HEAP_SIZE, false, "HEAP");
    if (!ret)
    {
        Sys_Printf("ERROR: failed to partition chunk\n");
    }
    
    c = memory->chunks.head->ref;
    
    ret = __block_partition(c, (char*)c->start + 35, 58);
    if (!ret)
        Sys_Printf("ERROR: failed to partition block\n");
    
    ret = __block_partition(c, (char*)c->start + 136 + 35, 58);
    if (!ret)
        Sys_Printf("ERROR: failed to partition block\n");
    __memzone_dump(stdout);
    
    __block_combine(c->blocks_by_addr.head->ref, c->blocks_by_addr.head->next->ref);
    
    __memzone_dump(stdout);
    __memzone_destroy();
    
    Sys_Printf("SIZE OF BLOCK HEADER:   %lu\n", sizeof (struct block));
    Sys_Printf ("BLOCKSIZE + PADDING:   %lu\n", 58 + calculate_padding(58));
    return 0;
}
