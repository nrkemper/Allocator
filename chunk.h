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
#include "chunkdll.h"

typedef char        bool;

struct chunk {
    unsigned int    id;
    void*           start;
    void*           end;
    char            name[16];
    unsigned long   freespace;
    unsigned long   usedspace;
    unsigned long   totalspace;
    unsigned short  nfreeblocks;    //used only if fixed
    unsigned short  nusedblocks;
    unsigned short  sblocks;
    struct chunkdll blksaddr;       //blocklist sorted by address
    struct chunkdll blkssize;       //blocklist sorted by size
    struct chunk*   next;
    struct chunk*   prev;
    bool            fixed;
    bool            readonly;
};

void __chunk_dump (struct chunk* c, FILE* stream);

#endif /* __CHUNK_H__ */
