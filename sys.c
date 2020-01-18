//
//  sys.c
//  Allocator Attempt 2
//
//  Created by Nicholas Kemp on 2020-01-17.
//  Copyright © 2020 Nicholas Kemp. All rights reserved.
//

#include <stdio.h>
#include <stdarg.h>
#include "sys.h"

void Sys_Printf (const char* str, ...)
{
    va_list     args;
    
    va_start (args, str);
    vprintf (str, args);
    va_end (args);
}

void Sys_FPrintf (FILE* stream, const char* str, ...)
{
    va_list     args;
    
    va_start (args, str);
    vfprintf (stream, str, args);
    va_end (args);
}
