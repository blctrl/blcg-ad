/*
** 2000-05-29
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** Driver template for the LEMON parser generator.
**
** The "lemon" program processes an LALR(1) input grammar file, then uses
** this template to construct a parser.  The "lemon" program inserts text
** at each "%%" line.  Also, any "P-a-r-s-e" identifer prefix (without the
** interstitial "-" characters) contained in this template is changed into
** the value of the %name directive from the grammar.  Otherwise, the content
** of this template is copied straight through into the generate parser
** source file.
**
** The following is the concatenation of all %include directives from the
** input grammar file:
*/
#include <stdio.h>
/************ Begin %include sections from the grammar ************************/
#line 10 "../snl.lem"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "node.h"
#include "var_types.h"
#include "main.h"
#include "parser.h"

#define NIL (Node *)0
#line 40 "./snl.c"
/**************** End of %include directives **********************************/
/* These constants specify the various numeric values for terminal symbols
** in a format understandable to "makeheaders".  This section is blank unless
** "lemon" is run with the "-m" command-line option.
***************** Begin makeheaders token definitions *************************/
/**************** End makeheaders token definitions ***************************/

/* The next sections is a series of control #defines.
** various aspects of the generated parser.
**    YYCODETYPE         is the data type used to store the integer codes
**                       that represent terminal and non-terminal symbols.
**                       "unsigned char" is used if there are fewer than
**                       256 symbols.  Larger types otherwise.
**    YYNOCODE           is a number of type YYCODETYPE that is not used for
**                       any terminal or nonterminal symbol.
**    YYFALLBACK         If defined, this indicates that one or more tokens
**                       (also known as: "terminal symbols") have fall-back
**                       values which should be used if the original symbol
**                       would not parse.  This permits keywords to sometimes
**                       be used as identifiers, for example.
**    YYACTIONTYPE       is the data type used for "action codes" - numbers
**                       that indicate what to do in response to the next
**                       token.
**    snlParserTOKENTYPE     is the data type used for minor type for terminal
**                       symbols.  Background: A "minor type" is a semantic
**                       value associated with a terminal or non-terminal
**                       symbols.  For example, for an "ID" terminal symbol,
**                       the minor type might be the name of the identifier.
**                       Each non-terminal can have a different minor type.
**                       Terminal symbols all have the same minor type, though.
**                       This macros defines the minor type for terminal 
**                       symbols.
**    YYMINORTYPE        is the data type used for all minor types.
**                       This is typically a union of many types, one of
**                       which is snlParserTOKENTYPE.  The entry in the union
**                       for terminal symbols is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.  If
**                       zero the stack is dynamically sized using realloc()
**    snlParserARG_SDECL     A static variable declaration for the %extra_argument
**    snlParserARG_PDECL     A parameter declaration for the %extra_argument
**    snlParserARG_STORE     Code to store %extra_argument into yypParser
**    snlParserARG_FETCH     Code to extract %extra_argument from yypParser
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YY_MAX_SHIFT       Maximum value for shift actions
**    YY_MIN_SHIFTREDUCE Minimum value for shift-reduce actions
**    YY_MAX_SHIFTREDUCE Maximum value for shift-reduce actions
**    YY_MIN_REDUCE      Maximum value for reduce actions
**    YY_ERROR_ACTION    The yy_action[] code for syntax error
**    YY_ACCEPT_ACTION   The yy_action[] code for accept
**    YY_NO_ACTION       The yy_action[] code for no-op
*/
#ifndef INTERFACE
# define INTERFACE 1
#endif
/************* Begin control #defines *****************************************/
#define YYCODETYPE unsigned char
#define YYNOCODE 160
#define YYACTIONTYPE unsigned short int
#define snlParserTOKENTYPE  Token 
typedef union {
  int yyinit;
  snlParserTOKENTYPE yy0;
  enum prim_type_tag yy6;
  Type* yy149;
  Node* yy255;
  Token yy270;
  int yy319;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 0
#endif
#define snlParserARG_SDECL  Node **presult ;
#define snlParserARG_PDECL , Node **presult 
#define snlParserARG_FETCH  Node **presult  = yypParser->presult 
#define snlParserARG_STORE yypParser->presult  = presult 
#define YYERRORSYMBOL 95
#define YYERRSYMDT yy319
#define YYNSTATE             254
#define YYNRULE              224
#define YY_MAX_SHIFT         253
#define YY_MIN_SHIFTREDUCE   402
#define YY_MAX_SHIFTREDUCE   625
#define YY_MIN_REDUCE        626
#define YY_MAX_REDUCE        849
#define YY_ERROR_ACTION      850
#define YY_ACCEPT_ACTION     851
#define YY_NO_ACTION         852
/************* End control #defines *******************************************/

/* Define the yytestcase() macro to be a no-op if is not already defined
** otherwise.
**
** Applications can choose to define yytestcase() in the %include section
** to a macro that can assist in verifying code coverage.  For production
** code the yytestcase() macro should be turned off.  But it is useful
** for testing.
*/
#ifndef yytestcase
# define yytestcase(X)
#endif


/* Next are the tables used to determine what action to take based on the
** current state and lookahead token.  These tables are used to implement
** functions that take a state number and lookahead value and return an
** action integer.  
**
** Suppose the action integer is N.  Then the action is determined as
** follows
**
**   0 <= N <= YY_MAX_SHIFT             Shift N.  That is, push the lookahead
**                                      token onto the stack and goto state N.
**
**   N between YY_MIN_SHIFTREDUCE       Shift to an arbitrary state then
**     and YY_MAX_SHIFTREDUCE           reduce by rule N-YY_MIN_SHIFTREDUCE.
**
**   N between YY_MIN_REDUCE            Reduce by rule N-YY_MIN_REDUCE
**     and YY_MAX_REDUCE
**
**   N == YY_ERROR_ACTION               A syntax error has occurred.
**
**   N == YY_ACCEPT_ACTION              The parser accepts its input.
**
**   N == YY_NO_ACTION                  No such action.  Denotes unused
**                                      slots in the yy_action[] table.
**
** The action table is constructed as a single large table named yy_action[].
** Given state S and lookahead X, the action is computed as either:
**
**    (A)   N = yy_action[ yy_shift_ofst[S] + X ]
**    (B)   N = yy_default[S]
**
** The (A) formula is preferred.  The B formula is used instead if:
**    (1)  The yy_shift_ofst[S]+X value is out of range, or
**    (2)  yy_lookahead[yy_shift_ofst[S]+X] is not equal to X, or
**    (3)  yy_shift_ofst[S] equal YY_SHIFT_USE_DFLT.
** (Implementation note: YY_SHIFT_USE_DFLT is chosen so that
** YY_SHIFT_USE_DFLT+X will be out of range for all possible lookaheads X.
** Hence only tests (1) and (2) need to be evaluated.)
**
** The formulas above are for computing the action when the lookahead is
** a terminal symbol.  If the lookahead is a non-terminal (as occurs after
** a reduce action) then the yy_reduce_ofst[] array is used in place of
** the yy_shift_ofst[] array and YY_REDUCE_USE_DFLT is used in place of
** YY_SHIFT_USE_DFLT.
**
** The following are the tables generated in this section:
**
**  yy_action[]        A single table containing all actions.
**  yy_lookahead[]     A table containing the lookahead for each entry in
**                     yy_action.  Used to detect hash collisions.
**  yy_shift_ofst[]    For each state, the offset into yy_action for
**                     shifting terminals.
**  yy_reduce_ofst[]   For each state, the offset into yy_action for
**                     shifting non-terminals after a reduce.
**  yy_default[]       Default action for each state.
**
*********** Begin parsing tables **********************************************/
#define YY_ACTTAB_COUNT (1382)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    61,   60,   59,   58,   57,   56,   55,   54,   53,   52,
 /*    10 */    51,   63,   62,   71,   70,   67,   66,   65,   75,   74,
 /*    20 */    77,   76,   73,   72,   69,   68,   80,   81,   79,   78,
 /*    30 */    64,  114,  135,  565,  566,   29,  497,   82,   33,  139,
 /*    40 */   140,   61,   60,   59,   58,   57,   56,   55,   54,   53,
 /*    50 */    52,   51,   63,  117,   71,   70,   67,   66,   65,   75,
 /*    60 */    74,   77,   76,   73,   72,   69,   68,   80,   81,   79,
 /*    70 */    78,   64,  246,   19,  565,  566,  446,  851,   82,   33,
 /*    80 */   139,  140,   79,   78,   64,  443,  107,  565,  566,  253,
 /*    90 */   126,   82,   33,  139,  140,  218,  562,  506,   61,   60,
 /*   100 */    59,   58,   57,   56,   55,   54,   53,   52,   51,   63,
 /*   110 */   212,   71,   70,   67,   66,   65,   75,   74,   77,   76,
 /*   120 */    73,   72,   69,   68,   80,   81,   79,   78,   64,  217,
 /*   130 */    45,  565,  566,   88,  426,   82,   33,  139,  140,   48,
 /*   140 */    47,   46,  246,   13,   44,   43,   42,   41,   49,  196,
 /*   150 */   446,   11,  497,  497,  124,  212,  435,   36,  246,   18,
 /*   160 */   109,   87,  206,  453,  456,  492,  246,  554,  483,  108,
 /*   170 */   116,  466,  467,  468,  469,  195,  474,  475,  476,  477,
 /*   180 */   478,  479,  480,  481,  482,  484,  485,  252,  249,  248,
 /*   190 */   247,  614,  565,  566,  137,  243,   82,   33,  139,  140,
 /*   200 */   133,  131,  127,  435,  555,  614,  105,  466,  467,  468,
 /*   210 */   469,  195,  474,  475,  476,  477,  478,  479,  480,  481,
 /*   220 */   482,  484,  485,  252,  251,  248,  247,  112,  447,   36,
 /*   230 */   122,  137,  246,   19,  207,  453,  456,  133,  131,  127,
 /*   240 */   483,  621,  623,  105,  466,  467,  468,  469,  195,  474,
 /*   250 */   475,  476,  477,  478,  479,  480,  481,  482,  484,  485,
 /*   260 */   252,  249,  248,  247,  112,  137,  446,  122,  210,  226,
 /*   270 */   532,  133,  131,  127,  205,  442,   86,  105,  466,  467,
 /*   280 */   468,  469,  195,  474,  475,  476,  477,  478,  479,  480,
 /*   290 */   481,  482,  484,  485,  252,  249,  248,  247,   45,   36,
 /*   300 */   217,  438,  533,   95,  208,  453,  456,   48,   47,   46,
 /*   310 */   483,   35,   44,   43,   42,   41,   49,  246,   19,   11,
 /*   320 */   246,    5,  498,  497,  435,   36,  435,  145,  145,  535,
 /*   330 */   661,  454,  456,  624,  146,  554,  483,  447,  436,   34,
 /*   340 */   115,  466,  467,  468,  469,  195,  474,  475,  476,  477,
 /*   350 */   478,  479,  480,  481,  482,  484,  485,  252,  249,  248,
 /*   360 */   247,  236,  215,  243,  616,  240,  239,   25,  234,  504,
 /*   370 */   232,  230,  555,  614,  623,  514,   63,  213,   71,   70,
 /*   380 */    67,   66,   65,   75,   74,   77,   76,   73,   72,   69,
 /*   390 */    68,   80,   81,   79,   78,   64,  224,   45,  565,  566,
 /*   400 */    99,  209,   82,   33,  139,  140,   48,   47,   46,  145,
 /*   410 */    88,   44,   43,   42,   41,   49,  446,  525,   11,  246,
 /*   420 */    13,  529,  111,  435,  205,  442,  107,  145,  470,  471,
 /*   430 */   472,  473,  246,   19,  554,  438,  505,  130,   87,  449,
 /*   440 */   466,  467,  468,  469,  195,  474,  475,  476,  477,  478,
 /*   450 */   479,  480,  481,  482,  484,  485,  252,  249,  248,  247,
 /*   460 */   236,  438,  243,  132,  240,  239,   25,  234,  527,  232,
 /*   470 */   230,  555,  614,  623,   70,   67,   66,   65,   75,   74,
 /*   480 */    77,   76,   73,   72,   69,   68,   80,   81,   79,   78,
 /*   490 */    64,  626,  226,  565,  566,  124,  436,   82,   33,  139,
 /*   500 */   140,  105,  466,  467,  468,  469,  195,  474,  475,  476,
 /*   510 */   477,  478,  479,  480,  481,  482,  484,  485,  252,  249,
 /*   520 */   248,  247,   66,   65,   75,   74,   77,   76,   73,   72,
 /*   530 */    69,   68,   80,   81,   79,   78,   64,  623,   98,  565,
 /*   540 */   566,  530,  219,   82,   33,  139,  140,  502,  503,  125,
 /*   550 */   466,  467,  468,  469,  195,  474,  475,  476,  477,  478,
 /*   560 */   479,  480,  481,  482,  484,  485,  252,  251,  248,  247,
 /*   570 */   457,  446,  497,  497,  138,  104,  446,   36,  497,  458,
 /*   580 */    29,  118,  206,  453,  456,  623,  119,  618,  483,  108,
 /*   590 */   117,  246,   18,  434,  199,  116,  101,  466,  467,  468,
 /*   600 */   469,  195,  474,  475,  476,  477,  478,  479,  480,  481,
 /*   610 */   482,  484,  485,  252,  249,  248,  247,  246,   19,  246,
 /*   620 */   614,  246,   19,  223,  461,  136,   28,  145,  424,  430,
 /*   630 */   220,  624,  623,   67,   66,   65,   75,   74,   77,   76,
 /*   640 */    73,   72,   69,   68,   80,   81,   79,   78,   64,  441,
 /*   650 */   128,  565,  566,   85,  624,   82,   33,  139,  140,  466,
 /*   660 */   467,  468,  469,  195,  474,  475,  476,  477,  478,  479,
 /*   670 */   480,  481,  482,  484,  485,  252,  249,  248,  247,   80,
 /*   680 */    81,   79,   78,   64,  129,  102,  565,  566,  428,  221,
 /*   690 */    82,   33,  139,  140,  623,  466,  467,  468,  469,  195,
 /*   700 */   474,  475,  476,  477,  478,  479,  480,  481,  482,  484,
 /*   710 */   485,  252,  249,  248,  247,   65,   75,   74,   77,   76,
 /*   720 */    73,   72,   69,   68,   80,   81,   79,   78,   64,  103,
 /*   730 */   222,  565,  566,  425,  134,   82,   33,  139,  140,   75,
 /*   740 */    74,   77,   76,   73,   72,   69,   68,   80,   81,   79,
 /*   750 */    78,   64,   96,  459,  565,  566,  564,  615,   82,   33,
 /*   760 */   139,  140,   77,   76,   73,   72,   69,   68,   80,   81,
 /*   770 */    79,   78,   64,   17,   45,  565,  566,  446,  563,   82,
 /*   780 */    33,  139,  140,   48,   47,   46,  446,   97,   44,   43,
 /*   790 */    42,   41,   49,  446,  556,   12,  118,  460,   45,   28,
 /*   800 */   435,  250,  121,  119,   20,  147,  557,   48,   47,   46,
 /*   810 */   446,  554,   44,   43,   42,   41,   49,  464,    2,   11,
 /*   820 */   109,  556,  203,  165,  435,    3,  556,  446,   21,  497,
 /*   830 */     7,  446,   14,  557,   15,  554,   83,  120,  557,  243,
 /*   840 */    89,  110,    1,   45,  403,  531,  115,  242,  555,  614,
 /*   850 */   150,  483,   48,   47,   46,  150,  123,   44,   43,   42,
 /*   860 */    41,   49,  556,  243,   11,    9,   16,   45,  516,  435,
 /*   870 */   202,   83,  555,  614,  557,  197,   48,   47,   46,  501,
 /*   880 */   554,   44,   43,   42,   41,   49,  214,  143,   10,  106,
 /*   890 */   216,  150,  433,  435,  405,  407,  408,  409,  410,  411,
 /*   900 */   412,  413,  414,  415,  554,  211,  534,  559,  243,  560,
 /*   910 */   558,  237,   37,   50,  432,   50,   83,  555,  614,  431,
 /*   920 */   429,  422,  423,  483,   69,   68,   80,   81,   79,   78,
 /*   930 */    64,  421,  243,  565,  566,  144,  225,   82,   33,  139,
 /*   940 */   140,  555,  614,    6,  556,   44,   43,   42,   41,   49,
 /*   950 */    22,   23,   11,  100,   24,  622,  557,  435,  552,  519,
 /*   960 */   520,  521,  522,  523,  524,    8,   94,  538,  554,  539,
 /*   970 */    30,   31,  499,  150,   83,  500,   38,  483,   27,   40,
 /*   980 */    38,   27,  509,  510,  511,  512,  513,  483,  227,  235,
 /*   990 */   545,  483,  556,  237,  544,  543,  243,  517,  542,   38,
 /*  1000 */   546,   26,  619,   35,  557,  555,  614,    4,  536,   84,
 /*  1010 */   483,   32,  246,    5,  448,  113,  507,  515,  435,  440,
 /*  1020 */    27,  150,  556,   39,  489,  245,  488,  487,  617,  141,
 /*  1030 */   546,   34,  628,  547,  557,  486,  231,  622,  556,  142,
 /*  1040 */   540,  237,  551,  628,  628,  628,  546,  628,   94,  556,
 /*  1050 */   557,  150,  628,  418,  419,  420,  416,  546,  628,  483,
 /*  1060 */   628,  557,  628,  547,   90,  556,  231,  150,  628,  628,
 /*  1070 */   553,  237,  551,  546,  628,  483,  628,  557,  150,  547,
 /*  1080 */   628,  628,  231,  628,  620,  628,  550,  237,  551,  628,
 /*  1090 */   547,  556,  628,  231,  150,  556,  628,  549,  237,  551,
 /*  1100 */   556,  628,  628,  557,  556,  628,  547,  557,  628,  231,
 /*  1110 */    89,  628,  557,  233,  237,  551,  557,  241,  464,   89,
 /*  1120 */   150,  483,  628,  204,  165,  556,  244,  628,  628,  150,
 /*  1130 */   483,  628,  628,  150,  556,  556,  628,  557,  556,  628,
 /*  1140 */   202,  628,   92,  628,  556,  628,  557,  557,  228,  202,
 /*  1150 */   557,  628,   93,  237,  150,  628,  557,  435,  463,  556,
 /*  1160 */   556,  445,  628,  150,  165,  556,  628,  165,  556,  229,
 /*  1170 */    91,  557,  557,  150,  237,  556,  628,  557,  238,  556,
 /*  1180 */   557,  628,  628,  237,  628,  628,  556,  557,  152,  152,
 /*  1190 */   556,  557,  628,  198,  191,  556,  628,  183,  557,  628,
 /*  1200 */   628,  556,  557,  628,  184,  628,  628,  557,  185,  628,
 /*  1210 */   201,  200,  628,  557,  556,  186,  628,  556,  628,  187,
 /*  1220 */   556,  556,  556,  556,  188,  556,  557,  556,  556,  557,
 /*  1230 */   189,  628,  557,  557,  557,  557,  628,  557,  628,  557,
 /*  1240 */   557,  556,  628,  190,  628,  628,  182,  628,  556,  151,
 /*  1250 */   153,  154,  155,  557,  156,  628,  157,  158,  628,  628,
 /*  1260 */   557,  556,  556,  556,  556,  628,  628,  556,  556,  556,
 /*  1270 */   159,  556,  556,  557,  557,  557,  557,  160,  628,  557,
 /*  1280 */   557,  557,  556,  557,  557,  628,  628,  556,  628,  628,
 /*  1290 */   161,  162,  163,  166,  557,  556,  148,  192,  171,  557,
 /*  1300 */   170,  169,  556,  628,  628,  556,  556,  557,  556,  628,
 /*  1310 */   628,  178,  628,  628,  557,  556,  179,  557,  557,  628,
 /*  1320 */   557,  556,  628,  628,  168,  628,  556,  557,  556,  556,
 /*  1330 */   556,  167,  628,  557,  174,  175,  556,  172,  557,  556,
 /*  1340 */   557,  557,  557,  628,  173,  628,  628,  628,  557,  628,
 /*  1350 */   176,  557,  556,  628,  628,  177,  628,  193,  194,  180,
 /*  1360 */   628,  628,  628,  628,  557,  181,  628,  628,  149,  628,
 /*  1370 */   628,  628,  628,  628,  628,  628,  628,  628,  628,  628,
 /*  1380 */   628,  164,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     1,    2,    3,    4,    5,    6,    7,    8,    9,   10,
 /*    10 */    11,   12,   13,   14,   15,   16,   17,   18,   19,   20,
 /*    20 */    21,   22,   23,   24,   25,   26,   27,   28,   29,   30,
 /*    30 */    31,  116,  117,   34,   35,    1,  117,   38,   39,   40,
 /*    40 */    41,    1,    2,    3,    4,    5,    6,    7,    8,    9,
 /*    50 */    10,   11,   12,  134,   14,   15,   16,   17,   18,   19,
 /*    60 */    20,   21,   22,   23,   24,   25,   26,   27,   28,   29,
 /*    70 */    30,   31,   38,   39,   34,   35,  115,   96,   38,   39,
 /*    80 */    40,   41,   29,   30,   31,  124,  125,   34,   35,   43,
 /*    90 */   116,   38,   39,   40,   41,  121,   56,   49,    1,    2,
 /*   100 */     3,    4,    5,    6,    7,    8,    9,   10,   11,   12,
 /*   110 */    84,   14,   15,   16,   17,   18,   19,   20,   21,   22,
 /*   120 */    23,   24,   25,   26,   27,   28,   29,   30,   31,   81,
 /*   130 */    18,   34,   35,   29,  103,   38,   39,   40,   41,   27,
 /*   140 */    28,   29,   38,   39,   32,   33,   34,   35,   36,  118,
 /*   150 */   115,   39,  117,  117,   83,   84,   44,  122,   38,   39,
 /*   160 */   125,   57,  127,  128,  129,   45,   38,   55,  133,  134,
 /*   170 */   134,   59,   60,   61,   62,   63,   64,   65,   66,   67,
 /*   180 */    68,   69,   70,   71,   72,   73,   74,   75,   76,   77,
 /*   190 */    78,   93,   34,   35,   46,   83,   38,   39,   40,   41,
 /*   200 */    52,   53,   54,   44,   92,   93,   58,   59,   60,   61,
 /*   210 */    62,   63,   64,   65,   66,   67,   68,   69,   70,   71,
 /*   220 */    72,   73,   74,   75,   76,   77,   78,   79,  117,  122,
 /*   230 */    82,   46,   38,   39,  127,  128,  129,   52,   53,   54,
 /*   240 */   133,   47,   94,   58,   59,   60,   61,   62,   63,   64,
 /*   250 */    65,   66,   67,   68,   69,   70,   71,   72,   73,   74,
 /*   260 */    75,   76,   77,   78,   79,   46,  115,   82,   81,   80,
 /*   270 */    83,   52,   53,   54,  123,  124,  125,   58,   59,   60,
 /*   280 */    61,   62,   63,   64,   65,   66,   67,   68,   69,   70,
 /*   290 */    71,   72,   73,   74,   75,   76,   77,   78,   18,  122,
 /*   300 */    81,  117,   95,  119,  127,  128,  129,   27,   28,   29,
 /*   310 */   133,   29,   32,   33,   34,   35,   36,   38,   39,   39,
 /*   320 */    38,   39,  117,  117,   44,  122,   44,   48,   48,   49,
 /*   330 */    47,  128,  129,   50,  101,   55,  133,  117,   55,   57,
 /*   340 */   134,   59,   60,   61,   62,   63,   64,   65,   66,   67,
 /*   350 */    68,   69,   70,   71,   72,   73,   74,   75,   76,   77,
 /*   360 */    78,   81,  135,   83,  144,   85,   86,   87,   88,  136,
 /*   370 */    90,   91,   92,   93,   94,  140,   12,  101,   14,   15,
 /*   380 */    16,   17,   18,   19,   20,   21,   22,   23,   24,   25,
 /*   390 */    26,   27,   28,   29,   30,   31,  103,   18,   34,   35,
 /*   400 */   100,  103,   38,   39,   40,   41,   27,   28,   29,   48,
 /*   410 */    29,   32,   33,   34,   35,   36,  115,  144,   39,   38,
 /*   420 */    39,  145,  144,   44,  123,  124,  125,   48,   59,   60,
 /*   430 */    61,   62,   38,   39,   55,  117,  136,  119,   57,   45,
 /*   440 */    59,   60,   61,   62,   63,   64,   65,   66,   67,   68,
 /*   450 */    69,   70,   71,   72,   73,   74,   75,   76,   77,   78,
 /*   460 */    81,  117,   83,  119,   85,   86,   87,   88,  144,   90,
 /*   470 */    91,   92,   93,   94,   15,   16,   17,   18,   19,   20,
 /*   480 */    21,   22,   23,   24,   25,   26,   27,   28,   29,   30,
 /*   490 */    31,    0,   80,   34,   35,   83,   55,   38,   39,   40,
 /*   500 */    41,   58,   59,   60,   61,   62,   63,   64,   65,   66,
 /*   510 */    67,   68,   69,   70,   71,   72,   73,   74,   75,   76,
 /*   520 */    77,   78,   17,   18,   19,   20,   21,   22,   23,   24,
 /*   530 */    25,   26,   27,   28,   29,   30,   31,   94,  142,   34,
 /*   540 */    35,  145,  121,   38,   39,   40,   41,   27,   28,  120,
 /*   550 */    59,   60,   61,   62,   63,   64,   65,   66,   67,   68,
 /*   560 */    69,   70,   71,   72,   73,   74,   75,   76,   77,   78,
 /*   570 */    47,  115,  117,  117,   51,   48,  115,  122,  117,  115,
 /*   580 */     1,  125,  127,  128,  129,   94,  125,   49,  133,  134,
 /*   590 */   134,   38,   39,   44,  130,  134,  115,   59,   60,   61,
 /*   600 */    62,   63,   64,   65,   66,   67,   68,   69,   70,   71,
 /*   610 */    72,   73,   74,   75,   76,   77,   78,   38,   39,   38,
 /*   620 */    93,   38,   39,   49,   49,   51,   51,   48,   47,   47,
 /*   630 */    95,   50,   94,   16,   17,   18,   19,   20,   21,   22,
 /*   640 */    23,   24,   25,   26,   27,   28,   29,   30,   31,   47,
 /*   650 */   120,   34,   35,   51,   50,   38,   39,   40,   41,   59,
 /*   660 */    60,   61,   62,   63,   64,   65,   66,   67,   68,   69,
 /*   670 */    70,   71,   72,   73,   74,   75,   76,   77,   78,   27,
 /*   680 */    28,   29,   30,   31,  116,  115,   34,   35,   47,   95,
 /*   690 */    38,   39,   40,   41,   94,   59,   60,   61,   62,   63,
 /*   700 */    64,   65,   66,   67,   68,   69,   70,   71,   72,   73,
 /*   710 */    74,   75,   76,   77,   78,   18,   19,   20,   21,   22,
 /*   720 */    23,   24,   25,   26,   27,   28,   29,   30,   31,  115,
 /*   730 */   103,   34,   35,  103,  116,   38,   39,   40,   41,   19,
 /*   740 */    20,   21,   22,   23,   24,   25,   26,   27,   28,   29,
 /*   750 */    30,   31,  115,  115,   34,   35,  155,   44,   38,   39,
 /*   760 */    40,   41,   21,   22,   23,   24,   25,   26,   27,   28,
 /*   770 */    29,   30,   31,   48,   18,   34,   35,  115,  155,   38,
 /*   780 */    39,   40,   41,   27,   28,   29,  115,  125,   32,   33,
 /*   790 */    34,   35,   36,  115,  103,   39,  125,   49,   18,   51,
 /*   800 */    44,  156,   39,  125,   48,   97,  115,   27,   28,   29,
 /*   810 */   115,   55,   32,   33,   34,   35,   36,  126,  141,   39,
 /*   820 */   125,  103,  131,  132,   44,  137,  103,  115,   48,  117,
 /*   830 */    45,  115,  148,  115,  102,   55,   51,  125,  115,   83,
 /*   840 */   122,  125,   98,   18,   45,   44,  134,  129,   92,   93,
 /*   850 */   132,  133,   27,   28,   29,  132,   45,   32,   33,   34,
 /*   860 */    35,   36,  103,   83,   39,   45,   39,   18,   49,   44,
 /*   870 */   152,   51,   92,   93,  115,  152,   27,   28,   29,   47,
 /*   880 */    55,   32,   33,   34,   35,   36,   44,   48,   39,   99,
 /*   890 */    44,  132,   47,   44,  104,  105,  106,  107,  108,  109,
 /*   900 */   110,  111,  112,  113,   55,  146,  147,   45,   83,   45,
 /*   910 */    45,  152,  122,   51,   47,   51,   51,   92,   93,   47,
 /*   920 */    47,   47,   47,  133,   25,   26,   27,   28,   29,   30,
 /*   930 */    31,   47,   83,   34,   35,   48,   44,   38,   39,   40,
 /*   940 */    41,   92,   93,   45,  103,   32,   33,   34,   35,   36,
 /*   950 */    47,   47,   39,   99,   39,  111,  115,   44,   47,  105,
 /*   960 */   106,  107,  108,  109,  110,   89,  122,  109,   55,  111,
 /*   970 */    39,   39,   45,  132,   51,   45,  122,  133,   51,   45,
 /*   980 */   122,   51,  105,  106,  107,  108,  109,  133,  147,   44,
 /*   990 */    47,  133,  103,  152,   47,   47,   83,  143,   47,  122,
 /*  1000 */   111,  157,  158,   29,  115,   92,   93,  149,  150,   45,
 /*  1010 */   133,   39,   38,   39,   45,  138,  139,  140,   44,   56,
 /*  1020 */    51,  132,  103,   45,   44,   55,   44,   44,   47,   44,
 /*  1030 */   111,   57,  159,  144,  115,   44,  147,  111,  103,   44,
 /*  1040 */   151,  152,  153,  159,  159,  159,  111,  159,  122,  103,
 /*  1050 */   115,  132,  159,  111,  112,  113,  114,  111,  159,  133,
 /*  1060 */   159,  115,  159,  144,  122,  103,  147,  132,  159,  159,
 /*  1070 */   151,  152,  153,  111,  159,  133,  159,  115,  132,  144,
 /*  1080 */   159,  159,  147,  159,  158,  159,  151,  152,  153,  159,
 /*  1090 */   144,  103,  159,  147,  132,  103,  159,  151,  152,  153,
 /*  1100 */   103,  159,  159,  115,  103,  159,  144,  115,  159,  147,
 /*  1110 */   122,  159,  115,  151,  152,  153,  115,  129,  126,  122,
 /*  1120 */   132,  133,  159,  131,  132,  103,  129,  159,  159,  132,
 /*  1130 */   133,  159,  159,  132,  103,  103,  159,  115,  103,  159,
 /*  1140 */   152,  159,   29,  159,  103,  159,  115,  115,  147,  152,
 /*  1150 */   115,  159,   39,  152,  132,  159,  115,   44,  126,  103,
 /*  1160 */   103,  126,  159,  132,  132,  103,  159,  132,  103,  147,
 /*  1170 */    57,  115,  115,  132,  152,  103,  159,  115,  147,  103,
 /*  1180 */   115,  159,  159,  152,  159,  159,  103,  115,  132,  132,
 /*  1190 */   103,  115,  159,  152,  132,  103,  159,  132,  115,  159,
 /*  1200 */   159,  103,  115,  159,  132,  159,  159,  115,  132,  159,
 /*  1210 */   154,  154,  159,  115,  103,  132,  159,  103,  159,  132,
 /*  1220 */   103,  103,  103,  103,  132,  103,  115,  103,  103,  115,
 /*  1230 */   132,  159,  115,  115,  115,  115,  159,  115,  159,  115,
 /*  1240 */   115,  103,  159,  132,  159,  159,  132,  159,  103,  132,
 /*  1250 */   132,  132,  132,  115,  132,  159,  132,  132,  159,  159,
 /*  1260 */   115,  103,  103,  103,  103,  159,  159,  103,  103,  103,
 /*  1270 */   132,  103,  103,  115,  115,  115,  115,  132,  159,  115,
 /*  1280 */   115,  115,  103,  115,  115,  159,  159,  103,  159,  159,
 /*  1290 */   132,  132,  132,  132,  115,  103,  132,  132,  132,  115,
 /*  1300 */   132,  132,  103,  159,  159,  103,  103,  115,  103,  159,
 /*  1310 */   159,  132,  159,  159,  115,  103,  132,  115,  115,  159,
 /*  1320 */   115,  103,  159,  159,  132,  159,  103,  115,  103,  103,
 /*  1330 */   103,  132,  159,  115,  132,  132,  103,  132,  115,  103,
 /*  1340 */   115,  115,  115,  159,  132,  159,  159,  159,  115,  159,
 /*  1350 */   132,  115,  103,  159,  159,  132,  159,  132,  132,  132,
 /*  1360 */   159,  159,  159,  159,  115,  132,  159,  159,  132,  159,
 /*  1370 */   159,  159,  159,  159,  159,  159,  159,  159,  159,  159,
 /*  1380 */   159,  132,
};
#define YY_SHIFT_USE_DFLT (1382)
#define YY_SHIFT_COUNT    (253)
#define YY_SHIFT_MIN      (-1)
#define YY_SHIFT_MAX      (1113)
static const short yy_shift_ofst[] = {
 /*     0 */    46,  148,  185,  219,  280,  282,  379,  379,  379,  379,
 /*    10 */   112,  112,  112,  381,  443,  491,  825,  600,  636,  636,
 /*    20 */   756,  756,  825,  825,  825,  825,  538,  636,  756,  756,
 /*    30 */   825,  825,  825,  825,  974,  974,  974, 1113, 1113,  780,
 /*    40 */   825,  825,  825,  825,  825,  825,  825,  825,  825,  849,
 /*    50 */   825,  825,  825,  825,  825,  825,  825,  825,  825,  825,
 /*    60 */   825,  825,  825,  825,  825,  825,  825,  825,  825,  825,
 /*    70 */   825,  825,  825,  825,  825,  825,  825,  825,  825,  825,
 /*    80 */   825,  825,  825,  825,  913, 1113,  579,  104,  104,  104,
 /*    90 */  1113, 1113, 1113, 1113, 1113,  283,  581,  279,   71,  412,
 /*   100 */    26,  128,  128,  128,   98,  159,  189,   34,  120,  394,
 /*   110 */   194,  187,  520,   48,  527,  553,  553,  553,  583,  583,
 /*   120 */   583,   98,  361,  361,  361,  441,  549,  159,  582,  549,
 /*   130 */   604,  159,  641,  159,   98,  604,   98,  159,  159,  713,
 /*   140 */   713,  725,  763, 1382, 1382, 1382, 1382, 1382,   -1,   40,
 /*   150 */    97,   97,   97,   97,   97,   97,   97,   97,   97,   97,
 /*   160 */    97,   97,   97,   97,   97,   97,  364,  459,  617,  505,
 /*   170 */   697,  720,  741,  741,  899,  899,  899,  899,  652,  652,
 /*   180 */    53,   53,  158,  158,  158,  158,  158,  158,  158,  158,
 /*   190 */   158,  158,  158,  158,  158,  369,  574,  785,  820,  523,
 /*   200 */   862,  864,  865,  575,  748,  602,  927,  930,  969,  799,
 /*   210 */   801,  811,  827,  819,  832,  842,  839,  846,  845,  867,
 /*   220 */   872,  873,  874,  875,  884,  887,  892,  898,  903,  904,
 /*   230 */   915,  911,  931,  876,  932,  943,  945,  923,  947,  948,
 /*   240 */   951,  934,  964,  972,  978,  963,  970,  980,  982,  983,
 /*   250 */   981,  985,  991,  995,
};
#define YY_REDUCE_USE_DFLT (-86)
#define YY_REDUCE_COUNT (147)
#define YY_REDUCE_MIN   (-85)
#define YY_REDUCE_MAX   (1249)
static const short yy_reduce_ofst[] = {
 /*     0 */   -19,  790,  854,  877,  889,   35,  919,  935,  946,  962,
 /*    10 */   718,  988,  997,  455,  858,  942,  759,  844,  107,  177,
 /*    20 */   691,  992,  841, 1001, 1022, 1031,  926,  203, 1032, 1035,
 /*    30 */   723, 1041, 1056, 1057,  456,  461,  712,  151,  301, 1062,
 /*    40 */  1062, 1065, 1072, 1076, 1083, 1087, 1092, 1098, 1111, 1114,
 /*    50 */  1117, 1118, 1119, 1120, 1122, 1124, 1125, 1138, 1145, 1158,
 /*    60 */  1159, 1160, 1161, 1164, 1165, 1166, 1168, 1169, 1179, 1184,
 /*    70 */  1192, 1199, 1202, 1203, 1205, 1212, 1218, 1223, 1225, 1226,
 /*    80 */  1227, 1233, 1236, 1249, 1062,  -39,  220,  -81,   36,  206,
 /*    90 */   662,  671,  678,  695,  716,  -26,  -85,  220,  276,  233,
 /*   100 */   396,  184,  318,  344,   31,  464,  300,  111,  205,  111,
 /*   110 */   111,  207,  227,  235,  293,  205,  205,  205,  111,  111,
 /*   120 */   111,  298,  273,  278,  324,  421,  429,  481,  535,  530,
 /*   130 */   568,  570,  594,  614,  627,  618,  630,  637,  638,  601,
 /*   140 */   623,  645,  708,  677,  688,  684,  732,  744,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   850,  750,  750,  850,  834,  676,  834,  834,  834,  834,
 /*    10 */   850,  850,  850,  676,  765,  850,  834,  850,  676,  676,
 /*    20 */   689,  689,  834,  834,  834,  834,  850,  850,  850,  850,
 /*    30 */   850,  850,  837,  837,  719,  717,  714,  850,  850,  850,
 /*    40 */   850,  850,  850,  850,  850,  850,  850,  850,  850,  850,
 /*    50 */   850,  850,  850,  850,  850,  850,  850,  850,  850,  850,
 /*    60 */   850,  850,  850,  850,  850,  850,  850,  850,  850,  850,
 /*    70 */   850,  850,  850,  850,  850,  850,  850,  850,  850,  850,
 /*    80 */   850,  850,  850,  850,  785,  850,  668,  719,  717,  714,
 /*    90 */   850,  850,  850,  850,  850,  849,  849,  850,  752,  752,
 /*   100 */   850,  663,  663,  663,  651,  850,  850,  668,  850,  850,
 /*   110 */   850,  850,  850,  850,  850,  715,  718,  720,  675,  674,
 /*   120 */   679,  850,  850,  850,  850,  661,  850,  850,  850,  850,
 /*   130 */   849,  850,  850,  850,  850,  849,  850,  850,  850,  850,
 /*   140 */   850,  711,  628,  742,  732,  761,  641,  630,  850,  850,
 /*   150 */   832,  835,  836,  830,  829,  828,  827,  826,  825,  824,
 /*   160 */   823,  822,  821,  820,  831,  686,  819,  811,  812,  815,
 /*   170 */   816,  817,  808,  807,  810,  809,  806,  805,  814,  813,
 /*   180 */   802,  801,  799,  798,  797,  796,  795,  794,  793,  792,
 /*   190 */   791,  800,  818,  804,  803,  850,  850,  850,  850,  850,
 /*   200 */   850,  850,  850,  850,  850,  850,  850,  850,  850,  850,
 /*   210 */   850,  850,  850,  850,  850,  850,  850,  850,  850,  850,
 /*   220 */   850,  850,  850,  850,  850,  850,  850,  850,  850,  850,
 /*   230 */   850,  850,  850,  772,  850,  850,  850,  833,  850,  850,
 /*   240 */   850,  850,  850,  850,  850,  850,  850,  850,  850,  850,
 /*   250 */   850,  850,  850,  850,
};
/********** End of lemon-generated parsing tables *****************************/

