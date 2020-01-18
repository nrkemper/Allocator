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
    struct block        blocks[20];
    int                 i;
    
    memset (blocks, 0, sizeof (struct block) * 20);
    
    for (i=0; i<20; i++) {
        blocks[i].id = i;
        
        tmp         = (struct blocknode*)malloc (sizeof (struct blocknode));
        tmp->ref    = &blocks[i];
        
        __blockdll_push (&dll, tmp);
    }
    
    __blockdll_dump (&dll, stdout);
    
    Sys_Printf("TRANSVERSING FORWARDS\n");
    for (tmp=dll.head,i=0; tmp; tmp=tmp->next, i++);
    Sys_Printf("Transversed %d nodes\n", i);
    Sys_Printf("TRANSVERSING BACKWARDS\n");
    for (tmp=dll.tail, i=0; tmp; tmp=tmp->prev, i++);
    Sys_Printf("Transversed %d nodes\n", i);
    Sys_Printf ("Destroyed %u nodes\n", __blockdll_destroy (&dll));
    return 0;
}
