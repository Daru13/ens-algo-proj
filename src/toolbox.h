//------------------------------------------------------------------------------
// TOOLBOX
//------------------------------------------------------------------------------

#ifndef _TOOLBOX_H
#define _TOOLBOX_H

// Macro to check the correct execution of a malloc() call
#define CHECK_MALLOC(ptr) assert(ptr != NULL);

//------------------------------------------------------------------------------

void printProgressMessage (char* const message);

#endif