/* The next table maps tokens (terminal symbols) into fallback tokens.  
** If a construct like the following:
** 
**      %fallback ID X Y Z.
**
** appears in the grammar, then ID becomes a fallback token for X, Y,
** and Z.  Whenever one of the tokens X, Y, or Z is input to the parser
** but it does not parse, the type of the token is changed to ID and
** the parse is retried before an error is thrown.
**
** This feature can be used, for example, to cause some keywords in a language
** to revert to identifiers if they keyword does not apply in the context where
** it appears.
*/
#ifdef YYFALLBACK
static const YYCODETYPE yyFallback[] = {
};
#endif /* YYFALLBACK */

/* The following structure represents a single element of the
** parser's stack.  Information stored includes:
**
**   +  The state number for the parser at this level of the stack.
**
**   +  The value of the token stored at this level of the stack.
**      (In other words, the "major" token.)
**
**   +  The semantic value stored at this level of the stack.  This is
**      the information used by the action routines in the grammar.
**      It is sometimes called the "minor" token.
**
** After the "shift" half of a SHIFTREDUCE action, the stateno field
** actually contains the reduce action for the second half of the
** SHIFTREDUCE.
*/
struct yyStackEntry {
  YYACTIONTYPE stateno;  /* The state-number, or reduce action in SHIFTREDUCE */
  YYCODETYPE major;      /* The major token value.  This is the code
                         ** number for the token at this stack level */
  YYMINORTYPE minor;     /* The user-supplied minor token value.  This
                         ** is the value of the token  */
};
typedef struct yyStackEntry yyStackEntry;

