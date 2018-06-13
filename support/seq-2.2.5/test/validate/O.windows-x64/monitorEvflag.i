#line 1 "../monitorEvflag.st"
program monitorEvflagTest

option +s;

%%#include "../testSupport.h"
%%#include <stdio.h>





int requested;
assign requested to "requested";

int actual;
assign actual to "actual";
monitor actual;

evflag actualChanged;
sync actual actualChanged;

entry {
    seq_test_init(1);
}

ss monitorEvflagTest {
    int cycleCount = 1;
    int error = 0;
    state init {
        when (delay(1.0)) {
            efClear(actualChanged);
        } state makeRequest
    }
    state makeRequest {
        when (error) {
        } exit
        when (cycleCount > (20*500)) {
            testPass("no error in %d cycles", (20*500));
        } exit
        when (cycleCount <= (20*500)) {
            requested = cycleCount;
            pvPut(requested);
        } state waitForActualToEqualRequested
    }
    state waitForActualToEqualRequested {
        when (efTestAndClear(actualChanged) && actual == requested) {
            if (actual != requested) {
                testFail("requested(%d)!=actual(%d)", requested, actual);
                error = TRUE;
            } else if (cycleCount % 500 == 0) {
                testDiag("passed %d cycles", cycleCount);
            }
            cycleCount++;
        } state makeRequest
        when (delay(1.0)) {
            testFail("timeout in cycle %d/%d (requested=%d, actual=%d)",
                cycleCount + 1, (20*500), requested, actual);
        } exit
    }
}

exit {
    seq_test_done();
}
