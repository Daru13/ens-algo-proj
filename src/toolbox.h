//------------------------------------------------------------------------------
// TOOLBOX
//------------------------------------------------------------------------------

#ifndef _TOOLBOX_H
#define _TOOLBOX_H

// Basic boolean type, for clearer types
typedef char bool;

#define true  1
#define false 0

// Macro to check the correct execution of a malloc() call
#define CHECK_MALLOC(ptr) assert(ptr != NULL);

//------------------------------------------------------------------------------

void printProgressMessage (char* const message);

#endif