/* The state of the parser is completely contained in an instance of
** the following structure */
struct yyParser {
  yyStackEntry *yytos;          /* Pointer to top element of the stack */
#ifdef YYTRACKMAXSTACKDEPTH
  int yyhwm;                    /* High-water mark of the stack */
#endif
#ifndef YYNOERRORRECOVERY
  int yyerrcnt;                 /* Shifts left before out of the error */
#endif
  snlParserARG_SDECL                /* A place to hold %extra_argument */
#if YYSTACKDEPTH<=0
  int yystksz;                  /* Current side of the stack */
  yyStackEntry *yystack;        /* The parser's stack */
  yyStackEntry yystk0;          /* First stack entry */
#else
  yyStackEntry yystack[YYSTACKDEPTH];  /* The parser's stack */
#endif
};
typedef struct yyParser yyParser;

#ifndef NDEBUG
#include <stdio.h>
static FILE *yyTraceFILE = 0;
static char *yyTracePrompt = 0;
#endif /* NDEBUG */

#ifndef NDEBUG
/* 
** Turn parser tracing on by giving a stream to which to write the trace
** and a prompt to preface each trace message.  Tracing is turned off
** by making either argument NULL 
**
** Inputs:
** <ul>
** <li> A FILE* to which trace output should be written.
**      If NULL, then tracing is turned off.
** <li> A prefix string written at the beginning of every
**      line of trace output.  If NULL, then tracing is
**      turned off.
** </ul>
**
** Outputs:
** None.
*/
void snlParserTrace(FILE *TraceFILE, char *zTracePrompt){
  yyTraceFILE = TraceFILE;
  yyTracePrompt = zTracePrompt;
  if( yyTraceFILE==0 ) yyTracePrompt = 0;
  else if( yyTracePrompt==0 ) yyTraceFILE = 0;
}
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing shifts, the names of all terminals and nonterminals
** are required.  The following table supplies these names */
static const char *const yyTokenName[] = { 
  "$",             "EQUAL",         "ADDEQ",         "SUBEQ",       
  "ANDEQ",         "OREQ",          "DIVEQ",         "MULEQ",       
  "MODEQ",         "LSHEQ",         "RSHEQ",         "XOREQ",       
  "QUESTION",      "COLON",         "OROR",          "ANDAND",      
  "VBAR",          "CARET",         "AMPERSAND",     "EQ",          
  "NE",            "GT",            "GE",            "LE",          
  "LT",            "LSHIFT",        "RSHIFT",        "ADD",         
  "SUB",           "ASTERISK",      "SLASH",         "MOD",         
  "NOT",           "TILDE",         "INCR",          "DECR",        
  "SIZEOF",        "PRE",           "LBRACKET",      "LPAREN",      
  "POINTER",       "PERIOD",        "POST",          "PROGRAM",     
  "NAME",          "RPAREN",        "ASSIGN",        "SEMICOLON",   
  "LBRACE",        "RBRACE",        "TO",            "COMMA",       
  "MONITOR",       "SYNC",          "SYNCQ",         "INTCON",      
  "RBRACKET",      "CONST",         "FOREIGN",       "CHAR",        
  "SHORT",         "INT",           "LONG",          "UNSIGNED",    
  "INT8T",         "UINT8T",        "INT16T",        "UINT16T",     
  "INT32T",        "UINT32T",       "FLOAT",         "DOUBLE",      
  "STRING",        "EVFLAG",        "VOID",          "ENUM",        
  "STRUCT",        "UNION",         "TYPENAME",      "OPTION",      
  "SS",            "STATE",         "ENTRY",         "EXIT",        
  "WHEN",          "BREAK",         "CONTINUE",      "RETURN",      
  "IF",            "ELSE",          "WHILE",         "FOR",         
  "FPCON",         "STRCON",        "CCODE",         "error",       
  "program",       "program_param",  "initial_defns",  "entry",       
  "state_sets",    "exit",          "final_defns",   "string",      
  "initial_defn",  "assign",        "monitor",       "sync",        
  "syncq",         "declaration",   "option",        "c_code",      
  "funcdef",       "structdef",     "final_defn",    "variable",    
  "to",            "subscript",     "strings",       "opt_subscript",
  "event_flag",    "syncq_size",    "basetype",      "init_declarators",
  "init_declarator",  "declarator",    "init_expr",     "param_decls", 
  "param_decl",    "type_expr",     "variables",     "init_exprs",  
  "expr",          "prim_type",     "abs_decl",      "option_value",
  "state_set",     "ss_defns",      "states",        "ss_defn",     
  "state",         "state_defns",   "transitions",   "state_defn",  
  "block",         "transition",    "condition",     "opt_expr",    
  "block_defns",   "statements",    "block_defn",    "statement",   
  "comma_expr",    "for_statement",  "args",          "member",      
  "members",       "member_decls",  "member_decl", 
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "program ::= PROGRAM NAME program_param initial_defns entry state_sets exit final_defns",
 /*   1 */ "program_param ::= LPAREN string RPAREN",
 /*   2 */ "program_param ::=",
 /*   3 */ "initial_defns ::= initial_defns initial_defn",
 /*   4 */ "initial_defns ::=",
 /*   5 */ "initial_defn ::= assign",
 /*   6 */ "initial_defn ::= monitor",
 /*   7 */ "initial_defn ::= sync",
 /*   8 */ "initial_defn ::= syncq",
 /*   9 */ "initial_defn ::= declaration",
 /*  10 */ "initial_defn ::= option",
 /*  11 */ "initial_defn ::= c_code",
 /*  12 */ "initial_defn ::= funcdef",
 /*  13 */ "initial_defn ::= structdef",
 /*  14 */ "final_defns ::= final_defns final_defn",
 /*  15 */ "final_defns ::=",
 /*  16 */ "final_defn ::= c_code",
 /*  17 */ "final_defn ::= funcdef",
 /*  18 */ "final_defn ::= structdef",
 /*  19 */ "assign ::= ASSIGN variable to string SEMICOLON",
 /*  20 */ "assign ::= ASSIGN variable subscript to string SEMICOLON",
 /*  21 */ "assign ::= ASSIGN variable to LBRACE strings RBRACE SEMICOLON",
 /*  22 */ "assign ::= ASSIGN variable SEMICOLON",
 /*  23 */ "strings ::= strings COMMA string",
 /*  24 */ "strings ::= string",
 /*  25 */ "strings ::=",
 /*  26 */ "monitor ::= MONITOR variable opt_subscript SEMICOLON",
 /*  27 */ "monitor ::= MONITOR variable opt_subscript error SEMICOLON",
 /*  28 */ "sync ::= SYNC variable opt_subscript to event_flag SEMICOLON",
 /*  29 */ "sync ::= SYNC variable opt_subscript to event_flag error SEMICOLON",
 /*  30 */ "syncq ::= SYNCQ variable opt_subscript to event_flag syncq_size SEMICOLON",
 /*  31 */ "syncq ::= SYNCQ variable opt_subscript syncq_size SEMICOLON",
 /*  32 */ "event_flag ::= NAME",
 /*  33 */ "variable ::= NAME",
 /*  34 */ "syncq_size ::= INTCON",
 /*  35 */ "syncq_size ::=",
 /*  36 */ "opt_subscript ::= subscript",
 /*  37 */ "opt_subscript ::=",
 /*  38 */ "subscript ::= LBRACKET INTCON RBRACKET",
 /*  39 */ "declaration ::= basetype init_declarators SEMICOLON",
 /*  40 */ "init_declarators ::= init_declarator",
 /*  41 */ "init_declarators ::= init_declarators COMMA init_declarator",
 /*  42 */ "init_declarator ::= declarator",
 /*  43 */ "init_declarator ::= declarator EQUAL init_expr",
 /*  44 */ "declarator ::= variable",
 /*  45 */ "declarator ::= declarator subscript",
 /*  46 */ "declarator ::= declarator LPAREN param_decls RPAREN",
 /*  47 */ "declarator ::= LPAREN declarator RPAREN",
 /*  48 */ "declarator ::= ASTERISK declarator",
 /*  49 */ "declarator ::= CONST declarator",
 /*  50 */ "param_decls ::=",
 /*  51 */ "param_decls ::= param_decl",
 /*  52 */ "param_decls ::= param_decls COMMA param_decl",
 /*  53 */ "param_decl ::= basetype declarator",
 /*  54 */ "param_decl ::= type_expr",
 /*  55 */ "declaration ::= FOREIGN variables SEMICOLON",
 /*  56 */ "variables ::= variable",
 /*  57 */ "variables ::= variables COMMA variable",
 /*  58 */ "init_expr ::= LPAREN type_expr RPAREN LBRACE init_exprs RBRACE",
 /*  59 */ "init_expr ::= LBRACE init_exprs RBRACE",
 /*  60 */ "init_expr ::= expr",
 /*  61 */ "init_exprs ::= init_exprs COMMA init_expr",
 /*  62 */ "init_exprs ::= init_expr",
 /*  63 */ "init_exprs ::=",
 /*  64 */ "prim_type ::= CHAR",
 /*  65 */ "prim_type ::= SHORT",
 /*  66 */ "prim_type ::= INT",
 /*  67 */ "prim_type ::= LONG",
 /*  68 */ "prim_type ::= UNSIGNED CHAR",
 /*  69 */ "prim_type ::= UNSIGNED SHORT",
 /*  70 */ "prim_type ::= UNSIGNED INT",
 /*  71 */ "prim_type ::= UNSIGNED LONG",
 /*  72 */ "prim_type ::= INT8T",
 /*  73 */ "prim_type ::= UINT8T",
 /*  74 */ "prim_type ::= INT16T",
 /*  75 */ "prim_type ::= UINT16T",
 /*  76 */ "prim_type ::= INT32T",
 /*  77 */ "prim_type ::= UINT32T",
 /*  78 */ "prim_type ::= FLOAT",
 /*  79 */ "prim_type ::= DOUBLE",
 /*  80 */ "prim_type ::= STRING",
 /*  81 */ "basetype ::= prim_type",
 /*  82 */ "basetype ::= EVFLAG",
 /*  83 */ "basetype ::= VOID",
 /*  84 */ "basetype ::= ENUM NAME",
 /*  85 */ "basetype ::= STRUCT NAME",
 /*  86 */ "basetype ::= UNION NAME",
 /*  87 */ "basetype ::= TYPENAME NAME",
 /*  88 */ "type_expr ::= basetype",
 /*  89 */ "type_expr ::= basetype abs_decl",
 /*  90 */ "abs_decl ::= LPAREN abs_decl RPAREN",
 /*  91 */ "abs_decl ::= ASTERISK",
 /*  92 */ "abs_decl ::= ASTERISK abs_decl",
 /*  93 */ "abs_decl ::= CONST",
 /*  94 */ "abs_decl ::= CONST abs_decl",
 /*  95 */ "abs_decl ::= subscript",
 /*  96 */ "abs_decl ::= abs_decl subscript",
 /*  97 */ "abs_decl ::= LPAREN param_decls RPAREN",
 /*  98 */ "abs_decl ::= abs_decl LPAREN param_decls RPAREN",
 /*  99 */ "option ::= OPTION option_value NAME SEMICOLON",
 /* 100 */ "option_value ::= ADD",
 /* 101 */ "option_value ::= SUB",
 /* 102 */ "state_sets ::= state_sets state_set",
 /* 103 */ "state_sets ::= state_set",
 /* 104 */ "state_set ::= SS NAME LBRACE ss_defns states RBRACE",
 /* 105 */ "ss_defns ::= ss_defns ss_defn",
 /* 106 */ "ss_defns ::=",
 /* 107 */ "ss_defn ::= assign",
 /* 108 */ "ss_defn ::= monitor",
 /* 109 */ "ss_defn ::= sync",
 /* 110 */ "ss_defn ::= syncq",
 /* 111 */ "ss_defn ::= declaration",
 /* 112 */ "states ::= states state",
 /* 113 */ "states ::= state",
 /* 114 */ "state ::= STATE NAME LBRACE state_defns entry transitions exit RBRACE",
 /* 115 */ "state_defns ::= state_defns state_defn",
 /* 116 */ "state_defns ::=",
 /* 117 */ "state_defn ::= assign",
 /* 118 */ "state_defn ::= monitor",
 /* 119 */ "state_defn ::= sync",
 /* 120 */ "state_defn ::= syncq",
 /* 121 */ "state_defn ::= declaration",
 /* 122 */ "state_defn ::= option",
 /* 123 */ "entry ::= ENTRY block",
 /* 124 */ "entry ::=",
 /* 125 */ "exit ::= EXIT block",
 /* 126 */ "exit ::=",
 /* 127 */ "transitions ::= transitions transition",
 /* 128 */ "transitions ::= transition",
 /* 129 */ "transition ::= WHEN LPAREN condition RPAREN block STATE NAME",
 /* 130 */ "transition ::= WHEN LPAREN condition RPAREN block EXIT",
 /* 131 */ "transition ::= WHEN LPAREN condition RPAREN block error",
 /* 132 */ "condition ::= opt_expr",
 /* 133 */ "block ::= LBRACE block_defns statements RBRACE",
 /* 134 */ "block_defns ::= block_defns block_defn",
 /* 135 */ "block_defns ::=",
 /* 136 */ "block_defn ::= declaration",
 /* 137 */ "block_defn ::= c_code",
 /* 138 */ "statements ::= statements statement",
 /* 139 */ "statements ::=",
 /* 140 */ "statement ::= BREAK SEMICOLON",
 /* 141 */ "statement ::= CONTINUE SEMICOLON",
 /* 142 */ "statement ::= RETURN opt_expr SEMICOLON",
 /* 143 */ "statement ::= STATE NAME SEMICOLON",
 /* 144 */ "statement ::= c_code",
 /* 145 */ "statement ::= block",
 /* 146 */ "statement ::= IF LPAREN comma_expr RPAREN statement",
 /* 147 */ "statement ::= IF LPAREN comma_expr RPAREN statement ELSE statement",
 /* 148 */ "statement ::= WHILE LPAREN comma_expr RPAREN statement",
 /* 149 */ "statement ::= for_statement",
 /* 150 */ "statement ::= opt_expr SEMICOLON",
 /* 151 */ "for_statement ::= FOR LPAREN opt_expr SEMICOLON opt_expr SEMICOLON opt_expr RPAREN statement",
 /* 152 */ "expr ::= INTCON",
 /* 153 */ "expr ::= FPCON",
 /* 154 */ "expr ::= string",
 /* 155 */ "expr ::= variable",
 /* 156 */ "expr ::= LPAREN comma_expr RPAREN",
 /* 157 */ "expr ::= expr LPAREN args RPAREN",
 /* 158 */ "expr ::= EXIT LPAREN args RPAREN",
 /* 159 */ "expr ::= SIZEOF LPAREN type_expr RPAREN",
 /* 160 */ "expr ::= expr LBRACKET expr RBRACKET",
 /* 161 */ "expr ::= expr PERIOD member",
 /* 162 */ "expr ::= expr POINTER member",
 /* 163 */ "expr ::= expr INCR",
 /* 164 */ "expr ::= expr DECR",
 /* 165 */ "expr ::= ADD expr",
 /* 166 */ "expr ::= SUB expr",
 /* 167 */ "expr ::= ASTERISK expr",
 /* 168 */ "expr ::= AMPERSAND expr",
 /* 169 */ "expr ::= NOT expr",
 /* 170 */ "expr ::= TILDE expr",
 /* 171 */ "expr ::= INCR expr",
 /* 172 */ "expr ::= DECR expr",
 /* 173 */ "expr ::= SIZEOF expr",
 /* 174 */ "expr ::= LPAREN type_expr RPAREN expr",
 /* 175 */ "expr ::= expr SUB expr",
 /* 176 */ "expr ::= expr ADD expr",
 /* 177 */ "expr ::= expr ASTERISK expr",
 /* 178 */ "expr ::= expr SLASH expr",
 /* 179 */ "expr ::= expr GT expr",
 /* 180 */ "expr ::= expr GE expr",
 /* 181 */ "expr ::= expr EQ expr",
 /* 182 */ "expr ::= expr NE expr",
 /* 183 */ "expr ::= expr LE expr",
 /* 184 */ "expr ::= expr LT expr",
 /* 185 */ "expr ::= expr OROR expr",
 /* 186 */ "expr ::= expr ANDAND expr",
 /* 187 */ "expr ::= expr LSHIFT expr",
 /* 188 */ "expr ::= expr RSHIFT expr",
 /* 189 */ "expr ::= expr VBAR expr",
 /* 190 */ "expr ::= expr CARET expr",
 /* 191 */ "expr ::= expr AMPERSAND expr",
 /* 192 */ "expr ::= expr MOD expr",
 /* 193 */ "expr ::= expr QUESTION expr COLON expr",
 /* 194 */ "expr ::= expr EQUAL expr",
 /* 195 */ "expr ::= expr ADDEQ expr",
 /* 196 */ "expr ::= expr SUBEQ expr",
 /* 197 */ "expr ::= expr ANDEQ expr",
 /* 198 */ "expr ::= expr OREQ expr",
 /* 199 */ "expr ::= expr DIVEQ expr",
 /* 200 */ "expr ::= expr MULEQ expr",
 /* 201 */ "expr ::= expr MODEQ expr",
 /* 202 */ "expr ::= expr LSHEQ expr",
 /* 203 */ "expr ::= expr RSHEQ expr",
 /* 204 */ "expr ::= expr XOREQ expr",
 /* 205 */ "comma_expr ::= comma_expr COMMA expr",
 /* 206 */ "comma_expr ::= expr",
 /* 207 */ "opt_expr ::= comma_expr",
 /* 208 */ "opt_expr ::=",
 /* 209 */ "args ::= args COMMA expr",
 /* 210 */ "args ::= expr",
 /* 211 */ "args ::=",
 /* 212 */ "string ::= STRCON",
 /* 213 */ "member ::= NAME",
 /* 214 */ "funcdef ::= basetype declarator block",
 /* 215 */ "structdef ::= STRUCT NAME members SEMICOLON",
 /* 216 */ "members ::= LBRACE member_decls RBRACE",
 /* 217 */ "member_decls ::= member_decl",
 /* 218 */ "member_decls ::= member_decls member_decl",
 /* 219 */ "member_decl ::= basetype declarator SEMICOLON",
 /* 220 */ "member_decl ::= c_code",
 /* 221 */ "c_code ::= CCODE",
 /* 222 */ "to ::= TO",
 /* 223 */ "to ::=",
};
#endif /* NDEBUG */


