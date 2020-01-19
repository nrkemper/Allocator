//
//  main.c
//  Allocator Attempt 2
//
//  Created by Nicholas Kemp on 2020-01-17.
//  Copyright © 2020 Nicholas Kemp. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sys.h"

#include "blockdll.h"

int main(int argc, const char * argv[]) {
    // insert code here...
    
    struct blockdll     dll;
    struct blocknode*   tmp;
    struct block        blocks[25];
    int                 i;
    
    memset (blocks, 0, sizeof (struct block) * 25);
 
    blocks[23].id   = 23;
    tmp             = (struct blocknode*)malloc (sizeof (struct blocknode));
    tmp->ref        = &blocks[23];
    __blockdll_insert(&dll, dll.head, tmp);
    
    for (i=0; i<20; i++) {
        blocks[i].id = i;
        
        tmp         = (struct blocknode*)malloc (sizeof (struct blocknode));
        tmp->ref    = &blocks[i];
        
        __blockdll_push (&dll, tmp);
    }
    
    blocks[20].id   = 20;
    tmp             = (struct blocknode*)malloc (sizeof (struct blocknode));
    tmp->ref        = &blocks[20];
    __blockdll_insert(&dll, dll.head, tmp);
  
    blocks[21].id   = 21;
    tmp             = (struct blocknode*)malloc (sizeof (struct blocknode));
    tmp->ref        = &blocks[21];
    __blockdll_insert(&dll, dll.tail, tmp);
    
    blocks[22].id   = 22;
    tmp             = (struct blocknode*)malloc (sizeof (struct blocknode));
    tmp->ref        = &blocks[22];
    __blockdll_insert(&dll, dll.head->next->next, tmp);
    
    __blockdll_dump(&dll, stdout);
    
    Sys_Printf("TRANSVERSING NODES FORWARDS...");
    for (i=0, tmp=dll.head; tmp; tmp=tmp->next, i++);
    Sys_Printf("SUCCESS\n");
    Sys_Printf("Transversed %d nodes\n", i);
    
    Sys_Printf("TRANSVERSING NODES BACKWARDS...");
    for (i=0, tmp=dll.tail; tmp; tmp=tmp->prev, i++);
    Sys_Printf("SUCCESS\n");
    Sys_Printf("Transversed %d nodes\n", i);
    
    Sys_Printf ("Destroyed %u nodes\n", __blockdll_destroy(&dll));
    return 0;
}
