#line 1 "../cppComment.st"
program cppCommentTest // ("parm=bla

// #include "../na.st"
#line 1 ".\\../simple.st"
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
#line 5 "../cppComment.st"

exit {
    // intt x;
    int x = 0+0-0*0; // funny initialiser
    // return x;
    printf("%d", x);
}