#if YYSTACKDEPTH<=0
/*
** Try to increase the size of the parser stack.  Return the number
** of errors.  Return 0 on success.
*/
static int yyGrowStack(yyParser *p){
  int newSize;
  int idx;
  yyStackEntry *pNew;

  newSize = p->yystksz*2 + 100;
  idx = p->yytos ? (int)(p->yytos - p->yystack) : 0;
  if( p->yystack==&p->yystk0 ){
    pNew = malloc(newSize*sizeof(pNew[0]));
    if( pNew ) pNew[0] = p->yystk0;
  }else{
    pNew = realloc(p->yystack, newSize*sizeof(pNew[0]));
  }
  if( pNew ){
    p->yystack = pNew;
    p->yytos = &p->yystack[idx];
#ifndef NDEBUG
    if( yyTraceFILE ){
      fprintf(yyTraceFILE,"%sStack grows from %d to %d entries.\n",
              yyTracePrompt, p->yystksz, newSize);
    }
#endif
    p->yystksz = newSize;
  }
  return pNew==0; 
}
#endif

/* Datatype of the argument to the memory allocated passed as the
** second argument to snlParserAlloc() below.  This can be changed by
** putting an appropriate #define in the %include section of the input
** grammar.
*/
#ifndef YYMALLOCARGTYPE
# define YYMALLOCARGTYPE size_t
#endif

/* 
** This function allocates a new parser.
** The only argument is a pointer to a function which works like
** malloc.
**
** Inputs:
** A pointer to the function used to allocate memory.
**
** Outputs:
** A pointer to a parser.  This pointer is used in subsequent calls
** to snlParser and snlParserFree.
*/
void *snlParserAlloc(void *(*mallocProc)(YYMALLOCARGTYPE)){
  yyParser *pParser;
  pParser = (yyParser*)(*mallocProc)( (YYMALLOCARGTYPE)sizeof(yyParser) );
  if( pParser ){
#ifdef YYTRACKMAXSTACKDEPTH
    pParser->yyhwm = 0;
#endif
#if YYSTACKDEPTH<=0
    pParser->yytos = NULL;
    pParser->yystack = NULL;
    pParser->yystksz = 0;
    if( yyGrowStack(pParser) ){
      pParser->yystack = &pParser->yystk0;
      pParser->yystksz = 1;
    }
#endif
#ifndef YYNOERRORRECOVERY
    pParser->yyerrcnt = -1;
#endif
    pParser->yytos = pParser->yystack;
    pParser->yystack[0].stateno = 0;
    pParser->yystack[0].major = 0;
  }
  return pParser;
}

/* The following function deletes the "minor type" or semantic value
** associated with a symbol.  The symbol can be either a terminal
** or nonterminal. "yymajor" is the symbol code, and "yypminor" is
** a pointer to the value to be deleted.  The code used to do the 
** deletions is derived from the %destructor and/or %token_destructor
** directives of the input grammar.
*/
static void yy_destructor(
  yyParser *yypParser,    /* The parser */
  YYCODETYPE yymajor,     /* Type code for object to destroy */
  YYMINORTYPE *yypminor   /* The object to be destroyed */
){
  snlParserARG_FETCH;
  switch( yymajor ){
    /* Here is inserted the actions which take place when a
    ** terminal or non-terminal is destroyed.  This can happen
    ** when the symbol is popped from the stack during a
    ** reduce or during error processing or when a parser is 
    ** being destroyed before it is finished parsing.
    **
    ** Note: during a reduce, the only symbols destroyed are those
    ** which appear on the RHS of the rule, but which are *not* used
    ** inside the C code.
    */
/********* Begin destructor definitions ***************************************/
/********* End destructor definitions *****************************************/
    default:  break;   /* If no destructor action specified: do nothing */
  }
  snlParserARG_STORE; /* Suppress warning about unused %extra_argument var */
}

