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

FILE *debugOutputStream = NULL;

#endif

void setDebuggingLevel( int debugLevel ) {
    globalDebugLevel = debugLevel;
    debug( E_INFO, "Debug level set to 0x%X\n", globalDebugLevel );
}

void setDebugOutputStream( FILE *outputStream ) {
    if(outputStream ) {
        debugOutputStream = outputStream;
    }
}

void debug( int debugType, const char *format, ... ) {
    if( (globalDebugLevel & debugType) == debugType ) {
        va_list args;
        va_start(args, format);
        vfprintf(debugOutputStream ? debugOutputStream : stdout, format, args);
        va_end(args);
    }
}

void vdebug( int debugType, const char *format, va_list args ) {
    if( (globalDebugLevel & debugType) == debugType ) {
        vfprintf(debugOutputStream, format, args);
    }
}

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
