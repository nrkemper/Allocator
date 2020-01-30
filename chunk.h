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
    unsigned long   freespace;
    unsigned long   usedspace;
    unsigned long   totalspace;
    unsigned short  nfreeblocks;    //used only if fixed
    unsigned short  nusedblocks;
    unsigned int    sblocks;
    struct chunk*   next;
    struct chunk*   prev;
    bool            fixed;
    bool            readonly;
    struct blockdll blocks;
};

unsigned long __chunk_calculate_size (unsigned int sblocks,
                                      unsigned int nblocks, bool fixed,
                                      unsigned int sheap);
bool __chunk_fixed_partition (unsigned int sblocks, unsigned int nblocks,
                              bool fixed, unsigned int sheap, bool readonly,
                              char name[17]);
void __chunk_dump (struct chunk* c, FILE* stream);
void __chunk_free (struct chunk* c);

#endif /* __CHUNK_H__ */
