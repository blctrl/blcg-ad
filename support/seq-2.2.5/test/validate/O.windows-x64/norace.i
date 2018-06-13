#line 1 "../norace.st"
/************************************************************************
*Copyright (c) 2010-2015 Helmholtz-Zentrum Berlin f. Materialien
                        und Energie GmbH, Germany (HZB)
This file is distributed subject to a Software License Agreement found
in the file LICENSE that is included with this distribution.
\*************************************************************************/
program noraceTest

option +s;

#line 1 "d:\\epics\\ad\\seq-2.2.5\\test\\validate\\raceCommon.st"
/************************************************************************
*Copyright (c) 2010-2015 Helmholtz-Zentrum Berlin f. Materialien
                        und Energie GmbH, Germany (HZB)
This file is distributed subject to a Software License Agreement found
in the file LICENSE that is included with this distribution.
\*************************************************************************/
%%#include "../testSupport.h"

string x;
assign x to "x";
monitor x;

string y;
assign y to "";



entry {
    seq_test_init(10);
    testDiag("This test will run for %d seconds...", 10);
}

ss race1 {
    string z;
    assign z to "";
    state init {
        when () {
            x[0] = 0;
        } state wait
    }
    state wait {
        when (delay(0.001)) {
            int j;
            for (j=0; j <1000; j++) {
                int i;
                for (i=0; i<MAX_STRING_SIZE; i++) {
                    /* sched_yield(); */
                    if (!x[i]) break;
                    if (x[i] != x[0]) {
                        /* this should never be executed */
                        testFail("x[0]=%c!=%c=x[%d]",x[0],x[i],i);
                        break;
                    }
                }
            }
        } state wait
        when ( (x[0] && x[1] && x[0] != x[1])
            || (y[0] && y[1] && y[0] != y[1])
            || (z[0] && z[1] && z[0] != z[1]) ) {
            /* this should never be executed */
            testFail("%c/%c",x[0],x[1]);
        } state wait
    }
}

ss race2 {
    state init {
        when () {
            x[0] = 0;
        } state wait
    }
    state wait {
        when (delay(0.001)) {
            int j;
            for (j=0; j <1000; j++) {
                int i;
                for (i=0; i<MAX_STRING_SIZE; i++) {
                    /* sched_yield(); */
                    if (!x[i]) break;
                    if (x[i] != x[0]) {
                        /* this should never be executed */
                        testFail("x[0]=%c!=%c=x[%d]",x[0],x[i],i);
                        break;
                    }
                }
            }
        } state wait
        when (delay(0.002)) {
            int j;
            for (j=0; j <1000; j++) {
                int i;
                for (i=0; i<MAX_STRING_SIZE; i++) {
                    /* sched_yield(); */
                    if (!x[i]) break;
                    if (x[i] != x[0]) {
                        /* this should never be executed */
                        testFail("x[0]=%c!=%c=x[%d]",x[0],x[i],i);
                        break;
                    }
                }
            }
        } state wait
        when (x[0] && x[1] && x[0] != x[1]) {
            /* this should never be executed */
            testFail("x[0]=%c!=%c=x[1]",x[0],x[1]);
        } state wait
    }
}

ss race3 {
    string z;
    assign z to "";
    state init {
        when () {
            x[0] = 0;
        } state wait
    }
    state wait {
        when (delay(0.001)) {
            int j;
            for (j=0; j <1000; j++) {
                int i;
                for (i=0; i<MAX_STRING_SIZE; i++) {
                    /* sched_yield(); */
                    if (!x[i]) break;
                    if (x[i] != x[0]) {
                        /* this should never be executed */
                        testFail("x[0]=%c!=%c=x[%d]",x[0],x[i],i);
                        break;
                    }
                }
            }
        } state wait
        when (x[0] && x[1] && x[0] != x[1]) {
            /* this should never be executed */
            testFail("x[0]=%c!=%c=x[1]",x[0],x[1]);
        } state wait
    }
}

ss stop {
    int seconds = 10;
    state count {
        when (seconds == 0) {
        } exit
        when (delay(1.0)) {
            seconds -= 1;
            testPass("%d seconds left", seconds);
        } state count
    }
}

exit {
    seq_test_done();
}
#line 12 "../norace.st"
