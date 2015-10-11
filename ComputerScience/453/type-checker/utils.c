/*
 * General utility functions. This includes functions to write debug statements and make assertions.
 * This assertions and debug statements can be turned off by changing the debug reporting level
 * using the setDebugReporting function.
 *
 * Created by Christopher Chapline.
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "utils.h"

#ifndef GLOBAL_DEBUG_LEVEL
#define GLOBAL_DEBUG_LEVEL

int globalDebugLevel;

#endif

#ifndef DEBUG_OUTPUT_STREAM
#define DEBUG_OUTPUT_STREAM

FILE *debugOutputStream;

#endif

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
void setDebuggingLevel( int debugLevel ) {
    globalDebugLevel = debugLevel;
    debug( E_INFO, "Debug level set to 0x%X\n", globalDebugLevel );
}

/*
 * Sets the stream that debug output will be written to.
 *
 * Arguments:
 * outputStream -- An output stream that debug functions will write their output too
 *
 */
void setDebugOutputStream( FILE *outputStream ) {
    if( ! outputStream ) {
        debugOutputStream = outputStream;
    }
}

/*
 * Prints debug information if the debug flag is enabled and if the option has been
 * passed into the command line using either --debug or -d.
 *
 * Arguments:
 * debugType -- The type of error message that is being reported
 * format    -- The format for printing. This follows the same rules as printf
 * VA_ARGS   -- The items that will be printed by printf
 */
void debug( int debugType, const char *format, ... ) {
    if( (globalDebugLevel & debugType) == debugType ) {
        va_list args;
        va_start(args, format);
        vfprintf(debugOutputStream ? debugOutputStream : stdout, format, args);
        va_end(args);
    }
}

/*
 * Prints debug information if the debug flag is enabled and if the option has been
 * passed into the command line using either --debug or -d.
 *
 * Arguments:
 * debugType -- The type of error message that is being reported
 * format    -- The format for printing. This follows the same rules as printf
 * args      -- The args to print
 */
void vdebug( int debugType, const char *format, va_list args ) {
    if( (globalDebugLevel & debugType) == debugType ) {
        vfprintf(debugOutputStream, format, args);
    }
}

/*
 * Provides the concrete implementation of the assertion testing and output. On test failure, this
 * will print a message in the following format:
 *
 * <srcFilename>:<lineNumber> <functionName>: <msg>
 *
 * where <msg> is the msgFormat with replacements parsed in a printf-style manner.
 *
 * Arguments:
 * assertionValue -- If this is false, the assertion output will be printed. Otherwise, nothing will
 *                   happen.
 * srcFilename    -- The name of the filename that this assertion is in.
 * lineNumber     -- The line number that this assertion is on.
 * functionName   -- The name of the function that this assertion is contained in.
 * msgFormat      -- A printf-style message format
 * VA_ARGS        -- A variable-length list of replacements for the printf-style message format.
 */
void __assert( int assertionValue, const char *srcFilename, int lineNumber,
        const char *functionName, char *msgFormat, ... ) {
    if( (globalDebugLevel & E_ERROR) != E_ERROR ) return;

    if( ! assertionValue ) {
        if( debugOutputStream ) {
            fprintf( debugOutputStream, "%s:%d %s: ", srcFilename, lineNumber, functionName );

            va_list args;
            va_start(args, msgFormat);
            vfprintf(debugOutputStream, msgFormat, args);
            va_end(args);
        } else {
            fprintf( stdout, "%s:%d %s: ", srcFilename, lineNumber, functionName );

            va_list args;
            va_start(args, msgFormat);
            vfprintf(stdout, msgFormat, args);
            va_end(args);
        }
    }
}
