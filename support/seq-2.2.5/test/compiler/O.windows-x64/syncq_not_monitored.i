#line 1 "../syncq_not_monitored.st"
/************************************************************************
*Copyright (c) 2010-2015 Helmholtz-Zentrum Berlin f. Materialien
                        und Energie GmbH, Germany (HZB)
This file is distributed subject to a Software License Agreement found
in the file LICENSE that is included with this distribution.
\*************************************************************************/
program p

int x;
assign x;
syncq x 1; /* ok: not monitored */

#line 1 "d:\\epics\\ad\\seq-2.2.5\\test\\compiler\\simple.st"
/************************************************************************
*Copyright (c) 2010-2015 Helmholtz-Zentrum Berlin f. Materialien
                        und Energie GmbH, Germany (HZB)
This file is distributed subject to a Software License Agreement found
in the file LICENSE that is included with this distribution.
\*************************************************************************/
ss simple {
    state simple {
        when () {} exit
    }
}
#line 14 "../syncq_not_monitored.st"
