#line 1 "../version.st"
program versionTest

#line 1 "../../../include\\seq_release.h"


#line 4 "../version.st"
%%#include <stdlib.h>    /* for exit() */

entry {
/* conditional compilation in SNL code, should not be needed */
    printf("conditional compilation in\n");

    printf("seq version is 2.2 or greater\n");


#line 14 "../version.st"

/* conditional compilation in escaped C code, MAGIC and directives resolved by initial CPP pass */
%{

    printf("seq version is 2.2 or greater\n");


#line 22 "../version.st"
}%

/* conditional compilation in escaped C code */
%%/* an #include "seq_release.h" is not needed when testing inside escaped C code */
%%#if 2002005 >= 2002000
%%    printf("seq version is 2.2 or greater\n");
%%#else
%%    printf("seq version is 2.1 or less\n");
%%#endif
}

ss x { state x { when (delay(1)) {exit(0);} exit } }
