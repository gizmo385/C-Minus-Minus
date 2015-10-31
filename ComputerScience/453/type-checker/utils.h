#include <stdio.h>

#ifndef UTILS_H
#define UTILS_H

/* Debug levels */
#define E_FATAL     1
#define E_ERROR     2
#define E_WARNING   4
#define E_DEBUG     8
#define E_INFO      16
#define E_ALL       E_FATAL | E_ERROR | E_WARNING | E_DEBUG | E_INFO

/* Assert macros */
#define assertTrue(assertionValue, msgFormat, ...) __assert((assertionValue), __FILE__, __LINE__, __func__, msgFormat,  ##__VA_ARGS__ )
#define assertFalse(assertionValue, msgFormat, ...) __assert(((assertionValue) == 0), __FILE__, __LINE__, __func__, msgFormat,  ##__VA_ARGS__ )
#define assertNull(assertionValue, msgFormat, ...) __assert(((assertionValue) == NULL), __FILE__, __LINE__, __func__, msgFormat,  ##__VA_ARGS__ )
#define assertNotNull(assertionValue, msgFormat, ...) __assert(((assertionValue) != NULL), __FILE__, __LINE__, __func__, msgFormat,  ##__VA_ARGS__ )

extern int globalDebugLevel;
extern FILE *debugOutputStream;

/*
 * Prints debug information if the debug flag is enabled and if the option has been
 * passed into the command line using either --debug or -d.
 *
 * Arguments:
 * debugType -- The type of error message that is being reported
 * format    -- The format for printing. This follows the same rules as printf
 * VA_ARGS   -- The items that will be printed by printf
 */
extern void debug( int debugType, const char *format, ... );

/*
 * Sets the types of debug messages that will be printed.
 *
 * Arguments:
 * debugLevel -- This is an integer that is formed by taking the defined debug levels that you wish
 *               to enable and performing a bitwise OR on all of them. An example is provided below.
 *
 * Enabling E_FATAL, E_ERROR, and E_DEBUG:
 * setDebugReporting( E_FATAL | E_ERROR | E_DEBUG );
 */
extern void setDebuggingLevel( int debugLevel );

/*
 * Sets the stream that debug output will be written to.
 *
 * Arguments:
 * outputStream -- An output stream that debug functions will write their output too
 *
 */
extern void setDebugOutputStream( FILE *outputStream );
extern void __assert( int assertionValue, const char *srcFilename, int lineNumber,
        const char *functionName, char *msgFormat, ... );

#endif