/*
** Pop the parser's stack once.
**
** If there is a destructor routine associated with the token which
** is popped from the stack, then call it.
*/
static void yy_pop_parser_stack(yyParser *pParser){
  yyStackEntry *yytos;
  assert( pParser->yytos!=0 );
  assert( pParser->yytos > pParser->yystack );
  yytos = pParser->yytos--;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sPopping %s\n",
      yyTracePrompt,
      yyTokenName[yytos->major]);
  }
#endif
  yy_destructor(pParser, yytos->major, &yytos->minor);
}

/* 
** Deallocate and destroy a parser.  Destructors are called for
** all stack elements before shutting the parser down.
**
** If the YYPARSEFREENEVERNULL macro exists (for example because it
** is defined in a %include section of the input grammar) then it is
** assumed that the input pointer is never NULL.
*/
void snlParserFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
  yyParser *pParser = (yyParser*)p;
#ifndef YYPARSEFREENEVERNULL
  if( pParser==0 ) return;
#endif
  while( pParser->yytos>pParser->yystack ) yy_pop_parser_stack(pParser);
#if YYSTACKDEPTH<=0
  if( pParser->yystack!=&pParser->yystk0 ) free(pParser->yystack);
#endif
  (*freeProc)((void*)pParser);
}

/*
** Return the peak depth of the stack for a parser.
*/
#ifdef YYTRACKMAXSTACKDEPTH
int snlParserStackPeak(void *p){
  yyParser *pParser = (yyParser*)p;
  return pParser->yyhwm;
}
#endif

/*
** Find the appropriate action for a parser given the terminal
** look-ahead token iLookAhead.
*/
static unsigned int yy_find_shift_action(
  yyParser *pParser,        /* The parser */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
  int stateno = pParser->yytos->stateno;
 
  if( stateno>=YY_MIN_REDUCE ) return stateno;
  assert( stateno <= YY_SHIFT_COUNT );
  do{
    i = yy_shift_ofst[stateno];
    assert( iLookAhead!=YYNOCODE );
    i += iLookAhead;
    if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
#ifdef YYFALLBACK
      YYCODETYPE iFallback;            /* Fallback token */
      if( iLookAhead<sizeof(yyFallback)/sizeof(yyFallback[0])
             && (iFallback = yyFallback[iLookAhead])!=0 ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE, "%sFALLBACK %s => %s\n",
             yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[iFallback]);
        }
#endif
        assert( yyFallback[iFallback]==0 ); /* Fallback loop must terminate */
        iLookAhead = iFallback;
        continue;
      }
#endif
#ifdef YYWILDCARD
      {
        int j = i - iLookAhead + YYWILDCARD;
        if( 
#if YY_SHIFT_MIN+YYWILDCARD<0
          j>=0 &&
#endif
#if YY_SHIFT_MAX+YYWILDCARD>=YY_ACTTAB_COUNT
          j<YY_ACTTAB_COUNT &&
#endif
          yy_lookahead[j]==YYWILDCARD && iLookAhead>0
        ){
#ifndef NDEBUG
          if( yyTraceFILE ){
            fprintf(yyTraceFILE, "%sWILDCARD %s => %s\n",
               yyTracePrompt, yyTokenName[iLookAhead],
               yyTokenName[YYWILDCARD]);
          }
#endif /* NDEBUG */
          return yy_action[j];
        }
      }
#endif /* YYWILDCARD */
      return yy_default[stateno];
    }else{
      return yy_action[i];
    }
  }while(1);
}

/*
** Find the appropriate action for a parser given the non-terminal
** look-ahead token iLookAhead.
*/
static int yy_find_reduce_action(
  int stateno,              /* Current state number */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
#ifdef YYERRORSYMBOL
  if( stateno>YY_REDUCE_COUNT ){
    return yy_default[stateno];
  }
#else
  assert( stateno<=YY_REDUCE_COUNT );
#endif
  i = yy_reduce_ofst[stateno];
  assert( i!=YY_REDUCE_USE_DFLT );
  assert( iLookAhead!=YYNOCODE );
  i += iLookAhead;
#ifdef YYERRORSYMBOL
  if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
    return yy_default[stateno];
  }
#else
  assert( i>=0 && i<YY_ACTTAB_COUNT );
  assert( yy_lookahead[i]==iLookAhead );
#endif
  return yy_action[i];
}

/*
** The following routine is called if the stack overflows.
*/
static void yyStackOverflow(yyParser *yypParser){
   snlParserARG_FETCH;
   yypParser->yytos--;
#ifndef NDEBUG
   if( yyTraceFILE ){
     fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
   }
#endif
   while( yypParser->yytos>yypParser->yystack ) yy_pop_parser_stack(yypParser);
   /* Here code is inserted which will execute if the parser
   ** stack every overflows */
/******** Begin %stack_overflow code ******************************************/
/******** End %stack_overflow code ********************************************/
   snlParserARG_STORE; /* Suppress warning about unused %extra_argument var */
}

/*
** Print tracing information for a SHIFT action
*/
#ifndef NDEBUG
static void yyTraceShift(yyParser *yypParser, int yyNewState){
  if( yyTraceFILE ){
    if( yyNewState<YYNSTATE ){
      fprintf(yyTraceFILE,"%sShift '%s', go to state %d\n",
         yyTracePrompt,yyTokenName[yypParser->yytos->major],
         yyNewState);
    }else{
      fprintf(yyTraceFILE,"%sShift '%s'\n",
         yyTracePrompt,yyTokenName[yypParser->yytos->major]);
    }
  }
}
#else
# define yyTraceShift(X,Y)
#endif

/*
** Perform a shift action.
*/
static void yy_shift(
  yyParser *yypParser,          /* The parser to be shifted */
  int yyNewState,               /* The new state to shift in */
  int yyMajor,                  /* The major token to shift in */
  snlParserTOKENTYPE yyMinor        /* The minor token to shift in */
){
  yyStackEntry *yytos;
  yypParser->yytos++;
#ifdef YYTRACKMAXSTACKDEPTH
  if( (int)(yypParser->yytos - yypParser->yystack)>yypParser->yyhwm ){
    yypParser->yyhwm++;
    assert( yypParser->yyhwm == (int)(yypParser->yytos - yypParser->yystack) );
  }
#endif
#if YYSTACKDEPTH>0 
  if( yypParser->yytos>=&yypParser->yystack[YYSTACKDEPTH] ){
    yyStackOverflow(yypParser);
    return;
  }
#else
  if( yypParser->yytos>=&yypParser->yystack[yypParser->yystksz] ){
    if( yyGrowStack(yypParser) ){
      yyStackOverflow(yypParser);
      return;
    }
  }
#endif
  if( yyNewState > YY_MAX_SHIFT ){
    yyNewState += YY_MIN_REDUCE - YY_MIN_SHIFTREDUCE;
  }
  yytos = yypParser->yytos;
  yytos->stateno = (YYACTIONTYPE)yyNewState;
  yytos->major = (YYCODETYPE)yyMajor;
  yytos->minor.yy0 = yyMinor;
  yyTraceShift(yypParser, yyNewState);
}

/* The following table contains information about every rule that
** is used during the reduce.
*/
static const struct {
  YYCODETYPE lhs;         /* Symbol on the left-hand side of the rule */
  unsigned char nrhs;     /* Number of right-hand side symbols in the rule */
} yyRuleInfo[] = {
  { 96, 8 },
  { 97, 3 },
  { 97, 0 },
  { 98, 2 },
  { 98, 0 },
  { 104, 1 },
  { 104, 1 },
  { 104, 1 },
  { 104, 1 },
  { 104, 1 },
  { 104, 1 },
  { 104, 1 },
  { 104, 1 },
  { 104, 1 },
  { 102, 2 },
  { 102, 0 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 105, 5 },
  { 105, 6 },
  { 105, 7 },
  { 105, 3 },
  { 118, 3 },
  { 118, 1 },
  { 118, 0 },
  { 106, 4 },
  { 106, 5 },
  { 107, 6 },
  { 107, 7 },
  { 108, 7 },
  { 108, 5 },
  { 120, 1 },
  { 115, 1 },
  { 121, 1 },
  { 121, 0 },
  { 119, 1 },
  { 119, 0 },
  { 117, 3 },
  { 109, 3 },
  { 123, 1 },
  { 123, 3 },
  { 124, 1 },
  { 124, 3 },
  { 125, 1 },
  { 125, 2 },
  { 125, 4 },
  { 125, 3 },
  { 125, 2 },
  { 125, 2 },
  { 127, 0 },
  { 127, 1 },
  { 127, 3 },
  { 128, 2 },
  { 128, 1 },
  { 109, 3 },
  { 130, 1 },
  { 130, 3 },
  { 126, 6 },
  { 126, 3 },
  { 126, 1 },
  { 131, 3 },
  { 131, 1 },
  { 131, 0 },
  { 133, 1 },
  { 133, 1 },
  { 133, 1 },
  { 133, 1 },
  { 133, 2 },
  { 133, 2 },
  { 133, 2 },
  { 133, 2 },
  { 133, 1 },
  { 133, 1 },
  { 133, 1 },
  { 133, 1 },
  { 133, 1 },
  { 133, 1 },
  { 133, 1 },
  { 133, 1 },
  { 133, 1 },
  { 122, 1 },
  { 122, 1 },
  { 122, 1 },
  { 122, 2 },
  { 122, 2 },
  { 122, 2 },
  { 122, 2 },
  { 129, 1 },
  { 129, 2 },
  { 134, 3 },
  { 134, 1 },
  { 134, 2 },
  { 134, 1 },
  { 134, 2 },
  { 134, 1 },
  { 134, 2 },
  { 134, 3 },
  { 134, 4 },
  { 110, 4 },
  { 135, 1 },
  { 135, 1 },
  { 100, 2 },
  { 100, 1 },
  { 136, 6 },
  { 137, 2 },
  { 137, 0 },
  { 139, 1 },
  { 139, 1 },
  { 139, 1 },
  { 139, 1 },
  { 139, 1 },
  { 138, 2 },
  { 138, 1 },
  { 140, 8 },
  { 141, 2 },
  { 141, 0 },
  { 143, 1 },
  { 143, 1 },
  { 143, 1 },
  { 143, 1 },
  { 143, 1 },
  { 143, 1 },
  { 99, 2 },
  { 99, 0 },
  { 101, 2 },
  { 101, 0 },
  { 142, 2 },
  { 142, 1 },
  { 145, 7 },
  { 145, 6 },
  { 145, 6 },
  { 146, 1 },
  { 144, 4 },
  { 148, 2 },
  { 148, 0 },
  { 150, 1 },
  { 150, 1 },
  { 149, 2 },
  { 149, 0 },
  { 151, 2 },
  { 151, 2 },
  { 151, 3 },
  { 151, 3 },
  { 151, 1 },
  { 151, 1 },
  { 151, 5 },
  { 151, 7 },
  { 151, 5 },
  { 151, 1 },
  { 151, 2 },
  { 153, 9 },
  { 132, 1 },
  { 132, 1 },
  { 132, 1 },
  { 132, 1 },
  { 132, 3 },
  { 132, 4 },
  { 132, 4 },
  { 132, 4 },
  { 132, 4 },
  { 132, 3 },
  { 132, 3 },
  { 132, 2 },
  { 132, 2 },
  { 132, 2 },
  { 132, 2 },
  { 132, 2 },
  { 132, 2 },
  { 132, 2 },
  { 132, 2 },
  { 132, 2 },
  { 132, 2 },
  { 132, 2 },
  { 132, 4 },
  { 132, 3 },
  { 132, 3 },
  { 132, 3 },
  { 132, 3 },
  { 132, 3 },
  { 132, 3 },
  { 132, 3 },
  { 132, 3 },
  { 132, 3 },
  { 132, 3 },
  { 132, 3 },
  { 132, 3 },
  { 132, 3 },
  { 132, 3 },
  { 132, 3 },
  { 132, 3 },
  { 132, 3 },
  { 132, 3 },
  { 132, 5 },
  { 132, 3 },
  { 132, 3 },
  { 132, 3 },
  { 132, 3 },
  { 132, 3 },
  { 132, 3 },
  { 132, 3 },
  { 132, 3 },
  { 132, 3 },
  { 132, 3 },
  { 132, 3 },
  { 152, 3 },
  { 152, 1 },
  { 147, 1 },
  { 147, 0 },
  { 154, 3 },
  { 154, 1 },
  { 154, 0 },
  { 103, 1 },
  { 155, 1 },
  { 112, 3 },
  { 113, 4 },
  { 156, 3 },
  { 157, 1 },
  { 157, 2 },
  { 158, 3 },
  { 158, 1 },
  { 111, 1 },
  { 116, 1 },
  { 116, 0 },
};

static void yy_accept(yyParser*);  /* Forward Declaration */

