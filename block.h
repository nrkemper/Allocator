//
//  block.h
//  Allocator Attempt 2
//
//  Created by Nicholas Kemp on 2020-01-18.
//  Copyright © 2020 Nicholas Kemp. All rights reserved.
//

#ifndef __BLOCK_H__
#define __BLOCK_H__

#include <stdio.h>
#include "chunk.h"

typedef char    bool;

struct block {
    unsigned int    id;
    struct block*   next;
    struct block*   prev;
    void*           start;
    void*           end;
    unsigned int    size;
};

bool __block_fixed_partition (struct chunk* c, unsigned int size);
bool __block_partition (struct chunk* c, unsigned int size);
void __block_dump (struct block* b, FILE* stream);
#endif /* __BLOCK_H__ */
