//------------------------------------------------------------------------------
// TOOLBOX
//------------------------------------------------------------------------------
// Various tools used in the projet
//------------------------------------------------------------------------------

#include <stdio.h>

// Terminal's display formatting strings
#define TERM_NORMAL_FORMAT 	"\x1B[0m"
#define TERM_ORANGE_TEXT	"\x1B[0;33m"

void printProgressMessage (char* const message)
{
	printf("%s%s%s", TERM_ORANGE_TEXT, message, TERM_NORMAL_FORMAT);
}
