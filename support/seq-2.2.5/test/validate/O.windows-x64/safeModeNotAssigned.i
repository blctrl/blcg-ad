#line 1 "../safeModeNotAssigned.st"
/************************************************************************
*Copyright (c) 2010-2015 Helmholtz-Zentrum Berlin f. Materialien
                        und Energie GmbH, Germany (HZB)
This file is distributed subject to a Software License Agreement found
in the file LICENSE that is included with this distribution.
\*************************************************************************/
program safeModeNotAssignedTest

%%#include "../testSupport.h"

option +s;

int v = 0;



entry {
    seq_test_init(10*2);
}

ss ss1 {
    int i = 0;
    state idle {
        when (delay(0.1)) {
            testOk(v == i, "ss1: v=%d==%d=i", v, i);
            v++;
            i++;
        } state idle
        when (v==10) {
        } state done
    }
    state done {
        when (delay(0.2)) {
        } exit
    }
}

ss ss2 {
    state idle {
        int i;
        when (delay(0.1)) {
            if (i < 10) {
                testOk(v == 0, "ss2: v=%d==0", v);
            }
            i++;
        } state idle
    }
}

exit {
    seq_test_done();
}