/*
** Perform a reduce action and the shift that must immediately
** follow the reduce.
*/
static void yy_reduce(
  yyParser *yypParser,         /* The parser */
  unsigned int yyruleno        /* Number of the rule by which to reduce */
){
  int yygoto;                     /* The next state */
  int yyact;                      /* The next action */
  yyStackEntry *yymsp;            /* The top of the parser's stack */
  int yysize;                     /* Amount to pop the stack */
  snlParserARG_FETCH;
  yymsp = yypParser->yytos;
#ifndef NDEBUG
  if( yyTraceFILE && yyruleno<(int)(sizeof(yyRuleName)/sizeof(yyRuleName[0])) ){
    yysize = yyRuleInfo[yyruleno].nrhs;
    fprintf(yyTraceFILE, "%sReduce [%s], go to state %d.\n", yyTracePrompt,
      yyRuleName[yyruleno], yymsp[-yysize].stateno);
  }
#endif /* NDEBUG */

  /* Check that the stack is large enough to grow by a single entry
  ** if the RHS of the rule is empty.  This ensures that there is room
  ** enough on the stack to push the LHS value */
  if( yyRuleInfo[yyruleno].nrhs==0 ){
#ifdef YYTRACKMAXSTACKDEPTH
    if( (int)(yypParser->yytos - yypParser->yystack)>yypParser->yyhwm ){
      yypParser->yyhwm++;
      assert( yypParser->yyhwm == (int)(yypParser->yytos - yypParser->yystack));
    }
#endif
#if YYSTACKDEPTH>0 
    if( yypParser->yytos>=&yypParser->yystack[YYSTACKDEPTH-1] ){
      yyStackOverflow(yypParser);
      return;
    }
#else
    if( yypParser->yytos>=&yypParser->yystack[yypParser->yystksz-1] ){
      if( yyGrowStack(yypParser) ){
        yyStackOverflow(yypParser);
        return;
      }
      yymsp = yypParser->yytos;
    }
#endif
  }

  switch( yyruleno ){
  /* Beginning here are the reduction cases.  A typical example
  ** follows:
  **   case 0:
  **  #line <lineno> <grammarfile>
  **     { ... }           // User supplied code
  **  #line <lineno> <thisfile>
  **     break;
  */
/********** Begin reduce actions **********************************************/
        YYMINORTYPE yylhsminor;
      case 0: /* program ::= PROGRAM NAME program_param initial_defns entry state_sets exit final_defns */
#line 101 "../snl.lem"
{
	*presult =  node(D_PROG, yymsp[-6].minor.yy0, yymsp[-5].minor.yy255, yymsp[-4].minor.yy255, yymsp[-3].minor.yy255, yymsp[-2].minor.yy255, yymsp[-1].minor.yy255, yymsp[0].minor.yy255);
}
#line 1581 "./snl.c"
        break;
      case 1: /* program_param ::= LPAREN string RPAREN */
      case 47: /* declarator ::= LPAREN declarator RPAREN */ yytestcase(yyruleno==47);
      case 90: /* abs_decl ::= LPAREN abs_decl RPAREN */ yytestcase(yyruleno==90);
      case 216: /* members ::= LBRACE member_decls RBRACE */ yytestcase(yyruleno==216);
#line 105 "../snl.lem"
{ yymsp[-2].minor.yy255 = yymsp[-1].minor.yy255; }
#line 1589 "./snl.c"
        break;
      case 2: /* program_param ::= */
      case 4: /* initial_defns ::= */ yytestcase(yyruleno==4);
      case 15: /* final_defns ::= */ yytestcase(yyruleno==15);
      case 25: /* strings ::= */ yytestcase(yyruleno==25);
      case 35: /* syncq_size ::= */ yytestcase(yyruleno==35);
      case 37: /* opt_subscript ::= */ yytestcase(yyruleno==37);
      case 50: /* param_decls ::= */ yytestcase(yyruleno==50);
      case 63: /* init_exprs ::= */ yytestcase(yyruleno==63);
      case 106: /* ss_defns ::= */ yytestcase(yyruleno==106);
      case 116: /* state_defns ::= */ yytestcase(yyruleno==116);
      case 124: /* entry ::= */ yytestcase(yyruleno==124);
      case 126: /* exit ::= */ yytestcase(yyruleno==126);
      case 135: /* block_defns ::= */ yytestcase(yyruleno==135);
      case 139: /* statements ::= */ yytestcase(yyruleno==139);
      case 208: /* opt_expr ::= */ yytestcase(yyruleno==208);
      case 211: /* args ::= */ yytestcase(yyruleno==211);
#line 106 "../snl.lem"
{ yymsp[1].minor.yy255 = 0; }
#line 1609 "./snl.c"
        break;
      case 3: /* initial_defns ::= initial_defns initial_defn */
      case 14: /* final_defns ::= final_defns final_defn */ yytestcase(yyruleno==14);
      case 115: /* state_defns ::= state_defns state_defn */ yytestcase(yyruleno==115);
      case 134: /* block_defns ::= block_defns block_defn */ yytestcase(yyruleno==134);
#line 110 "../snl.lem"
{
	yylhsminor.yy255 = link_node(yymsp[-1].minor.yy255, yymsp[0].minor.yy255);
}
#line 1619 "./snl.c"
  yymsp[-1].minor.yy255 = yylhsminor.yy255;
        break;
      case 5: /* initial_defn ::= assign */
      case 6: /* initial_defn ::= monitor */ yytestcase(yyruleno==6);
      case 7: /* initial_defn ::= sync */ yytestcase(yyruleno==7);
      case 8: /* initial_defn ::= syncq */ yytestcase(yyruleno==8);
      case 9: /* initial_defn ::= declaration */ yytestcase(yyruleno==9);
      case 10: /* initial_defn ::= option */ yytestcase(yyruleno==10);
      case 11: /* initial_defn ::= c_code */ yytestcase(yyruleno==11);
      case 12: /* initial_defn ::= funcdef */ yytestcase(yyruleno==12);
      case 13: /* initial_defn ::= structdef */ yytestcase(yyruleno==13);
      case 16: /* final_defn ::= c_code */ yytestcase(yyruleno==16);
      case 17: /* final_defn ::= funcdef */ yytestcase(yyruleno==17);
      case 18: /* final_defn ::= structdef */ yytestcase(yyruleno==18);
      case 24: /* strings ::= string */ yytestcase(yyruleno==24);
      case 40: /* init_declarators ::= init_declarator */ yytestcase(yyruleno==40);
      case 42: /* init_declarator ::= declarator */ yytestcase(yyruleno==42);
      case 51: /* param_decls ::= param_decl */ yytestcase(yyruleno==51);
      case 54: /* param_decl ::= type_expr */ yytestcase(yyruleno==54);
      case 60: /* init_expr ::= expr */ yytestcase(yyruleno==60);
      case 62: /* init_exprs ::= init_expr */ yytestcase(yyruleno==62);
      case 103: /* state_sets ::= state_set */ yytestcase(yyruleno==103);
      case 107: /* ss_defn ::= assign */ yytestcase(yyruleno==107);
      case 108: /* ss_defn ::= monitor */ yytestcase(yyruleno==108);
      case 109: /* ss_defn ::= sync */ yytestcase(yyruleno==109);
      case 110: /* ss_defn ::= syncq */ yytestcase(yyruleno==110);
      case 111: /* ss_defn ::= declaration */ yytestcase(yyruleno==111);
      case 113: /* states ::= state */ yytestcase(yyruleno==113);
      case 117: /* state_defn ::= assign */ yytestcase(yyruleno==117);
      case 118: /* state_defn ::= monitor */ yytestcase(yyruleno==118);
      case 119: /* state_defn ::= sync */ yytestcase(yyruleno==119);
      case 120: /* state_defn ::= syncq */ yytestcase(yyruleno==120);
      case 121: /* state_defn ::= declaration */ yytestcase(yyruleno==121);
      case 122: /* state_defn ::= option */ yytestcase(yyruleno==122);
      case 128: /* transitions ::= transition */ yytestcase(yyruleno==128);
      case 132: /* condition ::= opt_expr */ yytestcase(yyruleno==132);
      case 136: /* block_defn ::= declaration */ yytestcase(yyruleno==136);
      case 137: /* block_defn ::= c_code */ yytestcase(yyruleno==137);
      case 144: /* statement ::= c_code */ yytestcase(yyruleno==144);
      case 145: /* statement ::= block */ yytestcase(yyruleno==145);
      case 149: /* statement ::= for_statement */ yytestcase(yyruleno==149);
      case 154: /* expr ::= string */ yytestcase(yyruleno==154);
      case 206: /* comma_expr ::= expr */ yytestcase(yyruleno==206);
      case 207: /* opt_expr ::= comma_expr */ yytestcase(yyruleno==207);
      case 210: /* args ::= expr */ yytestcase(yyruleno==210);
      case 217: /* member_decls ::= member_decl */ yytestcase(yyruleno==217);
      case 220: /* member_decl ::= c_code */ yytestcase(yyruleno==220);
#line 115 "../snl.lem"
{ yylhsminor.yy255 = yymsp[0].minor.yy255; }
#line 1669 "./snl.c"
  yymsp[0].minor.yy255 = yylhsminor.yy255;
        break;
      case 19: /* assign ::= ASSIGN variable to string SEMICOLON */
#line 134 "../snl.lem"
{
	yymsp[-4].minor.yy255 = node(D_ASSIGN, yymsp[-3].minor.yy270, NIL, yymsp[-1].minor.yy255);
}
#line 1677 "./snl.c"
        break;
      case 20: /* assign ::= ASSIGN variable subscript to string SEMICOLON */
#line 137 "../snl.lem"
{
	yymsp[-5].minor.yy255 = node(D_ASSIGN, yymsp[-4].minor.yy270, node(E_CONST, yymsp[-3].minor.yy270), yymsp[-1].minor.yy255);
}
#line 1684 "./snl.c"
        break;
      case 21: /* assign ::= ASSIGN variable to LBRACE strings RBRACE SEMICOLON */
#line 140 "../snl.lem"
{
	yymsp[-6].minor.yy255 = node(D_ASSIGN, yymsp[-5].minor.yy270, NIL, node(E_INIT, yymsp[-3].minor.yy0, yymsp[-2].minor.yy255));
}
#line 1691 "./snl.c"
        break;
      case 22: /* assign ::= ASSIGN variable SEMICOLON */
#line 143 "../snl.lem"
{
	yymsp[-2].minor.yy255 = node(D_ASSIGN, yymsp[-1].minor.yy270, NIL, NIL);
}
#line 1698 "./snl.c"
        break;
      case 23: /* strings ::= strings COMMA string */
      case 41: /* init_declarators ::= init_declarators COMMA init_declarator */ yytestcase(yyruleno==41);
      case 52: /* param_decls ::= param_decls COMMA param_decl */ yytestcase(yyruleno==52);
      case 61: /* init_exprs ::= init_exprs COMMA init_expr */ yytestcase(yyruleno==61);
      case 209: /* args ::= args COMMA expr */ yytestcase(yyruleno==209);
#line 150 "../snl.lem"
{ yylhsminor.yy255 = link_node(yymsp[-2].minor.yy255, yymsp[0].minor.yy255); }
#line 1707 "./snl.c"
  yymsp[-2].minor.yy255 = yylhsminor.yy255;
        break;
      case 26: /* monitor ::= MONITOR variable opt_subscript SEMICOLON */
#line 154 "../snl.lem"
{
	yymsp[-3].minor.yy255 = node(D_MONITOR, yymsp[-2].minor.yy270, yymsp[-1].minor.yy255);
}
#line 1715 "./snl.c"
        break;
      case 27: /* monitor ::= MONITOR variable opt_subscript error SEMICOLON */
#line 157 "../snl.lem"
{
	yymsp[-4].minor.yy255 = node(D_MONITOR, yymsp[-3].minor.yy270, yymsp[-2].minor.yy255);
	report("expected %s';'\n", yymsp[-2].minor.yy255 ? "subscript or " : "");
}
#line 1723 "./snl.c"
        break;
      case 28: /* sync ::= SYNC variable opt_subscript to event_flag SEMICOLON */
#line 162 "../snl.lem"
{
	yymsp[-5].minor.yy255 = node(D_SYNC, yymsp[-4].minor.yy270, yymsp[-3].minor.yy255, node(E_VAR, yymsp[-1].minor.yy270), NIL);
}
#line 1730 "./snl.c"
        break;
      case 29: /* sync ::= SYNC variable opt_subscript to event_flag error SEMICOLON */
#line 165 "../snl.lem"
{
	yymsp[-6].minor.yy255 = node(D_SYNC, yymsp[-5].minor.yy270, yymsp[-4].minor.yy255, node(E_VAR, yymsp[-2].minor.yy270), NIL);
	report("expected ';'\n");
}
#line 1738 "./snl.c"
        break;
      case 30: /* syncq ::= SYNCQ variable opt_subscript to event_flag syncq_size SEMICOLON */
#line 170 "../snl.lem"
{
	yymsp[-6].minor.yy255 = node(D_SYNCQ, yymsp[-5].minor.yy270, yymsp[-4].minor.yy255, node(E_VAR, yymsp[-2].minor.yy270), yymsp[-1].minor.yy255);
}
#line 1745 "./snl.c"
        break;
      case 31: /* syncq ::= SYNCQ variable opt_subscript syncq_size SEMICOLON */
#line 173 "../snl.lem"
{
	yymsp[-4].minor.yy255 = node(D_SYNCQ, yymsp[-3].minor.yy270, yymsp[-2].minor.yy255, NIL, yymsp[-1].minor.yy255);
}
#line 1752 "./snl.c"
        break;
      case 32: /* event_flag ::= NAME */
      case 33: /* variable ::= NAME */ yytestcase(yyruleno==33);
      case 100: /* option_value ::= ADD */ yytestcase(yyruleno==100);
      case 101: /* option_value ::= SUB */ yytestcase(yyruleno==101);
#line 178 "../snl.lem"
{ yylhsminor.yy270 = yymsp[0].minor.yy0; }
#line 1760 "./snl.c"
  yymsp[0].minor.yy270 = yylhsminor.yy270;
        break;
      case 34: /* syncq_size ::= INTCON */
      case 152: /* expr ::= INTCON */ yytestcase(yyruleno==152);
      case 153: /* expr ::= FPCON */ yytestcase(yyruleno==153);
#line 182 "../snl.lem"
{ yylhsminor.yy255 = node(E_CONST, yymsp[0].minor.yy0); }
#line 1768 "./snl.c"
  yymsp[0].minor.yy255 = yylhsminor.yy255;
        break;
      case 36: /* opt_subscript ::= subscript */
#line 185 "../snl.lem"
{ yylhsminor.yy255 = node(E_CONST, yymsp[0].minor.yy270); }
#line 1774 "./snl.c"
  yymsp[0].minor.yy255 = yylhsminor.yy255;
        break;
      case 38: /* subscript ::= LBRACKET INTCON RBRACKET */
#line 189 "../snl.lem"
{ yymsp[-2].minor.yy270 = yymsp[-1].minor.yy0; }
#line 1780 "./snl.c"
        break;
      case 39: /* declaration ::= basetype init_declarators SEMICOLON */
#line 194 "../snl.lem"
{ yylhsminor.yy255 = mk_decls(yymsp[-1].minor.yy255, yymsp[-2].minor.yy149); }
#line 1785 "./snl.c"
  yymsp[-2].minor.yy255 = yylhsminor.yy255;
        break;
      case 43: /* init_declarator ::= declarator EQUAL init_expr */
      case 175: /* expr ::= expr SUB expr */ yytestcase(yyruleno==175);
      case 176: /* expr ::= expr ADD expr */ yytestcase(yyruleno==176);
      case 177: /* expr ::= expr ASTERISK expr */ yytestcase(yyruleno==177);
      case 178: /* expr ::= expr SLASH expr */ yytestcase(yyruleno==178);
      case 179: /* expr ::= expr GT expr */ yytestcase(yyruleno==179);
      case 180: /* expr ::= expr GE expr */ yytestcase(yyruleno==180);
      case 181: /* expr ::= expr EQ expr */ yytestcase(yyruleno==181);
      case 182: /* expr ::= expr NE expr */ yytestcase(yyruleno==182);
      case 183: /* expr ::= expr LE expr */ yytestcase(yyruleno==183);
      case 184: /* expr ::= expr LT expr */ yytestcase(yyruleno==184);
      case 185: /* expr ::= expr OROR expr */ yytestcase(yyruleno==185);
      case 186: /* expr ::= expr ANDAND expr */ yytestcase(yyruleno==186);
      case 187: /* expr ::= expr LSHIFT expr */ yytestcase(yyruleno==187);
      case 188: /* expr ::= expr RSHIFT expr */ yytestcase(yyruleno==188);
      case 189: /* expr ::= expr VBAR expr */ yytestcase(yyruleno==189);
      case 190: /* expr ::= expr CARET expr */ yytestcase(yyruleno==190);
      case 191: /* expr ::= expr AMPERSAND expr */ yytestcase(yyruleno==191);
      case 192: /* expr ::= expr MOD expr */ yytestcase(yyruleno==192);
      case 194: /* expr ::= expr EQUAL expr */ yytestcase(yyruleno==194);
      case 195: /* expr ::= expr ADDEQ expr */ yytestcase(yyruleno==195);
      case 196: /* expr ::= expr SUBEQ expr */ yytestcase(yyruleno==196);
      case 197: /* expr ::= expr ANDEQ expr */ yytestcase(yyruleno==197);
      case 198: /* expr ::= expr OREQ expr */ yytestcase(yyruleno==198);
      case 199: /* expr ::= expr DIVEQ expr */ yytestcase(yyruleno==199);
      case 200: /* expr ::= expr MULEQ expr */ yytestcase(yyruleno==200);
      case 201: /* expr ::= expr MODEQ expr */ yytestcase(yyruleno==201);
      case 202: /* expr ::= expr LSHEQ expr */ yytestcase(yyruleno==202);
      case 203: /* expr ::= expr RSHEQ expr */ yytestcase(yyruleno==203);
      case 204: /* expr ::= expr XOREQ expr */ yytestcase(yyruleno==204);
      case 205: /* comma_expr ::= comma_expr COMMA expr */ yytestcase(yyruleno==205);
#line 202 "../snl.lem"
{ yylhsminor.yy255 = node(E_BINOP, yymsp[-1].minor.yy0, yymsp[-2].minor.yy255, yymsp[0].minor.yy255); }
#line 1821 "./snl.c"
  yymsp[-2].minor.yy255 = yylhsminor.yy255;
        break;
      case 44: /* declarator ::= variable */
      case 56: /* variables ::= variable */ yytestcase(yyruleno==56);
      case 155: /* expr ::= variable */ yytestcase(yyruleno==155);
#line 204 "../snl.lem"
{ yylhsminor.yy255 = node(E_VAR, yymsp[0].minor.yy270); }
#line 1829 "./snl.c"
  yymsp[0].minor.yy255 = yylhsminor.yy255;
        break;
      case 45: /* declarator ::= declarator subscript */
      case 96: /* abs_decl ::= abs_decl subscript */ yytestcase(yyruleno==96);
#line 205 "../snl.lem"
{ yylhsminor.yy255 = node(E_SUBSCR, yymsp[0].minor.yy270, yymsp[-1].minor.yy255, node(E_CONST, yymsp[0].minor.yy270)); }
#line 1836 "./snl.c"
  yymsp[-1].minor.yy255 = yylhsminor.yy255;
        break;
      case 46: /* declarator ::= declarator LPAREN param_decls RPAREN */
      case 98: /* abs_decl ::= abs_decl LPAREN param_decls RPAREN */ yytestcase(yyruleno==98);
#line 207 "../snl.lem"
{ yylhsminor.yy255 = node(E_FUNC, yymsp[-2].minor.yy0, yymsp[-3].minor.yy255, yymsp[-1].minor.yy255); }
#line 1843 "./snl.c"
  yymsp[-3].minor.yy255 = yylhsminor.yy255;
        break;
      case 48: /* declarator ::= ASTERISK declarator */
      case 49: /* declarator ::= CONST declarator */ yytestcase(yyruleno==49);
      case 92: /* abs_decl ::= ASTERISK abs_decl */ yytestcase(yyruleno==92);
      case 94: /* abs_decl ::= CONST abs_decl */ yytestcase(yyruleno==94);
      case 165: /* expr ::= ADD expr */ yytestcase(yyruleno==165);
      case 166: /* expr ::= SUB expr */ yytestcase(yyruleno==166);
      case 167: /* expr ::= ASTERISK expr */ yytestcase(yyruleno==167);
      case 168: /* expr ::= AMPERSAND expr */ yytestcase(yyruleno==168);
      case 169: /* expr ::= NOT expr */ yytestcase(yyruleno==169);
      case 170: /* expr ::= TILDE expr */ yytestcase(yyruleno==170);
      case 171: /* expr ::= INCR expr */ yytestcase(yyruleno==171);
      case 172: /* expr ::= DECR expr */ yytestcase(yyruleno==172);
#line 209 "../snl.lem"
{ yylhsminor.yy255 = node(E_PRE, yymsp[-1].minor.yy0, yymsp[0].minor.yy255); }
#line 1860 "./snl.c"
  yymsp[-1].minor.yy255 = yylhsminor.yy255;
        break;
      case 53: /* param_decl ::= basetype declarator */
      case 89: /* type_expr ::= basetype abs_decl */ yytestcase(yyruleno==89);
#line 217 "../snl.lem"
{ yylhsminor.yy255 = mk_decl(yymsp[0].minor.yy255, yymsp[-1].minor.yy149); }
#line 1867 "./snl.c"
  yymsp[-1].minor.yy255 = yylhsminor.yy255;
        break;
      case 55: /* declaration ::= FOREIGN variables SEMICOLON */
#line 221 "../snl.lem"
{ yymsp[-2].minor.yy255 = mk_decls(yymsp[-1].minor.yy255, mk_no_type()); }
#line 1873 "./snl.c"
        break;
      case 57: /* variables ::= variables COMMA variable */
#line 224 "../snl.lem"
{ yylhsminor.yy255 = link_node(yymsp[-2].minor.yy255, node(E_VAR, yymsp[0].minor.yy270)); }
#line 1878 "./snl.c"
  yymsp[-2].minor.yy255 = yylhsminor.yy255;
        break;
      case 58: /* init_expr ::= LPAREN type_expr RPAREN LBRACE init_exprs RBRACE */
#line 229 "../snl.lem"
{ yylhsminor.yy255 = node(E_CAST, yymsp[-5].minor.yy0, yymsp[-4].minor.yy255, node(E_INIT, yymsp[-2].minor.yy0, yymsp[-1].minor.yy255)); }
#line 1884 "./snl.c"
  yymsp[-5].minor.yy255 = yylhsminor.yy255;
        break;
      case 59: /* init_expr ::= LBRACE init_exprs RBRACE */
#line 230 "../snl.lem"
{ yylhsminor.yy255 = node(E_INIT, yymsp[-2].minor.yy0, yymsp[-1].minor.yy255); }
#line 1890 "./snl.c"
  yymsp[-2].minor.yy255 = yylhsminor.yy255;
        break;
      case 64: /* prim_type ::= CHAR */
#line 241 "../snl.lem"
{ yymsp[0].minor.yy6 = P_CHAR; }
#line 1896 "./snl.c"
        break;
      case 65: /* prim_type ::= SHORT */
#line 242 "../snl.lem"
{ yymsp[0].minor.yy6 = P_SHORT; }
#line 1901 "./snl.c"
        break;
      case 66: /* prim_type ::= INT */
#line 243 "../snl.lem"
{ yymsp[0].minor.yy6 = P_INT; }
#line 1906 "./snl.c"
        break;
      case 67: /* prim_type ::= LONG */
#line 244 "../snl.lem"
{ yymsp[0].minor.yy6 = P_LONG; }
#line 1911 "./snl.c"
        break;
      case 68: /* prim_type ::= UNSIGNED CHAR */
#line 245 "../snl.lem"
{ yymsp[-1].minor.yy6 = P_UCHAR; }
#line 1916 "./snl.c"
        break;
      case 69: /* prim_type ::= UNSIGNED SHORT */
#line 246 "../snl.lem"
{ yymsp[-1].minor.yy6 = P_USHORT; }
#line 1921 "./snl.c"
        break;
      case 70: /* prim_type ::= UNSIGNED INT */
#line 247 "../snl.lem"
{ yymsp[-1].minor.yy6 = P_UINT; }
#line 1926 "./snl.c"
        break;
      case 71: /* prim_type ::= UNSIGNED LONG */
#line 248 "../snl.lem"
{ yymsp[-1].minor.yy6 = P_ULONG; }
#line 1931 "./snl.c"
        break;
      case 72: /* prim_type ::= INT8T */
#line 249 "../snl.lem"
{ yymsp[0].minor.yy6 = P_INT8T; }
#line 1936 "./snl.c"
        break;
      case 73: /* prim_type ::= UINT8T */
#line 250 "../snl.lem"
{ yymsp[0].minor.yy6 = P_UINT8T; }
#line 1941 "./snl.c"
        break;
      case 74: /* prim_type ::= INT16T */
#line 251 "../snl.lem"
{ yymsp[0].minor.yy6 = P_INT16T; }
#line 1946 "./snl.c"
        break;
      case 75: /* prim_type ::= UINT16T */
#line 252 "../snl.lem"
{ yymsp[0].minor.yy6 = P_UINT16T; }
#line 1951 "./snl.c"
        break;
      case 76: /* prim_type ::= INT32T */
#line 253 "../snl.lem"
{ yymsp[0].minor.yy6 = P_INT32T; }
#line 1956 "./snl.c"
        break;
      case 77: /* prim_type ::= UINT32T */
#line 254 "../snl.lem"
{ yymsp[0].minor.yy6 = P_UINT32T; }
#line 1961 "./snl.c"
        break;
      case 78: /* prim_type ::= FLOAT */
#line 255 "../snl.lem"
{ yymsp[0].minor.yy6 = P_FLOAT; }
#line 1966 "./snl.c"
        break;
      case 79: /* prim_type ::= DOUBLE */
#line 256 "../snl.lem"
{ yymsp[0].minor.yy6 = P_DOUBLE; }
#line 1971 "./snl.c"
        break;
      case 80: /* prim_type ::= STRING */
#line 257 "../snl.lem"
{ yymsp[0].minor.yy6 = P_STRING; }
#line 1976 "./snl.c"
        break;
      case 81: /* basetype ::= prim_type */
#line 260 "../snl.lem"
{ yylhsminor.yy149 = mk_prim_type(yymsp[0].minor.yy6); }
#line 1981 "./snl.c"
  yymsp[0].minor.yy149 = yylhsminor.yy149;
        break;
      case 82: /* basetype ::= EVFLAG */
#line 261 "../snl.lem"
{ yymsp[0].minor.yy149 = mk_ef_type(); }
#line 1987 "./snl.c"
        break;
      case 83: /* basetype ::= VOID */
#line 262 "../snl.lem"
{ yymsp[0].minor.yy149 = mk_void_type(); }
#line 1992 "./snl.c"
        break;
      case 84: /* basetype ::= ENUM NAME */
#line 263 "../snl.lem"
{ yymsp[-1].minor.yy149 = mk_foreign_type(F_ENUM, yymsp[0].minor.yy0.str); }
#line 1997 "./snl.c"
        break;
      case 85: /* basetype ::= STRUCT NAME */
#line 264 "../snl.lem"
{ yymsp[-1].minor.yy149 = mk_foreign_type(F_STRUCT, yymsp[0].minor.yy0.str); }
#line 2002 "./snl.c"
        break;
      case 86: /* basetype ::= UNION NAME */
#line 265 "../snl.lem"
{ yymsp[-1].minor.yy149 = mk_foreign_type(F_UNION, yymsp[0].minor.yy0.str); }
#line 2007 "./snl.c"
        break;
      case 87: /* basetype ::= TYPENAME NAME */
#line 266 "../snl.lem"
{ yymsp[-1].minor.yy149 = mk_foreign_type(F_TYPENAME, yymsp[0].minor.yy0.str); }
#line 2012 "./snl.c"
        break;
      case 88: /* type_expr ::= basetype */
#line 268 "../snl.lem"
{ yylhsminor.yy255 = mk_decl(0, yymsp[0].minor.yy149); }
#line 2017 "./snl.c"
  yymsp[0].minor.yy255 = yylhsminor.yy255;
        break;
      case 91: /* abs_decl ::= ASTERISK */
      case 93: /* abs_decl ::= CONST */ yytestcase(yyruleno==93);
#line 273 "../snl.lem"
{ yylhsminor.yy255 = node(E_PRE, yymsp[0].minor.yy0, NIL); }
#line 2024 "./snl.c"
  yymsp[0].minor.yy255 = yylhsminor.yy255;
        break;
      case 95: /* abs_decl ::= subscript */
#line 277 "../snl.lem"
{ yylhsminor.yy255 = node(E_SUBSCR, yymsp[0].minor.yy270, NIL, node(E_CONST, yymsp[0].minor.yy270)); }
#line 2030 "./snl.c"
  yymsp[0].minor.yy255 = yylhsminor.yy255;
        break;
      case 97: /* abs_decl ::= LPAREN param_decls RPAREN */
#line 280 "../snl.lem"
{ yylhsminor.yy255 = node(E_FUNC, yymsp[-2].minor.yy0, NIL, yymsp[-1].minor.yy255); }
#line 2036 "./snl.c"
  yymsp[-2].minor.yy255 = yylhsminor.yy255;
        break;
      case 99: /* option ::= OPTION option_value NAME SEMICOLON */
#line 293 "../snl.lem"
{ yymsp[-3].minor.yy255 = opt_defn(yymsp[-1].minor.yy0, yymsp[-2].minor.yy270); }
#line 2042 "./snl.c"
        break;
      case 102: /* state_sets ::= state_sets state_set */
      case 105: /* ss_defns ::= ss_defns ss_defn */ yytestcase(yyruleno==105);
      case 112: /* states ::= states state */ yytestcase(yyruleno==112);
      case 127: /* transitions ::= transitions transition */ yytestcase(yyruleno==127);
      case 138: /* statements ::= statements statement */ yytestcase(yyruleno==138);
      case 218: /* member_decls ::= member_decls member_decl */ yytestcase(yyruleno==218);
#line 301 "../snl.lem"
{ yylhsminor.yy255 = link_node(yymsp[-1].minor.yy255, yymsp[0].minor.yy255); }
#line 2052 "./snl.c"
  yymsp[-1].minor.yy255 = yylhsminor.yy255;
        break;
      case 104: /* state_set ::= SS NAME LBRACE ss_defns states RBRACE */
#line 305 "../snl.lem"
{ yymsp[-5].minor.yy255 = node(D_SS, yymsp[-4].minor.yy0, yymsp[-2].minor.yy255, yymsp[-1].minor.yy255); }
#line 2058 "./snl.c"
        break;
      case 114: /* state ::= STATE NAME LBRACE state_defns entry transitions exit RBRACE */
#line 322 "../snl.lem"
{
	yymsp[-7].minor.yy255 = node(D_STATE, yymsp[-6].minor.yy0, yymsp[-4].minor.yy255, yymsp[-3].minor.yy255, yymsp[-2].minor.yy255, yymsp[-1].minor.yy255);
}
#line 2065 "./snl.c"
        break;
      case 123: /* entry ::= ENTRY block */
      case 125: /* exit ::= EXIT block */ yytestcase(yyruleno==125);
#line 338 "../snl.lem"
{ yylhsminor.yy255 = node(D_ENTEX, yymsp[-1].minor.yy0, yymsp[0].minor.yy255); }
#line 2071 "./snl.c"
  yymsp[-1].minor.yy255 = yylhsminor.yy255;
        break;
      case 129: /* transition ::= WHEN LPAREN condition RPAREN block STATE NAME */
#line 347 "../snl.lem"
{
	yymsp[-6].minor.yy0.str = yymsp[0].minor.yy0.str;
	yylhsminor.yy255 = node(D_WHEN, yymsp[-6].minor.yy0, yymsp[-4].minor.yy255, yymsp[-2].minor.yy255);
}
#line 2080 "./snl.c"
  yymsp[-6].minor.yy255 = yylhsminor.yy255;
        break;
      case 130: /* transition ::= WHEN LPAREN condition RPAREN block EXIT */
#line 351 "../snl.lem"
{
	yymsp[-5].minor.yy0.str = 0;
	yylhsminor.yy255 = node(D_WHEN, yymsp[-5].minor.yy0, yymsp[-3].minor.yy255, yymsp[-1].minor.yy255);
}
#line 2089 "./snl.c"
  yymsp[-5].minor.yy255 = yylhsminor.yy255;
        break;
      case 131: /* transition ::= WHEN LPAREN condition RPAREN block error */
#line 355 "../snl.lem"
{
	yymsp[-5].minor.yy0.str = 0;
	yylhsminor.yy255 = node(D_WHEN, yymsp[-5].minor.yy0, yymsp[-3].minor.yy255, yymsp[-1].minor.yy255);
	report("expected 'state' or 'exit'\n");
}
#line 2099 "./snl.c"
  yymsp[-5].minor.yy255 = yylhsminor.yy255;
        break;
      case 133: /* block ::= LBRACE block_defns statements RBRACE */
#line 363 "../snl.lem"
{
	yylhsminor.yy255 = node(S_CMPND, yymsp[-3].minor.yy0, yymsp[-2].minor.yy255, yymsp[-1].minor.yy255);
}
#line 2107 "./snl.c"
  yymsp[-3].minor.yy255 = yylhsminor.yy255;
        break;
      case 140: /* statement ::= BREAK SEMICOLON */
      case 141: /* statement ::= CONTINUE SEMICOLON */ yytestcase(yyruleno==141);
#line 380 "../snl.lem"
{ yylhsminor.yy255 = node(S_JUMP, yymsp[-1].minor.yy0); }
#line 2114 "./snl.c"
  yymsp[-1].minor.yy255 = yylhsminor.yy255;
        break;
      case 142: /* statement ::= RETURN opt_expr SEMICOLON */
#line 383 "../snl.lem"
{ yylhsminor.yy255 = node(S_RETURN, yymsp[-2].minor.yy0, yymsp[-1].minor.yy255); }
#line 2120 "./snl.c"
  yymsp[-2].minor.yy255 = yylhsminor.yy255;
        break;
      case 143: /* statement ::= STATE NAME SEMICOLON */
#line 384 "../snl.lem"
{ yymsp[-2].minor.yy255 = node(S_CHANGE, yymsp[-1].minor.yy0); }
#line 2126 "./snl.c"
        break;
      case 146: /* statement ::= IF LPAREN comma_expr RPAREN statement */
#line 388 "../snl.lem"
{ yylhsminor.yy255 = node(S_IF, yymsp[-4].minor.yy0, yymsp[-2].minor.yy255, yymsp[0].minor.yy255, NIL); }
#line 2131 "./snl.c"
  yymsp[-4].minor.yy255 = yylhsminor.yy255;
        break;
      case 147: /* statement ::= IF LPAREN comma_expr RPAREN statement ELSE statement */
#line 390 "../snl.lem"
{ yylhsminor.yy255 = node(S_IF, yymsp[-6].minor.yy0, yymsp[-4].minor.yy255, yymsp[-2].minor.yy255, yymsp[0].minor.yy255); }
#line 2137 "./snl.c"
  yymsp[-6].minor.yy255 = yylhsminor.yy255;
        break;
      case 148: /* statement ::= WHILE LPAREN comma_expr RPAREN statement */
#line 392 "../snl.lem"
{ yylhsminor.yy255 = node(S_WHILE, yymsp[-4].minor.yy0, yymsp[-2].minor.yy255, yymsp[0].minor.yy255); }
#line 2143 "./snl.c"
  yymsp[-4].minor.yy255 = yylhsminor.yy255;
        break;
      case 150: /* statement ::= opt_expr SEMICOLON */
#line 394 "../snl.lem"
{ yylhsminor.yy255 = node(S_STMT, yymsp[0].minor.yy0, yymsp[-1].minor.yy255); }
#line 2149 "./snl.c"
  yymsp[-1].minor.yy255 = yylhsminor.yy255;
        break;
      case 151: /* for_statement ::= FOR LPAREN opt_expr SEMICOLON opt_expr SEMICOLON opt_expr RPAREN statement */
#line 400 "../snl.lem"
{ yylhsminor.yy255 = node(S_FOR, yymsp[-8].minor.yy0, yymsp[-6].minor.yy255, yymsp[-4].minor.yy255, yymsp[-2].minor.yy255, yymsp[0].minor.yy255); }
#line 2155 "./snl.c"
  yymsp[-8].minor.yy255 = yylhsminor.yy255;
        break;
      case 156: /* expr ::= LPAREN comma_expr RPAREN */
#line 415 "../snl.lem"
{ yylhsminor.yy255 = node(E_PAREN,  yymsp[-2].minor.yy0, yymsp[-1].minor.yy255); }
#line 2161 "./snl.c"
  yymsp[-2].minor.yy255 = yylhsminor.yy255;
        break;
      case 157: /* expr ::= expr LPAREN args RPAREN */
#line 416 "../snl.lem"
{ yylhsminor.yy255 = node(E_FUNC,   yymsp[-2].minor.yy0, yymsp[-3].minor.yy255, yymsp[-1].minor.yy255); }
#line 2167 "./snl.c"
  yymsp[-3].minor.yy255 = yylhsminor.yy255;
        break;
      case 158: /* expr ::= EXIT LPAREN args RPAREN */
      case 159: /* expr ::= SIZEOF LPAREN type_expr RPAREN */ yytestcase(yyruleno==159);
#line 417 "../snl.lem"
{ yylhsminor.yy255 = node(E_FUNC,   yymsp[-2].minor.yy0, node(E_VAR, yymsp[-3].minor.yy0), yymsp[-1].minor.yy255); }
#line 2174 "./snl.c"
  yymsp[-3].minor.yy255 = yylhsminor.yy255;
        break;
      case 160: /* expr ::= expr LBRACKET expr RBRACKET */
#line 419 "../snl.lem"
{ yylhsminor.yy255 = node(E_SUBSCR, yymsp[-2].minor.yy0, yymsp[-3].minor.yy255, yymsp[-1].minor.yy255); }
#line 2180 "./snl.c"
  yymsp[-3].minor.yy255 = yylhsminor.yy255;
        break;
      case 161: /* expr ::= expr PERIOD member */
      case 162: /* expr ::= expr POINTER member */ yytestcase(yyruleno==162);
#line 420 "../snl.lem"
{ yylhsminor.yy255 = node(E_SELECT, yymsp[-1].minor.yy0, yymsp[-2].minor.yy255, yymsp[0].minor.yy255); }
#line 2187 "./snl.c"
  yymsp[-2].minor.yy255 = yylhsminor.yy255;
        break;
      case 163: /* expr ::= expr INCR */
      case 164: /* expr ::= expr DECR */ yytestcase(yyruleno==164);
#line 422 "../snl.lem"
{ yylhsminor.yy255 = node(E_POST,   yymsp[0].minor.yy0, yymsp[-1].minor.yy255); }
#line 2194 "./snl.c"
  yymsp[-1].minor.yy255 = yylhsminor.yy255;
        break;
      case 173: /* expr ::= SIZEOF expr */
#line 434 "../snl.lem"
{ yylhsminor.yy255 = node(E_FUNC, yymsp[-1].minor.yy0, node(E_VAR, yymsp[-1].minor.yy0), yymsp[0].minor.yy255); }
#line 2200 "./snl.c"
  yymsp[-1].minor.yy255 = yylhsminor.yy255;
        break;
      case 174: /* expr ::= LPAREN type_expr RPAREN expr */
#line 437 "../snl.lem"
{ yylhsminor.yy255 = node(E_CAST, yymsp[-3].minor.yy0, yymsp[-2].minor.yy255, yymsp[0].minor.yy255); }
#line 2206 "./snl.c"
  yymsp[-3].minor.yy255 = yylhsminor.yy255;
        break;
      case 193: /* expr ::= expr QUESTION expr COLON expr */
#line 461 "../snl.lem"
{ yylhsminor.yy255 = node(E_TERNOP, yymsp[-3].minor.yy0, yymsp[-4].minor.yy255, yymsp[-2].minor.yy255, yymsp[0].minor.yy255); }
#line 2212 "./snl.c"
  yymsp[-4].minor.yy255 = yylhsminor.yy255;
        break;
      case 212: /* string ::= STRCON */
#line 489 "../snl.lem"
{ yylhsminor.yy255 = node(E_STRING, yymsp[0].minor.yy0); }
#line 2218 "./snl.c"
  yymsp[0].minor.yy255 = yylhsminor.yy255;
        break;
      case 213: /* member ::= NAME */
#line 491 "../snl.lem"
{ yylhsminor.yy255 = node(E_MEMBER, yymsp[0].minor.yy0); }
#line 2224 "./snl.c"
  yymsp[0].minor.yy255 = yylhsminor.yy255;
        break;
      case 214: /* funcdef ::= basetype declarator block */
#line 495 "../snl.lem"
{
	yylhsminor.yy255 = node(D_FUNCDEF, yymsp[-1].minor.yy255->token, mk_decl(yymsp[-1].minor.yy255, yymsp[-2].minor.yy149), yymsp[0].minor.yy255);
}
#line 2232 "./snl.c"
  yymsp[-2].minor.yy255 = yylhsminor.yy255;
        break;
      case 215: /* structdef ::= STRUCT NAME members SEMICOLON */
#line 501 "../snl.lem"
{ yymsp[-3].minor.yy255 = node(D_STRUCTDEF, yymsp[-2].minor.yy0, yymsp[-1].minor.yy255); }
#line 2238 "./snl.c"
        break;
      case 219: /* member_decl ::= basetype declarator SEMICOLON */
#line 508 "../snl.lem"
{ yylhsminor.yy255 = mk_decl(yymsp[-1].minor.yy255, yymsp[-2].minor.yy149); }
#line 2243 "./snl.c"
  yymsp[-2].minor.yy255 = yylhsminor.yy255;
        break;
      case 221: /* c_code ::= CCODE */
#line 513 "../snl.lem"
{ yylhsminor.yy255 = node(T_TEXT, yymsp[0].minor.yy0); }
#line 2249 "./snl.c"
  yymsp[0].minor.yy255 = yylhsminor.yy255;
        break;
      default:
      /* (222) to ::= TO */ yytestcase(yyruleno==222);
      /* (223) to ::= */ yytestcase(yyruleno==223);
        break;
/********** End reduce actions ************************************************/
  };
  assert( yyruleno<sizeof(yyRuleInfo)/sizeof(yyRuleInfo[0]) );
  yygoto = yyRuleInfo[yyruleno].lhs;
  yysize = yyRuleInfo[yyruleno].nrhs;
  yyact = yy_find_reduce_action(yymsp[-yysize].stateno,(YYCODETYPE)yygoto);
  if( yyact <= YY_MAX_SHIFTREDUCE ){
    if( yyact>YY_MAX_SHIFT ){
      yyact += YY_MIN_REDUCE - YY_MIN_SHIFTREDUCE;
    }
    yymsp -= yysize-1;
    yypParser->yytos = yymsp;
    yymsp->stateno = (YYACTIONTYPE)yyact;
    yymsp->major = (YYCODETYPE)yygoto;
    yyTraceShift(yypParser, yyact);
  }else{
    assert( yyact == YY_ACCEPT_ACTION );
    yypParser->yytos -= yysize;
    yy_accept(yypParser);
  }
}

/*
** The following code executes when the parse fails
*/
#ifndef YYNOERRORRECOVERY
static void yy_parse_failed(
  yyParser *yypParser           /* The parser */
){
  snlParserARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yytos>yypParser->yystack ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
/************ Begin %parse_failure code ***************************************/
#line 29 "../snl.lem"

	report("parser giving up\n");
	exit(1);
#line 2299 "./snl.c"
/************ End %parse_failure code *****************************************/
  snlParserARG_STORE; /* Suppress warning about unused %extra_argument variable */
}
#endif /* YYNOERRORRECOVERY */

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  snlParserTOKENTYPE yyminor         /* The minor type of the error token */
){
  snlParserARG_FETCH;
#define TOKEN yyminor
/************ Begin %syntax_error code ****************************************/
#line 34 "../snl.lem"

	error_at(TOKEN.file, TOKEN.line,
		"syntax error near token '%s'\n", TOKEN.str);
	report_loc(TOKEN.file, TOKEN.line);
#line 2321 "./snl.c"
/************ End %syntax_error code ******************************************/
  snlParserARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  snlParserARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
#ifndef YYNOERRORRECOVERY
  yypParser->yyerrcnt = -1;
#endif
  assert( yypParser->yytos==yypParser->yystack );
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
/*********** Begin %parse_accept code *****************************************/
/*********** End %parse_accept code *******************************************/
  snlParserARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "snlParserAlloc" which describes the current state of the parser.
** The second argument is the major token number.  The third is
** the minor token.  The fourth optional argument is whatever the
** user wants (and specified in the grammar) and is available for
** use by the action routines.
**
** Inputs:
** <ul>
** <li> A pointer to the parser (an opaque structure.)
** <li> The major token number.
** <li> The minor token number.
** <li> An option argument of a grammar-specified type.
** </ul>
**
** Outputs:
** None.
*/
void snlParser(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  snlParserTOKENTYPE yyminor       /* The value for the token */
  snlParserARG_PDECL               /* Optional %extra_argument parameter */
){
  YYMINORTYPE yyminorunion;
  unsigned int yyact;   /* The parser action. */
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  int yyendofinput;     /* True if we are at the end of input */
#endif
#ifdef YYERRORSYMBOL
  int yyerrorhit = 0;   /* True if yymajor has invoked an error */
#endif
  yyParser *yypParser;  /* The parser */

  yypParser = (yyParser*)yyp;
  assert( yypParser->yytos!=0 );
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  yyendofinput = (yymajor==0);
#endif
  snlParserARG_STORE;

#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sInput '%s'\n",yyTracePrompt,yyTokenName[yymajor]);
  }
#endif

  do{
    yyact = yy_find_shift_action(yypParser,(YYCODETYPE)yymajor);
    if( yyact <= YY_MAX_SHIFTREDUCE ){
      yy_shift(yypParser,yyact,yymajor,yyminor);
#ifndef YYNOERRORRECOVERY
      yypParser->yyerrcnt--;
#endif
      yymajor = YYNOCODE;
    }else if( yyact <= YY_MAX_REDUCE ){
      yy_reduce(yypParser,yyact-YY_MIN_REDUCE);
    }else{
#ifdef YYERRORSYMBOL
      int yymx;
#endif
      assert( yyact == YY_ERROR_ACTION );
      yyminorunion.yy0 = yyminor;
#ifndef NDEBUG
      if( yyTraceFILE ){
        fprintf(yyTraceFILE,"%sSyntax Error!\n",yyTracePrompt);
      }
#endif
#ifdef YYERRORSYMBOL
      /* A syntax error has occurred.
      ** The response to an error depends upon whether or not the
      ** grammar defines an error token "ERROR".  
      **
      ** This is what we do if the grammar does define ERROR:
      **
      **  * Call the %syntax_error function.
      **
      **  * Begin popping the stack until we enter a state where
      **    it is legal to shift the error symbol, then shift
      **    the error symbol.
      **
      **  * Set the error count to three.
      **
      **  * Begin accepting and shifting new tokens.  No new error
      **    processing will occur until three tokens have been
      **    shifted successfully.
      **
      */
      if( yypParser->yyerrcnt<0 ){
        yy_syntax_error(yypParser,yymajor,yyminor);
      }
      yymx = yypParser->yytos->major;
      if( yymx==YYERRORSYMBOL || yyerrorhit ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE,"%sDiscard input token %s\n",
             yyTracePrompt,yyTokenName[yymajor]);
        }
#endif
        yy_destructor(yypParser, (YYCODETYPE)yymajor, &yyminorunion);
        yymajor = YYNOCODE;
      }else{
        while( yypParser->yytos > yypParser->yystack
            && yymx != YYERRORSYMBOL
            && (yyact = yy_find_reduce_action(
                        yypParser->yytos->stateno,
                        YYERRORSYMBOL)) >= YY_MIN_REDUCE
        ){
          yy_pop_parser_stack(yypParser);
        }
        assert( yypParser->yytos >= yypParser->yystack );
        if( yypParser->yytos == yypParser->yystack || yymajor==0 ){
          yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
          yy_parse_failed(yypParser);
#ifndef YYNOERRORRECOVERY
          yypParser->yyerrcnt = -1;
#endif
          yymajor = YYNOCODE;
        }else if( yymx!=YYERRORSYMBOL ){
          yy_shift(yypParser,yyact,YYERRORSYMBOL,yyminor);
        }
      }
      yypParser->yyerrcnt = 3;
      yyerrorhit = 1;
#elif defined(YYNOERRORRECOVERY)
      /* If the YYNOERRORRECOVERY macro is defined, then do not attempt to
      ** do any kind of error recovery.  Instead, simply invoke the syntax
      ** error routine and continue going as if nothing had happened.
      **
      ** Applications can set this macro (for example inside %include) if
      ** they intend to abandon the parse upon the first syntax error seen.
      */
      yy_syntax_error(yypParser,yymajor, yyminor);
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      yymajor = YYNOCODE;
      
#else  /* YYERRORSYMBOL is not defined */
      /* This is what we do if the grammar does not define ERROR:
      **
      **  * Report an error message, and throw away the input token.
      **
      **  * If the input token is $, then fail the parse.
      **
      ** As before, subsequent error messages are suppressed until
      ** three input tokens have been successfully shifted.
      */
      if( yypParser->yyerrcnt<=0 ){
        yy_syntax_error(yypParser,yymajor, yyminor);
      }
      yypParser->yyerrcnt = 3;
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      if( yyendofinput ){
        yy_parse_failed(yypParser);
#ifndef YYNOERRORRECOVERY
        yypParser->yyerrcnt = -1;
#endif
      }
      yymajor = YYNOCODE;
#endif
    }
  }while( yymajor!=YYNOCODE && yypParser->yytos>yypParser->yystack );
#ifndef NDEBUG
  if( yyTraceFILE ){
    yyStackEntry *i;
    char cDiv = '[';
    fprintf(yyTraceFILE,"%sReturn. Stack=",yyTracePrompt);
    for(i=&yypParser->yystack[1]; i<=yypParser->yytos; i++){
      fprintf(yyTraceFILE,"%c%s", cDiv, yyTokenName[i->major]);
      cDiv = ' ';
    }
    fprintf(yyTraceFILE,"]\n");
  }
#endif
  return;
}
